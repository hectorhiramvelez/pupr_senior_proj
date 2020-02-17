#include "charge.hpp"

charge::charge(double load) : vector3<double>()
{
	this->load = load;
}

charge::charge(const vector3<double> V, double load) : vector3<double>(V)
{
	this->load = load;
}

charge::charge(double X, double Y, double Z, double load) : vector3<double>(X, Y, Z)
{
	this->load = load;
}

charge::charge(const charge& C) : vector3<double>(C)
{
	this->load = C.load;
}

charge::~charge()
{
	/* Nothing to do */
	this->vector3<double>::~vector3(); /* Base class destructor */
}

double charge::getLoad() const
{
	return this->load;
}

double& charge::getLoad()
{
	return this->load;
}

void charge::setLoad(const double load)
{
	this->load = load;
}

charge& charge::operator = (const charge& C)
{
	vector3<double>::operator = (C);
	this->load = C.load;

	return *this;
}

charge& charge::operator = (double load)
{
	this->load = load;

	return *this;
}

