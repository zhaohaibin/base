#include "timer.h"

timer::timer_impl::timer_impl()
	: m_timer(m_io_service)
	, m_work(m_io_service)
{
	m_thread = boost::thread(boost::bind(&timer::timer_impl::work_thread, this));
}

void timer::timer_impl::work_thread()
{
	m_io_service.run();
}

void timer::timer_impl::set_timer(unsigned int milliseconds,
	boost::function<void(const boost::system::error_code&)> handler)
{
	m_timer.expires_from_now(boost::posix_time::milliseconds(milliseconds));
	m_timer.async_wait(handler);
}

timer::timer()
{
	m_p_timer_impl = new timer_impl();
}

timer::~timer()
{
	if (m_p_timer_impl)
		delete m_p_timer_impl;
	m_p_timer_impl = nullptr;
}

void timer::set_timer(unsigned int milliseconds, boost::function<void(timer&, const boost::system::error_code&)> handler)
{
	m_timer_hander = handler;
	m_p_timer_impl->set_timer(milliseconds, boost::bind(&timer::timeout, this, boost::placeholders::_1));
}

void timer::timeout(boost::system::error_code code)
{
	m_timer_hander(*this, code);
}
