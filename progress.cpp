#include "progress.hpp"

progress::progress(double value)
{
	this->setProgress(value);
}

progress::progress(const progress& P)
{
	this->setProgress(P.getProgress());
}

progress::~progress()
{
	/* Nothing to do */
}

double progress::getProgress() const
{
	autolock __lock(this->_lock);
	return this->value;
}

double& progress::getProgress()
{
	autolock __lock(this->_lock);
	return this->value;
}

void progress::setProgress(double value)
{
	autolock __lock(this->_lock);
	this->value = value;
}

progress::operator double() const
{
	return this->getProgress();
}

progress& progress::operator = (const progress& P)
{
	this->setProgress(P.getProgress());
	return *this;
}

progress& progress::operator = (double value)
{
	this->setProgress(value);
	return *this;
}

