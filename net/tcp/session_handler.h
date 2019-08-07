#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H
#include <memory>
#include <boost/system/error_code.hpp>

class session;
class session_handler
{
public:
	session_handler();
	virtual ~session_handler();

public:
	//此函数在连接服务器成功后被调用
	virtual void connect_handler(std::shared_ptr<session> sp_session);
	//此函数在连接服务器失败后被调用
	virtual void connect_error_handler(boost::system::error_code ec);

	//此函数在服务器接收到一个新的连接并为连接建立会话后被调用
	virtual void accept_handler(std::shared_ptr<session> sp_session);
	
	virtual void read_handler(std::shared_ptr<session> sp_session, char* buffer, size_t length);
	virtual void write_handler(std::shared_ptr<session> sp_session,char* buffer, size_t length);
	virtual void read_error_handler(boost::system::error_code ec);
	virtual void write_error_handler(boost::system::error_code ec, char* buffer);
};
#endif //DATA_HANDLER_H