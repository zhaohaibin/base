#include "session.h"
#include "session_handler.h"
#include <iostream>
using namespace std;
session::session(tcp::socket socket, session_handler* p_session_handler)
	: m_socket(std::move(socket))
	, m_buffer(new char[buffer_size])
	, m_psession_handler(p_session_handler)
	, m_do_write_busy(false)
{
}

session::~session()
{
	delete[]m_buffer;
	cout << "session destroy" << endl;
}

void session::start()
{
	do_read();
}

bool session::async_write(char* buffer, int length)
{
	int ret = push_to_write_queue(buffer, length);
	if (ret == 1)
	{
		m_do_write_busy = true;
		do_write();
	}
	else
	{
		if (ret == -1)
			return false;
	}
	return true;
}

std::string session::remote_ip()
{
	return m_socket.remote_endpoint().address().to_string();
}

unsigned short session::remote_port()
{
	return m_socket.remote_endpoint().port();
}

std::string session::local_ip()
{
	return m_socket.local_endpoint().address().to_string();
}

unsigned short session::local_port()
{
	return m_socket.local_endpoint().port();
}

void session::do_read()
{
	auto self(shared_from_this());
	m_socket.async_read_some(boost::asio::buffer(m_buffer, buffer_size),
		[this, self](boost::system::error_code ec, std::size_t length)
	{
		if (!ec)
		{
			if (m_psession_handler)
			{
				m_psession_handler->read_handler(self, m_buffer, length);
			}
			do_read();
		}
		else
		{
			m_psession_handler->read_error_handler(ec);
			m_psession_handler = nullptr;
		}
	});
}

void session::do_write()
{
	if (front_write_queue(m_write_buffer))
	{
		auto self(shared_from_this());
		boost::asio::async_write(m_socket, boost::asio::buffer(m_write_buffer._buffer, m_write_buffer._length),
			[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				m_psession_handler->write_handler(self,m_write_buffer._buffer, length);
				if (pop_front_write_queue() > 0)
				{
					do_write();
				}
			}
			else
			{
				m_psession_handler->write_error_handler(ec, m_write_buffer._buffer);
				m_psession_handler = nullptr;
			}
		});
	}
}

int session::push_to_write_queue(char* buffer, int length)
{
	boost::mutex::scoped_lock locker(m_write_buffer_queue_mutex);
	if (m_write_buffer_queue.size() >= max_write_buffer_queue_size)
		return -1;

	m_write_buffer_queue.push_back(write_buffer(buffer, length));
	return m_write_buffer_queue.size();
}

bool session::front_write_queue(write_buffer& buffer)
{
	boost::mutex::scoped_lock locker(m_write_buffer_queue_mutex);
	if (m_write_buffer_queue.size() > 0)
	{
		buffer = m_write_buffer_queue.front();
		return true;
	}
	return false;
}

int session::pop_front_write_queue()
{
	boost::mutex::scoped_lock locker(m_write_buffer_queue_mutex);
	m_write_buffer_queue.pop_front();
	return m_write_buffer_queue.size();
}