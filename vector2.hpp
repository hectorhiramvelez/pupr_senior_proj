#pragma once

#ifndef VECTOR2_HPP
#define VECTOR2_HPP

template<typename dtype>
class vector2
{
	private:
		dtype x;
		dtype y;

	public:
		vector2();
		vector2(const dtype& X, const dtype& Y);
		vector2(const vector2<dtype>& V);
		~vector2();

		const dtype& getX() const;
		dtype& getX();
		const dtype& getY() const;
		dtype& getY();

		void setX(const dtype& X);
		void setY(const dtype& Y);

		dtype magnitude() const;
		vector2<dtype> difference(const vector2<dtype>& V) const;
		vector2<dtype> difference(const dtype& X, const dtype& Y) const;
		vector2<dtype> rotate(const dtype& R) const;
		vector2<dtype> unit() const;
		vector2<dtype> inverse() const;
		dtype dot(const vector2<dtype>& V) const;
		dtype dot(const dtype& X, const dtype& Y) const;
		dtype dotProduct(const vector2<dtype>& V) const;
		dtype dotProduct(const dtype& X, const dtype& Y) const;

		vector2<dtype> operator + (const vector2<dtype>& V) const;
		vector2<dtype> operator - (const vector2<dtype>& V) const;
		dtype operator * (const vector2<dtype>& V) const;
		vector2<dtype> operator * (const dtype& D) const;
		vector2<dtype> operator / (const dtype& D) const;

		vector2<dtype>& operator += (const vector2<dtype>& V);
		vector2<dtype>& operator -= (const vector2<dtype>& V);
		vector2<dtype>& operator *= (const dtype& D);
		vector2<dtype>& operator /= (const dtype& D);

		vector2<dtype> operator - () const;

		vector2<dtype>& operator = (const vector2<dtype>& V);

		bool operator < (const vector2<dtype>& V) const;
		bool operator < (const dtype& D) const;
		bool operator <= (const vector2<dtype>& V) const;
		bool operator <= (const dtype& D) const;
		bool operator == (const vector2<dtype>& V) const;
		bool operator == (const dtype& D) const;
		bool operator >= (const vector2<dtype>& V) const;
		bool operator >= (const dtype& D) const;
		bool operator > (const vector2<dtype>& V) const;
		bool operator > (const dtype& D) const;
		bool operator != (const vector2<dtype>& V) const;
		bool operator != (const dtype& D) const;
};

template<typename dtype>
vector2<dtype> operator * (const dtype& D, const vector2<dtype>& V);

template<typename dtype>
bool operator < (const dtype& D, const vector2<dtype>& V);

template<typename dtype>
bool operator <= (const dtype& D, const vector2<dtype>& V);

template<typename dtype>
bool operator == (const dtype& D, const vector2<dtype>& V);

template<typename dtype>
bool operator >= (const dtype& D, const vector2<dtype>& V);

template<typename dtype>
bool operator > (const dtype& D, const vector2<dtype>& V);

template<typename dtype>
bool operator != (const dtype& D, const vector2<dtype>& V);

#include "vector2.hpp.inline"

#endif
