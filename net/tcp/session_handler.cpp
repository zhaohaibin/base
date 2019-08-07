#include "session_handler.h"
session_handler::session_handler()
{

}

session_handler::~session_handler()
{

}

void session_handler::connect_handler(std::shared_ptr<session> sp_session)
{

}

void session_handler::connect_error_handler(boost::system::error_code ec)
{
	delete this;
}

void session_handler::accept_handler(std::shared_ptr<session> sp_session)
{

}

void session_handler::read_handler(std::shared_ptr<session> sp_session,char* buffer, size_t length)
{

}

void session_handler::write_handler(std::shared_ptr<session> sp_session,char* buffer, size_t length)
{

}

void session_handler::read_error_handler(boost::system::error_code ec)
{
	delete this;
}

void session_handler::write_error_handler(boost::system::error_code ec, char* buffer)
{
	delete this;
}