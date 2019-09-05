#include "socket.h"
namespace base
{
	namespace net
	{
		namespace udp
		{

			socket_handler::~socket_handler()
			{

			}


			///////////////////////////////////////////////////////////////////////////////////////////////////////////////

			socket::socket(const string& ip, unsigned short port, socket_handler* p_socket_handler) 
				: m_read_buffer(new char[read_buffer_size])
				, m_endpoint(boost::asio::ip::address::from_string(ip), port)
				, m_sock_impl(m_io_context, m_endpoint)
				, m_p_socket_handler(p_socket_handler)
			{
				m_work_thread = boost::thread(bind_impl(&socket::work_thread, this));
			}

			socket::~socket()
			{
				if (m_read_buffer)
					delete[]m_read_buffer;
			}

			void socket::work_thread()
			{
				do_async_read();
				m_io_context.run();
			}

			void socket::do_async_read()
			{
				m_sock_impl.async_receive_from(
					boost::asio::buffer(m_read_buffer, read_buffer_size),
					m_sender_endpoint,
					bind_impl(&socket::read_handler, this, bind_impl_placeholder::_1, bind_impl_placeholder::_2));
			}

			void socket::async_write(const endpoint_impl& endpoint, char* buffer, int length)
			{
				m_sock_impl.async_send_to(
					boost::asio::buffer(buffer, length),
					endpoint,
					bind_impl(&socket::write_handler, this, bind_impl_placeholder::_1, bind_impl_placeholder::_2));
			}

			base::net::udp::endpoint_impl socket::local_endpoint()
			{
				return m_endpoint;
			}

			void socket::write_handler(boost::system::error_code code, std::size_t length)
			{
				if (!code)
				{
					m_p_socket_handler->write_handler(length);
				}
				else
				{
					m_p_socket_handler->write_error_handler(code);
				}
			}

			void socket::read_handler(boost::system::error_code code, std::size_t length)
			{
				if (!code)
				{
					m_p_socket_handler->read_handler(m_sender_endpoint, m_read_buffer, length);
					do_async_read();
				}
				else
				{
					m_p_socket_handler->read_error_handler(code);
				}
			}

		}
	}
}