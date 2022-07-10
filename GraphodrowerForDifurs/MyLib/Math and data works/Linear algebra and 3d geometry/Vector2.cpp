#include "Vector2.h"
#include <cmath>
#include "Vector.h"

Vector2::Vector2() : _x(0), _y(0)
{
}


Vector2::~Vector2()
{
}
Vector2::Vector2(long double ix, long double iy) : _x(ix), _y(iy)
{

}
Vector2::Vector2(const Vector2 & copy) : _x(copy._x), _y(copy._y)
{

}
Vector2::Vector2(const Vector& vec)
{
	if (vec.size() != 2)
		throw "Conversion of different sized vector";
	this->_x = vec.x();
	this->_y = vec.y();
}
Vector2& Vector2::operator=(const Vector& vec)
{
	*this = Vector2(vec);
	return *this;
}
long double Vector2::x() const
{
	return this->_x;
}
long double Vector2::y() const
{
	return this->_y;
}
Vector2 Vector2::operator+(Vector2 b) const
{
	Vector2 result(*this);
	result._x += b._x;
	result._y += b._y;
	return result;
}
Vector2 Vector2::operator-(Vector2 b) const
{
	Vector2 result(*this);
	result._x -= b._x;
	result._y -= b._y;
	return result;
}
Vector2 Vector2::operator*(long double val) const
{
	return Vector2(this->_x * val, this->_y * val);
}
Vector2& Vector2::operator+=(Vector2 b)
{
	this->_x += b._x;
	this->_y += b._y;
	return *this;
}
Vector2& Vector2::operator-=(Vector2 b)
{
	this->_x -= b._x;
	this->_y -= b._y;
	return *this;
}
Vector2& Vector2::operator*=(long double val)
{
	this->_x *= val;
	this->_y *= val;
	return *this;
}
long double mind(long double a, long double b)
{
	return a < b ? a : b;
}
long double maxd(long double a, long double b)
{
	return a > b ? a : b;
}
Vector2 min(const Vector2 & a, const Vector2 & b)
{
	return Vector2(mind(a.x(), b.x()), mind(a.y(), b.y()));
}
Vector2 max(const Vector2 & a, const Vector2 & b)
{
	return Vector2(maxd(a.x(), b.x()), maxd(a.y(), b.y()));
}
long double Vector2::length() const
{
	return sqrt(this->_x * this->_x + this->_y * this->_y);
}
Vector2 operator*(long double val, const Vector2 & vec)
{
	return vec * val;
}
Vector2& Vector2::operator=(int val)
{
	this->_x = val;
	this->_y = val;
	return *this;
}
Vector2 Vector2::create(int val)
{
	return Vector2(val, val);
	
}
