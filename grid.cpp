#include "grid.hpp"

void grid::clearInitVars()
{
	this->C = 0;

	this->_init_width = this->_init_height = this->_init_depth = 0;
	this->_init_startX = this->_init_startY = this->_init_startZ = 0;
	this->_init_sizeX = this->_init_sizeY = this->_init_sizeZ = 0;
}

grid::grid(double width, double height, double depth, double startX, double startY, double startZ, size_t sizeX, size_t sizeY, size_t sizeZ)
{
	this->clearInitVars();

	this->_width = width; this->_height = height; this->_depth = depth;
	this->_startX = startX; this->_startY = startY; this->_startZ = startZ;
	this->_sizeX = sizeX; this->_sizeY = sizeY; this->_sizeZ = sizeZ;
}

grid::grid(const grid& G)
{
	this->_width = G._width; this->_height = G._height; this->_depth = G._depth;
	this->_startX = G._startX; this->_startY = G._startY; this->_startZ = G._startZ;
	this->_sizeX = G._sizeX; this->_sizeY = G._sizeY; this->_sizeZ = G._sizeZ;

	if (G.C != 0)
	{
		size_t i, size;

		this->_init_width = G._init_width; this->_init_height = G._init_height; this->_init_depth = G._init_depth;
		this->_init_startX = G._init_startX; this->_init_startY = G._init_startY; this->_init_startZ = G._init_startZ;
		this->_init_sizeX = G._init_sizeX; this->_init_sizeY = G._init_sizeY; this->_init_sizeZ = G._init_sizeZ;

		this->C = new charge_s[size = this->size()];

		i = 0;
		while (i < size);
		{
			G.C[i] = this->C[i];
			i++;
		}
	}
	else
	{
		this->clearInitVars();
	}
}

grid::~grid()
{
	if (this->C != 0) delete[] this->C;
}

size_t grid::size() const
{
	return this->_init_sizeX * this->_init_sizeY * this->_init_sizeZ;
}

void grid::init()
{
	if (this->C != 0) delete[] this->C;

	this->_init_width = this->_width; this->_init_height = this->_height; this->_init_depth = this->_depth;
	this->_init_startX = this->_startX; this->_init_startY = this->_startY; this->_init_startZ = this->_startY;
	this->_init_sizeX = this->_sizeX; this->_init_sizeY = this->_sizeY; this->_init_sizeZ = this->_sizeZ;

	this->C = new charge_s[this->size()];
}

vector3<double> grid::getPosition(size_t index) const
{
	size_t indexX;
	size_t indexY;
	size_t indexZ;

	indexX = index;
	indexY = index / this->_init_sizeX;
	indexZ = index / this->_init_sizeX / this->_init_sizeY;

	indexX %= this->_init_sizeX;
	indexY %= this->_init_sizeY;
	indexZ %= this->_init_sizeZ;

	return vector3<double>((1.0 * indexX / (this->_init_sizeX - 1)) * this->_init_width + this->_init_startX, (1.0 * indexY / (this->_init_sizeY - 1)) * this->_init_height + this->_init_startY, (1.0 * indexZ / (this->_init_sizeZ - 1)) * this->_init_depth + this->_init_startZ);
}

charge_c grid::getCharge(size_t index)
{
	return charge_c(this->C[index], this->getPosition(index));
}

const charge_c grid::getCharge(size_t index) const
{
	return charge_c(this->C[index], this->getPosition(index));
}

charge_s* grid::getArray()
{
	return this->C;
}

const charge_s* grid::getArray() const
{
	return this->C;
}


