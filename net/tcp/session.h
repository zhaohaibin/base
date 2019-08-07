#ifndef SESSION_H
#define SESSION_H
#include <memory>
#include <queue>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
using boost::asio::ip::tcp;
class session_handler;
class session : public std::enable_shared_from_this<session>
{
	static const int buffer_size = 1024 * 10;
	static const int max_write_buffer_queue_size = 1024;
	struct write_buffer
	{
		write_buffer()
			: _buffer(nullptr)
			, _length(0)
		{
		}
		write_buffer(char* buffer, int lenght)
			: _buffer(buffer)
			, _length(lenght)
		{
		}
		char* _buffer;
		int _length;
	};
public:
	session(tcp::socket socket, session_handler* p_session_handler);
	virtual ~session();

	void start();
	bool async_write(char* buffer, int length);

	std::string remote_ip();
	unsigned short remote_port();

	std::string local_ip();
	unsigned short local_port();
private:
	void do_read();
	void do_write();

	int push_to_write_queue(char* buffer, int length);
	bool front_write_queue(write_buffer& buffer);
	int pop_front_write_queue();
private:
	tcp::socket m_socket;
	char* m_buffer;

	write_buffer m_write_buffer;
	boost::mutex m_write_buffer_queue_mutex;
	std::deque<write_buffer> m_write_buffer_queue;
	bool m_do_write_busy;

	session_handler* m_psession_handler;
};
#endif //SESSION_H