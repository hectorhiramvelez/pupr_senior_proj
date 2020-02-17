#include "charge_c.hpp"

charge_c::charge_c(charge_s* C, vector3<double> V) : vector3<double>(V)
{
	this->C = C;
}

charge_c::charge_c(charge_s& C, vector3<double> V) : vector3<double>(V)
{
	this->C = &C;
}

charge_c::charge_c(const charge_c& C) : vector3<double>(C)
{
	this->C = C.C;
}

charge_c::~charge_c()
{
	/* Nothing to do */
	/* NOTE: this->C pointer is just a link */
}

double charge_c::getLoad() const
{
	return this->C->getLoad();
}

double& charge_c::getLoad()
{
	return this->C->getLoad();
}

void charge_c::setLoad(double load)
{
	this->C->setLoad(load);
}

void charge_c::setLink(charge_s* C)
{
	this->C = C;
}

charge_c& charge_c::operator = (const charge_c& C)
{
	this->C = C.C;
	vector3<double>::operator = (C);

	return *this;
}

charge_c& charge_c::operator = (charge_s& C)
{
	this->C = &C;

	return *this;
}

charge_c& charge_c::operator = (double load)
{
	this->C->setLoad(load);

	return *this;
}

charge_c::operator charge() const
{
	return charge(*this, this->C->getLoad());
}


