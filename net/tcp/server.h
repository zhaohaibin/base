#ifndef SERVER_H
#define SERVER_H
#include <string>
#include <boost/asio.hpp>
#include "session.h"
using namespace std;
using boost::asio::ip::tcp;
class session_factory
{
public:
	virtual std::shared_ptr<session> create_session(tcp::socket socket);
};

class server
{
public:
	server(const string& ip, unsigned short port, session_factory* pfactory);
	void run();
private:
	void do_accecpt();
private:
	string m_ip;
	unsigned short m_port;
	boost::asio::io_context m_ioContext;
	tcp::endpoint m_endPoint;
	tcp::acceptor m_acceptor;
	session_factory* m_psession_factory;
};
#endif //SERVER_H