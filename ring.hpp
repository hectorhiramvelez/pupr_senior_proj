#pragma once

#ifndef RING_HPP
#define RING_HPP

#include "charge.hpp"
#include <cstdlib>

#include <randomc.h>

class ring
{
	private:
		double _radius;
		size_t _charges;
		charge* C;
		size_t _size;
		CRandomMother* rnd; // <-- private pseudo-random number generator
		bool rnd_init;

	public:
		ring(size_t charges = 10, double radius = 1.0);
		ring(double radius, size_t charges = 10);
		ring(const ring& R);
		~ring();
		
		double getRadius() const;
		double& getRadius();
		double radius() const;
		double& radius();
		void setRadius(double radius);

		size_t getCharges() const;
		size_t& getCharges();
		size_t charges() const;
		size_t& charges();
		void setCharges(size_t charges);

		const charge* getCharge() const;
		const charge& getCharge(size_t index) const;
		charge* getCharge();
		charge& getCharge(size_t index);
		/*
		const charge* charge() const;
		const charge& charge(size_t index) const;
		charge* charge();
		charge& charge(size_t index);
		*/

		size_t size() const;

		void initialize(unsigned int _rand = 0);
		void rotate(double rad);

		ring& operator = (const ring& R);
};

#endif /* RING_HPP */
