#ifndef TIMER_H
#define TIMER_H

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
class timer
{
	class timer_impl
	{
	public:
		timer_impl();
		void set_timer(unsigned int milliseconds,
			boost::function<void(const boost::system::error_code&)> handler);
	private:
		void work_thread();
	private:
		boost::asio::io_service m_io_service;
		boost::asio::io_service::work m_work;
		boost::asio::deadline_timer m_timer;
		
		boost::thread m_thread;
	};

public:
	timer();
	~timer();
	void set_timer(unsigned int milliseconds,
		boost::function<void(timer&, const boost::system::error_code&)> handler);

public:
	void timeout(boost::system::error_code code);
private:
	timer_impl* m_p_timer_impl;
	boost::function<void(timer&, const boost::system::error_code&)> m_timer_hander;
};
#endif //TIMER_H