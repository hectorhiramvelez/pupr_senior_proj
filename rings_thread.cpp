#include "rings_thread.hpp"

#include <boost/thread/thread.hpp>
#include <limits>
#include <randomc.h>

rings_thread::rings_thread(size_t times, size_t rings, size_t charges, double radius, size_t _rand)
{
	this->times = times;
	this->rings = rings;	
	this->charges = charges;
	this->radius = radius;
	this->_charge = 0;
	this->distrib = std::numeric_limits<double>::infinity();

	srand(time(0));

	this->_lock.lock(); /* Calling normal functions while this thread is executing will block */
	th = new boost::thread(this->run, this, rand() + _rand);
}

rings_thread::~rings_thread()
{
	delete th;
	//free(this->_charge);
	delete [] this->_charge;
}

size_t rings_thread::_size() const
{
	return this->charges * this->rings;
}

#include <cstdio>

bool rings_thread::run(rings_thread* _this, unsigned int _rand) // <-- ??? dyna-static function because i got no other name for it
{
	#ifdef this
	#undef this /* Should not happen */
	#endif
	
	#define this _this

	{
		size_t i, j, k, size;
		ring* _ring;
		charge* _charge;
		charge* tmp;
		double distrib;
		CRandomMother rnd(0);

		size = this->_size();

		if (this->_charge != 0) return false; // <-- Can only be run once
		//if ((this->_charge = reinterpret_cast<charge*>(malloc(sizeof(charge) * size))) == 0) return false; // <-- Out of memory
		if ((this->_charge = new charge[size]) == 0) return false;
		//if ((_ring = new ring[this->rings]) == 0) return false; /*  <-- Only 1 ring is needed in memory */
		if ((_ring = new ring) == 0) return false;
		if ((_charge = new charge[size]) == 0) return false;

		srand(time(0) + _rand);
		rnd.RandomInit((int)rand());

		_ring->setCharges(this->charges);
		_ring->setRadius(this->radius);

		i = 0;
		while (i < this->times)
		{
			j = 0;
			while (j < this->rings)
			{	
				_ring->initialize(rnd.BRandom());
				_ring->rotate((1.0 * M_PI) * (1.0 * j / this->rings));

				k = 0;
				tmp = _ring->getCharge();
				while (k < this->charges)
				{
					_charge[(j * this->charges) + k] = tmp[k];
					k++;
				}
				j++;
			}

			j = 0;
			distrib = 0;
			while (j < size)
			{
				k = j + 1;
				while (k < size)
				{
					double mag;
					if ((mag = _charge[j].difference(_charge[k]).magnitude()) != 0)
					{
						distrib += 1.0 / mag;
						//printf("%lf\n", _charge[j].difference(_charge[k]).magnitude());
					}
					else
					{
						//printf("Infinite hit (%ld, %ld, %ld)\n", j, k, size);
						distrib = std::numeric_limits<double>::infinity();
						k = j = size;
					}
					k++;
				}
				j++;
				//printf("*"); fflush(stdout);
			}

			if ((distrib < this->distrib))
			{
				//printf("New lower distribution: %lf --> %lf\n", this->distrib, distrib);
				this->distrib = distrib;
				j = 0;
				while (j < size)
				{
					this->_charge[j] = _charge[j];
					j++;
				}
				this->p.setProgress((i * 1.00) / (this->times * 1.00));
			}

			if ((i & 255) == 0)
			{
				this->p.setProgress((i * 1.00) / (this->times * 1.00));
			}
			i++;
		}

		this->p.setProgress(1.00);
		this->_lock.unlock();

		delete[] _charge;
		delete _ring;
		return true;
	}

	#undef this
}

double rings_thread::getProgress() const
{
	return this->p.getProgress();
}

bool rings_thread::finished() const
{
	if (this->_lock.try_lock() == true)
	{
		this->_lock.unlock();
		return true;
	}
	return false;
}

double rings_thread::getDistrib() const
{
	this->_lock.lock(); this->_lock.unlock();
	return this->distrib;
}

charge* rings_thread::getCharges()
{
	this->_lock.lock(); this->_lock.unlock();
	return this->_charge;
}

size_t rings_thread::size() const
{
	this->_lock.lock(); this->_lock.unlock();
	return this->_size();
}

