#pragma once

#ifndef RINGS_THREAD_HPP
#define RINGS_THREAD_HPP

#include "progress.hpp"
#include "ring.hpp"

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <vector>

class rings_thread
{
	private:
		mutable boost::mutex _lock;
		size_t times;
		size_t rings;
		size_t charges;
		double radius;

		double distrib;
		charge* _charge;

		boost::thread* th;

		size_t _size() const;

		progress p;

	public:
		rings_thread(size_t times = 1, size_t rings = 6, size_t charges = 10, double radius = 1.0, size_t _rand = 0);
		~rings_thread();

		static bool run(rings_thread* _this, unsigned int _rand = 0);
		double getProgress() const;
		bool finished() const;

		double getDistrib() const;
		charge* getCharges();

		size_t size() const;
};

#endif
