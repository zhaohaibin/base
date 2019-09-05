#ifndef BASE_SOCKET_H
#define BASE_SOCKET_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <string>
using namespace std;
namespace base
{
	namespace net
	{
		namespace udp
		{
			typedef boost::asio::ip::udp::socket socket_impl;
			typedef boost::asio::ip::udp::endpoint endpoint_impl;
			#define bind_impl boost::bind
			#define bind_impl_placeholder boost::placeholders
			#define read_buffer_size 1024*100

			class socket_handler
			{
				friend class socket;
			public:
				virtual ~socket_handler();
			protected:
				virtual void read_handler(const endpoint_impl& sender_endpoint, char* buffer, size_t length) = 0;
				virtual void read_error_handler(boost::system::error_code code) = 0;
				virtual void write_handler(size_t lenght) = 0;
				virtual void write_error_handler(boost::system::error_code code) = 0;
			};

			class socket
			{
			public:
				socket(const string& ip, unsigned short port, socket_handler* p_socket_handler);
				~socket();
				
				
				void async_write(const endpoint_impl& endpoint, char* buffer, int length);
				endpoint_impl local_endpoint();
			private:
				void work_thread();
				void do_async_read();
			private:
				void write_handler(boost::system::error_code code, std::size_t length);
				void read_handler(boost::system::error_code code, std::size_t length);
			private:
				char* m_read_buffer;

				endpoint_impl m_endpoint;
				endpoint_impl m_sender_endpoint;
				boost::asio::io_context m_io_context;
				socket_impl m_sock_impl;

				socket_handler* m_p_socket_handler;
				boost::thread m_work_thread;
			};
		}
	}
}
#endif //BASE_SOCKET_H