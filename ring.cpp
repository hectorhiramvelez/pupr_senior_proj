#include "ring.hpp"

#include <cmath>
#include <ctime>
using namespace std;

ring::ring(size_t charges, double radius)
{
	this->_radius = radius;
	this->_charges = charges;
	this->C = 0;
	this->_size = 0;

	this->rnd = new CRandomMother(0);
	this->rnd_init = false;
}

ring::ring(double radius, size_t charges)
{
	this->_radius = radius;
	this->_charges = charges;
	this->C = 0;
	this->_size = 0;

	this->rnd = new CRandomMother(0);
	this->rnd_init = false;
}

ring::ring(const ring& R)
{
	this->_radius = R._radius;
	this->_charges = R._charges;

	if (R.C != 0)
	{
		size_t i;

		this->C = new charge[R._size];
		this->_size = R._size;

		i = 0;
		while (i < this->_size)
		{
			this->C[i] = R.C[i];
			i++;
		}
	}
	else
	{
		this->C = 0;
		this->_size = 0;
	}

	this->rnd = new CRandomMother(0);
	this->rnd_init = false;
}

ring::~ring()
{
	if (this->C != 0) delete[] this->C;
	delete this->rnd;
}

double ring::getRadius() const
{
	return this->_radius;
}

double& ring::getRadius()
{
	return this->_radius;
}

double ring::radius() const
{
	return this->_radius;
}

double& ring::radius()
{
	return this->_radius;
}

void ring::setRadius(double radius)
{
	this->_radius = radius;
}

size_t ring::getCharges() const
{
	return this->_charges;
}

size_t& ring::getCharges()
{
	return this->_charges;
}

size_t ring::charges() const
{
	return this->_charges;
}

size_t& ring::charges()
{
	return this->_charges;
}

void ring::setCharges(size_t charges)
{
	this->_charges = charges;
}

const charge* ring::getCharge() const
{
	return this->C;
}

const charge& ring::getCharge(size_t index) const
{
	return this->C[index];
}

charge* ring::getCharge()
{
	return this->C;
}

charge& ring::getCharge(size_t index)
{
	return this->C[index];
}

/*
const charge* ring::charge() const
{
	return this->C;
}

const charge& ring::charge(size_t index) const
{
	return this->C[index];
}

charge* ring::charge()
{
	return this->C;
}

charge& ring::charge(size_t index)
{
	return this->C[index];
}
*/

size_t ring::size() const
{
	return this->_size;
}

void ring::initialize(unsigned int _rand)
{
	if ((this->C != 0) && (this->_size != this->_charges))
	{
		delete[] this->C;
		this->C = 0;
	}

	if (this->_charges > 0)
	{
		size_t i, cpos, csize;
		double rad;

		if (this->C == 0) this->C = new charge[this->_charges];
		this->_size = this->_charges;

		if (this->rnd_init == false)
		{
			this->rnd->RandomInit(time(0) + _rand);
		}

		i = 0;
		csize = (this->_size / 2) + 2;
		while (i < this->_size)
		{
			cpos = (i / 2) + 1;
			rad = (M_PI * ((cpos + this->rnd->Random()) / csize)) + (M_PI * (i % 2)) - (M_PI * (90.0 / 180.0));
			this->C[i] = vector3<double>(cos(rad) * this->_radius, sin(rad) * this->_radius, 0); /* Commit the new charge position */
			i++;

			/*
			The following rules are followed by the above function:
				1. Charges should not be near the poles
				2. Charges should be load balanced between the left and right sides of the ring
				3. Charges should not be close to each other (NOTE: not 100% followed)
			*/
		}
	}
	else
	{
		this->C = 0;
		this->_size = 0;
	}
}

void ring::rotate(double rad)
{
	size_t i;

	i = 0;
	while (i < this->_size)
	{
		this->C[i] = this->C[i].rotate(0, rad, 0);
		i++;
	}
}

ring& ring::operator = (const ring& R)
{
	if (this == &R) return *this;
	if (this->C != 0) delete[] this->C;

	this->_radius = R._radius;
	this->_charges = R._charges;

	if (R.C != 0)
	{
		size_t i;

		this->C = new charge[R._size];
		this->_size = R._size;

		i = 0;
		while (i < this->_size)
		{
			this->C[i] = R.C[i];
			i++;
		}
	}
	else
	{
		this->C = 0;
		this->_size = 0;
	}

	return *this;
}

