#pragma once

#ifndef PROGRESS_HPP
#define PROGRESS_HPP

#include "autolock.hpp"

#include <boost/thread/mutex.hpp>

class progress
{
	private:
		mutable boost::mutex _lock;
		double value;

	public:
		progress(double value = 0);
		progress(const progress& P);
		~progress();

		double getProgress() const;
		double& getProgress();
		void setProgress(double value = 0);

		operator double() const;

		progress& operator = (const progress& P);
		progress& operator = (double value);
};

#endif /* PROGRESS_HPP */
