#include "Plane.h"
#include "geometry 3d.h"
#include <assert.h>
#include <cmath>
#include <vector>
Plane::Plane()
{
}


Plane::~Plane()
{
}

Plane::Plane(double a, double b, double c, double d, bool make_units_vector) : _a(a), _b(b), _c(c), _d(d)
{
	if (make_units_vector)
		this->makeOwnUnitsVectors();
}
Vector3d Plane::normal() const
{
	return Vector3d(_a, _b, _c);
}
double Plane::a() const
{
	return this->_a;
}
double Plane::b() const
{
	return this->_b;
}
double Plane::c() const
{
	return this->_c;
}
double Plane::d() const
{
	return this->_d;
}
void Plane::setA(double a)
{
	this->_a = a;
	this->has_counted_units_vectors = false;
}
void Plane::setB(double b)
{
	this->_b = b;
	this->has_counted_units_vectors = false;
}
void Plane::setC(double c)
{
	this->_c = c;
	this->has_counted_units_vectors = false;
}
void Plane::setD(double d)
{
	this->_d = d;
	this->has_counted_units_vectors = false;
}
double Plane::z(double x, double y)
{
	assert(this->_c == 0);
	return (-_d - _a * x - _b * y) / _c;
}
double Plane::y(double x, double z)
{
	assert(this->_b == 0);
	return (-_d - _a * x - _c * z) / _b;
}
double Plane::x(double y, double z)
{
	assert(this->_a == 0);
	return (-_d - _b * y - _c * z) / _a;
}
void Plane::makeOwnUnitsVectors()
{
	if (has_counted_units_vectors)
		return;
	Vector3d zero = getPointOfNormalToPlane(Vector3d(0, 0, 0), *this);

	Vector3d one = getPointOfNormalToPlane(Vector3d(1, 1, 1), *this);
	
	if (one == zero)
		one = getPointOfNormalToPlane(Vector3d(0, 0, 1), *this);
	one += (one * ((sqrt(2) - (one - zero).length()) / one.length()));
	Vector3d middle = (zero + one) * 0.5;

	Line3d another_diagonal = this->getIntersection(getPlaneThroughPointPerpendicularToVector(middle, one - zero));
	Vector3d sq_diagonal = another_diagonal.getBaseVector() * (sqrt(2.) / 2);

	this->abscissa_unit_vector = middle - sq_diagonal - zero;
	this->ordinata_unit_vector = middle + sq_diagonal - zero;

	if (this->abscissa_unit_vector.length() != 1.0)
	{
		abscissa_unit_vector *= (1. / abscissa_unit_vector.length());
		ordinata_unit_vector *= (1. / ordinata_unit_vector.length());
	}

	this->abscissa_axis = Line3d(zero, zero + this->abscissa_unit_vector);
	this->ordinata_axis = Line3d(zero, zero + this->ordinata_unit_vector);

	has_counted_units_vectors = true;
}

Line3d Plane::getIntersection(const Plane & plane)
{
	/*//plane не совпадает и не параллельно, иначе херню выдаст
	double a = this->_a, b = this->_b, c = this->_c;
	double d = plane._a, e = plane._b, f = plane._c;
	auto Power = [](double x, int d) { return pow(x, d); };
	//следующий код был скопирован с вольфрам математика, анализу не подлежит
	std::vector<std::vector<double>> inverse = { {(d*(-(a*d) - b * e - c * f)) /
		(Power(b, 2)*Power(d, 2) + Power(c, 2)*Power(d, 2) - 2 * a*b*d*e + Power(a, 2)*Power(e, 2) +
			Power(c, 2)*Power(e, 2) - 2 * a*c*d*f - 2 * b*c*e*f + Power(a, 2)*Power(f, 2) +
			Power(b, 2)*Power(f, 2)) + (a*(Power(d, 2) + Power(e, 2) + Power(f, 2))) /
			(Power(b, 2)*Power(d, 2) + Power(c, 2)*Power(d, 2) - 2 * a*b*d*e + Power(a, 2)*Power(e, 2) +
				Power(c, 2)*Power(e, 2) - 2 * a*c*d*f - 2 * b*c*e*f + Power(a, 2)*Power(f, 2) +
				Power(b, 2)*Power(f, 2)), ((Power(a, 2) + Power(b, 2) + Power(c, 2))*d) /
				(Power(b, 2)*Power(d, 2) + Power(c, 2)*Power(d, 2) - 2 * a*b*d*e + Power(a, 2)*Power(e, 2) +
					Power(c, 2)*Power(e, 2) - 2 * a*c*d*f - 2 * b*c*e*f + Power(a, 2)*Power(f, 2) +
					Power(b, 2)*Power(f, 2)) + (a*(-(a*d) - b * e - c * f)) /
					(Power(b, 2)*Power(d, 2) + Power(c, 2)*Power(d, 2) - 2 * a*b*d*e + Power(a, 2)*Power(e, 2) +
						Power(c, 2)*Power(e, 2) - 2 * a*c*d*f - 2 * b*c*e*f + Power(a, 2)*Power(f, 2) +
						Power(b, 2)*Power(f, 2))}, {(e*(-(a*d) - b * e - c * f)) /
						(Power(b, 2)*Power(d, 2) + Power(c, 2)*Power(d, 2) - 2 * a*b*d*e + Power(a, 2)*Power(e, 2) +
							Power(c, 2)*Power(e, 2) - 2 * a*c*d*f - 2 * b*c*e*f + Power(a, 2)*Power(f, 2) +
							Power(b, 2)*Power(f, 2)) + (b*(Power(d, 2) + Power(e, 2) + Power(f, 2))) /
							(Power(b, 2)*Power(d, 2) + Power(c, 2)*Power(d, 2) - 2 * a*b*d*e + Power(a, 2)*Power(e, 2) +
								Power(c, 2)*Power(e, 2) - 2 * a*c*d*f - 2 * b*c*e*f + Power(a, 2)*Power(f, 2) +
								Power(b, 2)*Power(f, 2)), ((Power(a, 2) + Power(b, 2) + Power(c, 2))*e) /
								(Power(b, 2)*Power(d, 2) + Power(c, 2)*Power(d, 2) - 2 * a*b*d*e + Power(a, 2)*Power(e, 2) +
									Power(c, 2)*Power(e, 2) - 2 * a*c*d*f - 2 * b*c*e*f + Power(a, 2)*Power(f, 2) +
									Power(b, 2)*Power(f, 2)) + (b*(-(a*d) - b * e - c * f)) /
									(Power(b, 2)*Power(d, 2) + Power(c, 2)*Power(d, 2) - 2 * a*b*d*e + Power(a, 2)*Power(e, 2) +
										Power(c, 2)*Power(e, 2) - 2 * a*c*d*f - 2 * b*c*e*f + Power(a, 2)*Power(f, 2) +
										Power(b, 2)*Power(f, 2)) }, {(f*(-(a*d) - b * e - c * f)) /
										(Power(b, 2)*Power(d, 2) + Power(c, 2)*Power(d, 2) - 2 * a*b*d*e + Power(a, 2)*Power(e, 2) +
											Power(c, 2)*Power(e, 2) - 2 * a*c*d*f - 2 * b*c*e*f + Power(a, 2)*Power(f, 2) +
											Power(b, 2)*Power(f, 2)) + (c*(Power(d, 2) + Power(e, 2) + Power(f, 2))) /
											(Power(b, 2)*Power(d, 2) + Power(c, 2)*Power(d, 2) - 2 * a*b*d*e + Power(a, 2)*Power(e, 2) +
												Power(c, 2)*Power(e, 2) - 2 * a*c*d*f - 2 * b*c*e*f + Power(a, 2)*Power(f, 2) +
												Power(b, 2)*Power(f, 2)), ((Power(a, 2) + Power(b, 2) + Power(c, 2))*f) /
												(Power(b, 2)*Power(d, 2) + Power(c, 2)*Power(d, 2) - 2 * a*b*d*e + Power(a, 2)*Power(e, 2) +
													Power(c, 2)*Power(e, 2) - 2 * a*c*d*f - 2 * b*c*e*f + Power(a, 2)*Power(f, 2) +
													Power(b, 2)*Power(f, 2)) + (c*(-(a*d) - b * e - c * f)) /
													(Power(b, 2)*Power(d, 2) + Power(c, 2)*Power(d, 2) - 2 * a*b*d*e + Power(a, 2)*Power(e, 2) +
														Power(c, 2)*Power(e, 2) - 2 * a*c*d*f - 2 * b*c*e*f + Power(a, 2)*Power(f, 2) +
														Power(b, 2)*Power(f, 2))} };
	//a-1b
	std::vector<double> ind_part = { -this->_d, -plane._d };

	std::vector<double> mult_res(3, 1);
	for (int i = 0; i < 3; ++i)
	{
		double sum = 0;
		for (int j = 0; j < 2; ++j)
			sum += inverse[i][j] * ind_part[j];
		mult_res[i] = sum;
	}*/
	/*double a = this->_a, b = this->_b, c = this->_c, d = this->_d;
	double e = plane._a, f = plane._b, g = plane._c, h = plane._d;

	if (this->_c == 0 && plane._c == 0)
	{
		double x = (d * f - b * h) / (b * e - a * f);
		double y = (d * e - a * h) / (a * f - b * e);

		return Line3d(Vector3d(x, y, 0), Vector3d(x, y, 1));
	}
	if (b * e == a * f)
	{
		
	}
	auto x = [&a, &b, &c, &d, &e, &f, &g, &h](double z) { return(-b * g *z - b * h + c * f *z + d * f) / (b *e - a * f); };
	auto y = [&a, &b, &c, &d, &e, &f, &g, &h](double z) { return(-a*(g *z + h) + c *e *z + d *e) / (a *f - b *e); };
	return Line3d(Vector3d(x(0), y(0), 0), Vector3d(x(1), y(1), 1));*/

	//это методом гаусса. плоскости не совпадают и не параллельны
	std::vector<std::vector<double>> extended_equation_mattrix = 
	{
		{ this->_a, this->_b, this->_c, -this->_d},
		{ plane._a, plane._b, plane._c, -plane._d}
	};
	auto get_index_of_first_non_zero = [](std::vector<double> & vec)->int {
		for (int i = 0; i < vec.size(); ++i)
			if (vec[i] != 0)
				return i;
		return vec.size();
	};
	int ind1 = get_index_of_first_non_zero(extended_equation_mattrix[0]);
	int ind2 = get_index_of_first_non_zero(extended_equation_mattrix[1]);
	if (ind1 > ind2)
	{
		std::swap(extended_equation_mattrix[0], extended_equation_mattrix[1]);
		std::swap(ind1, ind2);
	}
	if (ind1 == ind2)
	{
		double multiplier = -extended_equation_mattrix[1][ind2] / extended_equation_mattrix[0][ind1];
		extended_equation_mattrix[1][ind2] = 0;
		++ind2;
		for (int i = ind2; i < extended_equation_mattrix[1].size(); ++i)
			extended_equation_mattrix[1][i] += (extended_equation_mattrix[0][i] * multiplier);
		ind2 = get_index_of_first_non_zero(extended_equation_mattrix[1]);
	}
	double multiplier = -extended_equation_mattrix[0][ind2] / extended_equation_mattrix[1][ind2];
	
	for (int i = ind2; i < extended_equation_mattrix[0].size(); ++i)
		extended_equation_mattrix[0][i] += (extended_equation_mattrix[1][i] * multiplier);
	ind1 = get_index_of_first_non_zero(extended_equation_mattrix[0]);
	auto div_on_lead_element = [](std::vector<double>& vec)->void {
		long double lead_el = 0;
		for (int i = 0; i < vec.size(); ++i)
		{
			if (lead_el == 0)
				lead_el = vec[i];
			if (lead_el != 0)
				vec[i] /= lead_el;
		}
	};
	div_on_lead_element(extended_equation_mattrix[0]);
	div_on_lead_element(extended_equation_mattrix[1]);
	assert(ind1 != ind2 && ind1 < ind2 && extended_equation_mattrix[0][ind1] == 1 && extended_equation_mattrix[1][ind2] == 1 &&
		extended_equation_mattrix[0][ind2] == 0 && extended_equation_mattrix[1][ind1] == 0 && ind1 >= 0 && ind1 <= 2 && ind2 >= 0 && ind2 <= 2);
	
	std::vector<double> res0(3);
	std::vector<double> rest(3);
	res0[3 - ind1 - ind2] = 0;
	rest[3 - ind1 - ind2] = 1;
	res0[ind1] = extended_equation_mattrix[0][3];
	rest[ind1] = -extended_equation_mattrix[0][3 - ind1 - ind2];
	res0[ind2] = extended_equation_mattrix[1][3];
	rest[ind2] = -extended_equation_mattrix[1][3 - ind1 - ind2];
	//ставя минус перед res0 мы получаем результат решения уравнения. без минуса - точку
	Vector3d point_on_line(res0[0], res0[1], res0[2]);
	Vector3d base_vector(rest[0], rest[1], rest[2]);
	return Line3d(point_on_line, point_on_line + base_vector);
}
Vector2 Plane::getCoordinatesInPlanesUnitsVector(const Vector3d & point) const
{
	assert(this->has_counted_units_vectors);
	Vector2 preresult = Vector2(distance(point, this->ordinata_axis), distance(point, this->abscissa_axis));
	if (point == this->abscissa_unit_vector * preresult.x() + this->ordinata_unit_vector * preresult.y())
		return preresult;
	if (point == this->abscissa_unit_vector * (-preresult.x()) + this->ordinata_unit_vector * preresult.y())
		return Vector2(-preresult.x(), preresult.y());
	if (point == this->abscissa_unit_vector * preresult.x() + this->ordinata_unit_vector * (-preresult.y()))
		return Vector2(preresult.x(), -preresult.y());
	return preresult * -1;
}

Plane getPlaneThroughPointAndTwoVectors(const Vector3d& point, const Vector3d& vector1, const Vector3d& vector2)
{
	double D1 = (vector1.y() * vector2.z() - vector1.z() * vector2.y());
	double D2 = (vector1.x() * vector2.z() - vector1.z() * vector2.x());
	double D3 = (vector1.x() * vector2.y() - vector1.y() * vector2.x());

	return Plane(D1, -D2, D3, -point.x() * D1 + point.y() * D2 - point.z() * D3);
}
