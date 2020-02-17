#pragma once

#ifndef GRID_HPP
#define GRID_HPP

#include "charge_c.hpp"

#include <cstdlib>

class grid
{
	private:
		double _width; /* X */
		double _height; /* Y */
		double _depth; /* Z */

		double _startX;
		double _startY;
		double _startZ;

		size_t _sizeX;
		size_t _sizeY;
		size_t _sizeZ;

		charge_s* C;

		double _init_width;
		double _init_height;
		double _init_depth;

		double _init_startX;
		double _init_startY;
		double _init_startZ;

		size_t _init_sizeX;
		size_t _init_sizeY;
		size_t _init_sizeZ;

		void clearInitVars();

	public:
		grid(double width = 2, double height = 2, double depth = 2, double startX = -1, double startY = -1, double startZ = -1, size_t sizeX = 100, size_t sizeY = 100, size_t sizeZ = 100);
		grid(const grid& G);
		~grid();

		size_t size() const;

		void init();

		vector3<double> getPosition(size_t index) const;
		const charge_c getCharge(size_t index) const;
		charge_c getCharge(size_t index);

		const charge_s* getArray() const;
		charge_s* getArray();
};

#endif /* GRID_HPP */
