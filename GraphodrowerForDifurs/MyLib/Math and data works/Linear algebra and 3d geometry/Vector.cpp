#include "Vector.h"
#include "Matrix.h"
#include <cmath>
Vector::Vector()
{
}

Vector Vector::create(int size)
{
	Vector res;
	res.vector.resize(size);
	res._size = size;
	return res;
}

Vector::Vector(const Vector& vec)
{
	this->vector = vec.vector;
	this->_size = this->vector.size();
}
Vector::Vector(Vector&& vec) noexcept
{
	this->vector = std::move(vec.vector);
	this->_size = this->vector.size();
}
Vector::Vector(const std::vector<long double>& vec)
{
	this->vector = vec;
	this->_size = this->vector.size();
}
Vector::Vector(std::vector<long double>&& vec)
{
	this->vector = std::move(vec);
	this->_size = this->vector.size();
}

Vector::Vector(const Vector2& vec)
{
	this->vector = getVector(vec.x(), vec.y());
	this->_size = 2;
}

Vector::Vector(const Vector3d& vec)
{
	this->vector = getVector(vec.x(), vec.y(), vec.z());
	this->_size = 3;
}

Vector& Vector::operator=(const Vector2& vec)
{
	*this = Vector(vec);
	return *this;
}

Vector& Vector::operator=(const Vector3d& vec)
{
	*this = Vector(vec);
	return *this;
}

Vector& Vector::operator=(const Vector& vec)
{
	this->vector = vec.vector;
	this->_size = this->vector.size();
	return *this;
}
Vector& Vector::operator=(Vector&& vec) noexcept
{
	this->vector = std::move(vec.vector);
	this->_size = this->vector.size();
	return *this;
}
Vector& Vector::operator=(std::initializer_list<long double> && vec)
{
	this->vector = std::move(vec);
	this->_size = this->vector.size();
	return *this;
}
Vector& Vector::operator=(const std::vector<long double>& vec)
{
	this->vector = vec;
	this->_size = this->vector.size();
	return *this;
}
Vector& Vector::operator=(std::vector<long double>&& vec)
{
	this->vector = vec;
	this->_size = this->vector.size();
	return *this;
}
Vector Vector::null() const
{
	std::vector<long double> vec(this->vector.size(), 0.0);
	return vec;
}
long double& Vector::operator[](int ind)
{
	if (ind > this->_size)
		throw "Index out of borders";
	return this->vector[ind];
}
const long double& Vector::operator[](int ind) const
{
	if (ind > this->_size)
		throw "Index out of borders";
	return this->vector[ind];
}
long double Vector::x() const
{
	return this->vector[0];
}
long double Vector::y() const
{
	if (this->_size == 1)
		throw "reference to nonexistent dimension";
	return this->vector[1];
}
long double Vector::z() const
{
	if (this->_size < 3)
		throw "reference to nonexistent dimension";
	return this->vector[2];
}
Vector Vector::operator+(const Vector& second) const
{
	if (this->_size != second._size)
		throw "Addition of vectors with different sizes";
	Vector res = *this;
	for (int i = 0; i < this->_size; ++i)
		res.vector[i] += second.vector[i];
	return res;
}
Vector Vector::operator-(const Vector& second) const
{
	if (this->_size != second._size)
		throw "Substraction of vectors with different sizes";
	Vector res = *this;
	for (int i = 0; i < this->_size; ++i)
		res.vector[i] -= second.vector[i];
	return res;
}
Vector Vector::operator*(long double num) const
{
	Vector res = *this;
	for (int i = 0; i < this->_size; ++i)
		res.vector[i] *= num;
	return res;
}
Vector Vector::operator/(long double num) const
{
	Vector res = *this;
	for (int i = 0; i < this->_size; ++i)
		res.vector[i] /= num;
	return res;
}
Vector Vector::operator*(const Matrix& second) const
{
	return second * *this;
}
Vector& Vector::operator+=(const Vector& second)
{
	if (this->_size != second._size)
		throw "Addition of vectors with different sizes";
	for (int i = 0; i < this->_size; ++i)
		this->vector[i] += second.vector[i];
	return *this;
}
Vector& Vector::operator-=(const Vector& second)
{
	if (this->_size != second._size)
		throw "Substraction of vectors with different sizes";
	for (int i = 0; i < this->_size; ++i)
		this->vector[i] -= second.vector[i];
	return *this;
}
Vector& Vector::operator*=(long double num)
{
	for (int i = 0; i < this->_size; ++i)
		this->vector[i] *= num;
	return *this;
}
Vector& Vector::operator/=(long double num)
{
	for (int i = 0; i < this->_size; ++i)
		this->vector[i] /= num;
	return *this;
}
Vector addDimension(const Vector& a)
{
	Vector b = Vector::create(a.size() + 1);
	for (int i = 0; i < a.size(); ++i)
		b[i] = a[i];
	b[a.size()] = 0;
	return b;
}
Vector takeAwayDimension(const Vector& a)
{
	Vector res = Vector::create(a.size() - 1) ;
	for (int i = 0; i < a.size() - 1; ++i)
		res[i] = a[i];
	return res;
}
Vector operator*(long double num, const Vector& vec)
{
	return vec * num;
}
int Vector::size() const
{
	return this->_size;
}
long double scalar(const Vector& a, const Vector& b)
{
	if (a.size() != b.size())
		throw "Scalar multiplication of different dimension vectors";
	long double res = 0;
	for (int i = 0; i < a.size(); ++i)
		res += a[i] * b[i];
	return res;
}

long double distance(const Vector& a, const Vector& b)
{
	if (a.size() != b.size())
		throw "Cannot count distance of vectors with different size";
	long double res = 0;
	for (int i = 0; i < a.size(); ++i)
		res += (a[i] - b[i]) * (a[i] - b[i]);
	return sqrt(res);
}

long double length(const Vector& a)
{
	long double l = 0;
	for (int i = 0; i < a.size(); ++i)
		l += a[i] * a[i];
	return sqrt(l);
}

Vector normalize(const Vector& a)
{
	return a / length(a);
}
long double angle(const Vector& a, const Vector& b)
{
	return acos(scalar(a, b) / length(a) / length(b));
}
Vector operator*(const Vector& a, const Vector& b)
{
	if (a.size() != 3 || b.size() != 3)
		throw "Cannot multiply non 3 dimensional vectors";
	return Vector(a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(), a.x() * b.y() - a.y() * b.x());
}