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
	//�˺��������ӷ������ɹ��󱻵���
	virtual void connect_handler(std::shared_ptr<session> sp_session);
	//�˺��������ӷ�����ʧ�ܺ󱻵���
	virtual void connect_error_handler(boost::system::error_code ec);

	//�˺����ڷ��������յ�һ���µ����Ӳ�Ϊ���ӽ����Ự�󱻵���
	virtual void accept_handler(std::shared_ptr<session> sp_session);
	
	virtual void read_handler(std::shared_ptr<session> sp_session, char* buffer, size_t length);
	virtual void write_handler(std::shared_ptr<session> sp_session,char* buffer, size_t length);
	virtual void read_error_handler(boost::system::error_code ec);
	virtual void write_error_handler(boost::system::error_code ec, char* buffer);
};
#endif //DATA_HANDLER_H