#include "charge_s.hpp"

charge_s::charge_s(double load)
{
	this->load = load;
}

charge_s::charge_s(const charge_s& C)
{
	this->load = C.load;
}

charge_s::charge_s(const charge& C)
{
	this->load = C.getLoad();
}

charge_s::~charge_s()
{
	/* Nothing to do */
}

double charge_s::getLoad() const
{
	return this->load;
}

double& charge_s::getLoad()
{
	return this->load;
}

void charge_s::setLoad(double load)
{
	this->load = load;
}

charge_s& charge_s::operator = (const charge_s& C)
{
	this->load = C.load;

	return *this;
}

charge_s& charge_s::operator = (const charge& C)
{
	this->load = C.getLoad();

	return *this;
}

charge_s& charge_s::operator = (double load)
{
	this->load = load;

	return *this;
}

charge_s::operator charge() const
{
	return charge(this->load);
}

