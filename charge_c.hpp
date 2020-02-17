#pragma once

/* Charge container special class (for grid) */

#include "charge_s.hpp"

#ifndef CHARGE_C_HPP
#define CHARGE_C_HPP

class charge_c : public vector3<double>
{
	private:
		charge_s* C;

	public:
		charge_c(charge_s* C = 0, vector3<double> V = vector3<double>());
		charge_c(charge_s& C, vector3<double> V = vector3<double>());
		charge_c(const charge_c& C);
		~charge_c();

		double getLoad() const;
		double& getLoad();
		void setLoad(double load);

		void setLink(charge_s* C);

		charge_c& operator = (const charge_c& C);
		charge_c& operator = (charge_s& C);
		charge_c& operator = (double load);

		operator charge() const;
};


#endif /* CHARGE_C_HPP */
