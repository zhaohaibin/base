#include "server.h"
#include "session.h"
#include "session_handler.h"

std::shared_ptr<session> session_factory::create_session(tcp::socket socket)
{
	return std::make_shared<session>(std::move(socket), new session_handler());
}

server::server(const string& ip, unsigned short port, session_factory* pfactory)
	: m_ip(ip)
	, m_port(port)
	, m_endPoint(tcp::v4(), port)
	, m_acceptor(m_ioContext, m_endPoint)
	, m_psession_factory(pfactory)
{
}

void server::run()
{
	do_accecpt();
	m_ioContext.run();
}

void server::do_accecpt()
{
	m_acceptor.async_accept(
		[this](boost::system::error_code ec, tcp::socket socket)
	{
		if (!ec)
		{
			m_psession_factory->create_session(std::move(socket))->start();
		}
		do_accecpt();
	});
}

