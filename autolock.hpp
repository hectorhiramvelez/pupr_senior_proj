#pragma once

#ifndef AUTOLOCK_HPP
#define AUTOLOCK_HPP

#include <boost/thread/mutex.hpp>

class autolock
{
	private:
		boost::mutex* _lock;

	public:
		autolock(boost::mutex* _lock = 0);
		autolock(boost::mutex& _lock);
		~autolock();
};

#endif /* AUTOLOCK_HPP */
