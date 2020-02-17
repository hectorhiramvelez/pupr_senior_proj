#pragma once

#ifndef VECTOR3_HPP
#define VECTOR3_HPP

template<typename dtype>
class vector3
{
	private:
		dtype x;
		dtype y;
		dtype z;

	public:
		vector3();
		vector3(const dtype& X, const dtype& Y, const dtype& Z);
		vector3(const vector3<dtype>& V);
		~vector3();

		const dtype& getX() const;
		dtype& getX();
		const dtype& getY() const;
		dtype& getY();
		const dtype& getZ() const;
		dtype& getZ();

		void setX(const dtype& X);
		void setY(const dtype& Y);
		void setZ(const dtype& Z);

		dtype magnitude() const;
		vector3<dtype> difference(const vector3<dtype>& V) const;
		vector3<dtype> difference(const dtype& X, const dtype& Y, const dtype& Z) const;
		vector3<dtype> rotate(const dtype& rotX, const dtype& rotY, const dtype& rotZ) const;
		vector3<dtype> unit() const;
		vector3<dtype> inverse() const;
		dtype dot(const vector3<dtype>& V) const;
		dtype dot(const dtype& X, const dtype& Y, const dtype& Z) const;
		dtype dotProduct(const vector3<dtype>& V) const;
		dtype dotProduct(const dtype& X, const dtype& Y, const dtype& Z) const;
		vector3<dtype> cross(const vector3<dtype>& V) const;
		vector3<dtype> cross(const dtype& X, const dtype& Y, const dtype& Z) const;
		vector3<dtype> crossProduct(const vector3<dtype>& V) const;
		vector3<dtype> crossProduct(const dtype& X, const dtype& Y, const dtype& Z) const;

		vector3<dtype> operator + (const vector3<dtype>& V) const;
		vector3<dtype> operator - (const vector3<dtype>& V) const;
		dtype operator * (const vector3<dtype>& V) const;
		vector3<dtype> operator / (const vector3<dtype>& V) const;
		vector3<dtype> operator * (const dtype& D) const;
		vector3<dtype> operator / (const dtype& D) const;

		vector3<dtype>& operator += (const vector3<dtype>& V);
		vector3<dtype>& operator -= (const vector3<dtype>& V);
		vector3<dtype>& operator /= (const vector3<dtype>& V);
		vector3<dtype>& operator *= (const dtype& D);
		vector3<dtype>& operator /= (const dtype& D);

		vector3<dtype> operator - () const;

		vector3<dtype>& operator = (const vector3<dtype>& V);

		bool operator < (const vector3<dtype>& V) const;
		bool operator < (const dtype& D) const;
		bool operator <= (const vector3<dtype>& V) const;
		bool operator <= (const dtype& D) const;
		bool operator == (const vector3<dtype>& V) const;
		bool operator == (const dtype& D) const;
		bool operator >= (const vector3<dtype>& V) const;
		bool operator >= (const dtype& D) const;
		bool operator > (const vector3<dtype>& V) const;
		bool operator > (const dtype& D) const;
		bool operator != (const vector3<dtype>& V) const;
		bool operator != (const dtype& D) const;
};

template<typename dtype>
vector3<dtype> operator * (const dtype& D, const vector3<dtype>& V);

template<typename dtype>
bool operator < (const dtype& D, const vector3<dtype>& V);

template<typename dtype>
bool operator <= (const dtype& D, const vector3<dtype>& V);

template<typename dtype>
bool operator == (const dtype& D, const vector3<dtype>& V);

template<typename dtype>
bool operator >= (const dtype& D, const vector3<dtype>& V);

template<typename dtype>
bool operator > (const dtype& D, const vector3<dtype>& V);

template<typename dtype>
bool operator != (const dtype& D, const vector3<dtype>& V);

#include "vector3.hpp.inline"

#endif /* VECTOR3_HPP */
