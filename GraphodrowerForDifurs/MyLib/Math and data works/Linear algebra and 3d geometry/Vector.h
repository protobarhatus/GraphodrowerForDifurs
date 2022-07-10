#pragma once
#include <vector>
#include "Vector2.h"
#include "Vector3.h"
template <typename ... D>
void _fillVector(std::vector<long double>& vec, int ind_of_start, D ... args);
template<typename D>
void _fillVector(std::vector<long double>& vec, int ind_of_start, D arg);
template <typename first, typename ... others>
void _fillVector(std::vector<long double>& vec, int ind_of_start, first arg, others ... args);
template <typename ... type>
std::vector<long double> getVector(type ... args);
class Matrix;

class Vector {
	std::vector<long double> vector;
	int _size;
public:
	Vector();
	static Vector create(int size);
	template <typename ... doubl>
	Vector(doubl ... args)
	{
		this->vector = getVector(args...);
		this->_size = this->vector.size();
	}
	Vector(const Vector& vec);
	Vector(Vector&& vec) noexcept;
	Vector(const std::vector<long double>& vec);
	Vector(std::vector<long double>&& vec);
	Vector(const Vector2& vec);
	Vector(const Vector3d& vec);
	Vector& operator=(const Vector2& vec);
	Vector& operator=(const Vector3d& vec);
	Vector& operator=(const Vector& vec);
	Vector& operator=(Vector&& vec) noexcept;
	Vector& operator=(std::initializer_list<long double>&& vec);
	Vector& operator=(const std::vector<long double>& vec);
	Vector& operator=(std::vector<long double>&& vec);
	Vector null() const;
	long double& operator[](int ind);
	const long double& operator[](int ind) const;
	long double x() const;
	long double y() const;
	long double z() const;
	
	Vector operator+(const Vector& second) const;
	Vector operator-(const Vector& second) const;
	Vector operator*(long double num) const;
	Vector operator/(long double num) const;
	Vector operator*(const Matrix& second) const;

	Vector& operator+=(const Vector& second);
	Vector& operator-=(const Vector& second);
	Vector& operator*=(long double num);
	Vector& operator/=(long double num);
	
	int size() const;


};

Vector addDimension(const Vector& a);
Vector takeAwayDimension(const Vector& a);

Vector operator*(long double num, const Vector& vec);
Vector operator*(const Vector& a, const Vector& b);

long double scalar(const Vector& a, const Vector& b);
long double distance(const Vector& a, const Vector& b);
long double length(const Vector& a);
Vector normalize(const Vector& a);
long double angle(const Vector& a, const Vector& b);

template<typename D>
void _fillVector(std::vector<long double>& vec, int ind_of_start, D arg)
{
	vec[ind_of_start] = arg;
}
template <typename first, typename ... others>
void _fillVector(std::vector<long double>& vec, int ind_of_start, first arg, others ... args)
{
	vec[ind_of_start] = arg;
	_fillVector(vec, ++ind_of_start, args...);
}

template <typename ... type>
std::vector<long double> getVector(type ... args)
{
	std::vector<long double> vec(sizeof ... (args));
	_fillVector(vec, 0, args...);
	return vec;
}