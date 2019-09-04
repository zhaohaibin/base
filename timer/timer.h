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
		~timer_impl();
		void set_timer(unsigned int milliseconds,
			boost::function<void(const boost::system::error_code&)> handler);
	private:
		void work_thread();
	private:
		boost::asio::io_service m_io_service;
		boost::asio::io_service::work* m_p_work;
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

class timer_test
{
public:
	void timeout(timer&, const boost::system::error_code&)
	{
		boost::thread(boost::bind(&timer_test::delete_thread, this));
	}
	void run()
	{
		m_timer.set_timer(1000,boost::bind(&timer_test::timeout, this, boost::placeholders::_1, boost::placeholders::_2));
	}
	void delete_thread()
	{
		delete this;
	}
private:
	timer m_timer;

};
#endif //TIMER_H