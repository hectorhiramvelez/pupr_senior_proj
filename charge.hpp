#pragma once

#ifndef CHARGE_HPP
#define CHARGE_HPP

#include "vector3.hpp"

class charge : public vector3<double>
{
	private:
		double load;

	public:
		charge(double load = 0);
		charge(const vector3<double> V, double load = 0);
		charge(double X, double Y, double Z, double load = 0);
		charge(const charge& C);
		~charge();

		double getLoad() const;
		double& getLoad();
		void setLoad(double load);

		charge& operator = (const charge& C);
		charge& operator = (double load);
};

#endif /* CHARGE_HPP */

