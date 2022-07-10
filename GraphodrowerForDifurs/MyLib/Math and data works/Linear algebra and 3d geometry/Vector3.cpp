#include "Vector3.h"
#include <cmath>
#include "Vector.h"
Vector3d::Vector3d(long double x, long double y, long double z) : _x(x), _y(y), _z(z)
{

}

Vector3d::Vector3d()
{
}

Vector3d::~Vector3d()
{
}

Vector3d::Vector3d(const Vector3d & copy)
{
	this->_x = copy._x;
	this->_y = copy._y;
	this->_z = copy._z;
}

Vector3d::Vector3d(const Vector& vec)
{
	if (vec.size() != 3)
		throw "Conversion of vector with different size";
	this->_x = vec.x();
	this->_y = vec.y();
	this->_z = vec.z();
}

Vector3d& Vector3d::operator=(const Vector& vec)
{
	*this = Vector3d(vec);
	return *this;
}

long double Vector3d::x() const
{
	return this->_x;
}

long double Vector3d::y() const
{
	return this->_y;
}

long double Vector3d::z() const
{
	return this->_z;
}

Vector3d Vector3d::operator+(const Vector3d &  b) const
{
	return Vector3d(this->_x + b._x, this->_y + b._y, this->_z + b._z);
}

Vector3d Vector3d::operator-(const Vector3d &  b) const
{
	return Vector3d(this->_x - b._x, this->_y - b._y, this->_z - b._z);
}

Vector3d Vector3d::operator*(long double val) const
{
	return Vector3d(this->_x * val, this->_y * val, this->_z * val);
}
Vector3d Vector3d::operator*(const Vector3d & b) const
{
	return Vector3d(this->_y * b._z - this->_z * b._y, this->_z * b._x - this->_x * b._z, this->_x * b._y - this->_y * b._x);
}
Vector3d & Vector3d::operator+=(const Vector3d &  b)
{
	this->_x += b._x;
	this->_y += b._y;
	this->_z += b._z;
	return *this;
}

Vector3d & Vector3d::operator-=(const Vector3d & b)
{
	this->_x -= b._x;
	this->_y -= b._y;
	this->_z -= b._z;
	return *this;
}

Vector3d & Vector3d::operator*=(long double val)
{
	this->_x *= val;
	this->_y *= val;
	this->_z *= val;
	return *this;
}
Vector3d & Vector3d::operator*=(const Vector3d & b)
{
	*this = *this * b;
	return *this;
}
long double Vector3d::length() const
{
	return sqrt(this->_x * this->_x + this->_y * this->_y + this->_z * this->_z);
}
bool Vector3d::operator==(const Vector3d & b) const
{
	auto eq = [](long double a, long double b) { return abs(a - b) <= 0.01*abs(a + b); };
	return eq(this->_x, b._x) && eq(this->_y, b._y) && eq(this->_z, b._z);
}

bool scalar(const Vector3d& a, const Vector3d& b)
{
	return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}
