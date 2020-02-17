#include "autolock.hpp"

autolock::autolock(boost::mutex* _lock)
{
	this->_lock = _lock;
	if (this->_lock != 0) this->_lock->lock();
}

autolock::autolock(boost::mutex& _lock)
{
	this->_lock = &_lock;
	if (this->_lock != 0) this->_lock->lock();
}

autolock::~autolock()
{
	if (this->_lock != 0) this->_lock->unlock();
}

