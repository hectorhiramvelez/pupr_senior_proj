#pragma once

#ifndef CHARGE_S_HPP
#define CHARGE_S_HPP

/* Memory optimized version of charge */

#include "charge.hpp"

class charge_s
{
	private:
		double load;

	public:
		charge_s(double load = 0);
		charge_s(const charge_s& C);
		charge_s(const charge& C);
		~charge_s();

		double getLoad() const;
		double& getLoad();
		void setLoad(double load);

		charge_s& operator = (const charge_s& C);
		charge_s& operator = (const charge& C);
		charge_s& operator = (double load);

		operator charge() const;
};

#endif /* CHARGE_S_HPP */

