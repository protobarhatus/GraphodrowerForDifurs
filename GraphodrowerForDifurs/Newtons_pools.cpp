#include "Plot.h"
#include "SFML\Graphics.hpp"
#include <cmath>
#include <iostream>
#include "Plot3D.h"
#include "ButchersTable.h"
#include "MyLib/MyLib.h"
#include <complex>
#include <iomanip>
#include <iostream>
#include "Newtons_pools.h"
#include "Dormand_prince.h"

Vector pow(Vector a, int p)
{
	using namespace std::complex_literals;

	using namespace std::complex_literals;
	std::complex<long double> c = a.x() + 1il * a.y();
	c = pow(c, p);
	return Vector(c.real(), c.imag());
}


long double newtonMethodForEquations(const std::function<long double(long double)>& function, const std::function<long double(long double)>& derivative, long double eps)
{
	long double x0;
	long double x_n = 2;
	do
	{
		x0 = x_n;
		x_n = x0 - function(x0) / derivative(x0);
	} while (abs(x0 - x_n) > eps);
	return x_n;
}
int newtonMethodForEquationsWithCountingSteps(const std::function<Vector(Vector)>& function, const std::function<Matrix(Vector)>& derivative, long double eps, int size)
{
	int counter = 0;
	Vector x0;
	Vector x_n = Vector::create(size);
	do
	{
		++counter;
		x0 = x_n;
		x_n = x0 - function(x0) * inverse(derivative(x0));
	} while (distance(x0, x_n) > eps);
	return counter;
}
int newtonMethodForEquationsWithCountingSteps(const std::function<Vector(Vector)>& function, const std::function<Matrix(Vector)>& derivative, long double eps, Vector start)
{
	int counter = 0;
	Vector x0;
	Vector x_n = start;
	do
	{
		++counter;
		x0 = x_n;
		x_n = x0 - function(x0) * inverse(derivative(x0));
	} while (distance(x0, x_n) > eps);
	return counter;
}
//для уравнения вида y'' = f(x, y, y')
//y' = u, y'' = u'
//dU_dx - производная от du_dx
long double findBorderByNewtonMethod(const std::function<long double(const Vector3d&)>& du_dx, long double y_0, long double y_l, long double l, long double tol,
	const std::function<long double(const Vector3d&)>& dU_dx)
{
	long double s = 2;
	long double ls = s;
	do
	{
		ls = s;
		long double phi = integrateFirstOrderSystem([&du_dx](long double t, const Vector2& p) {return Vector2(p.y(), du_dx(Vector3d(t, p.x(), p.y()))); },
			tol, 0, l, l / 100., Vector2(y_0, s), [](long double, const Vector&) {})[0] - y_l;
		long double dphi_ds = integrateFirstOrderSystem([&dU_dx](long double t, const Vector2& p) {return Vector2(p.y(), dU_dx(Vector3d(t, p.x(), p.y()))); },
			tol, 0, l, l / 100., Vector(0, 1), [](long double t, const Vector&) {})[0];
		/*long double ds = 0.00000001;
		long double phi_d = integrateFirstOrderSystem([&du_dx](long double t, const Vector2& p) {return Vector2(p.y(), du_dx(Vector3d(t, p.x(), p.y()))); },
			tol, 0, l, l / 100., Vector2(y_0, s + ds), [](long double, const Vector&) {})[0] - y_l;
		long double dphi_ds = (phi_d - phi) / ds;*/

		s = s - phi / dphi_ds;

	} while (abs(ls - s) >= 0.0001);
	return s;
}
long double newtonMethodIntegrate(const std::function<long double(const Vector3d&)>& du_dx, long double y_0, long double y_l, long double l, long double tol,
	double start, double finish, double step,
	const std::function<long double(const Vector3d&)>& dU_dx, const std::function<void(long double, const Vector2&)>& setPoint)
{
	long double s = findBorderByNewtonMethod(du_dx, y_0, y_l, l, tol, dU_dx);
	return integrateFirstOrderSystem([&du_dx](long double t, const Vector2& p) { return Vector2(du_dx(Vector3d(t, p.y(), p.x())), p.x()); }, tol, start, finish, step,
		Vector(s, y_0), setPoint)[1];
	return integrateFirstOrderSystem([&du_dx](long double t, const Vector2& p) { return Vector2(p.y(), du_dx(Vector3d(t, p.x(), p.y()))); }, tol, start, finish, step,
		Vector(y_0, s), setPoint)[0];
}

/*auto func = [](Vector x) {return pow(x, 5) - Vector(1, 0); };
[&func](Vector x) {
	Matrix result(2, 2);
	result[0][0] = 5 * pow(x.x(), 4) - 30 * pow(x.x(), 2) * pow(x.y(), 2) + 5 * pow(x.y(), 4);
	result[1][0] = 20 * pow(x.x(), 3) * x.y() - 20 * x.x() * pow(x.y(), 3);
	result[0][1] = -result[1][0];
	result[1][1] = result[0][0];
	return result;
}*/
DrImage newtonsPools(const std::function<Vector (Vector)>& func, const std::function<Matrix(Vector x)> & jacobian, int image_size, double region)
{
	DrImage image;
	image.create(image_size, image_size);
	auto color = [](double amount, double bord)->sf::Color {
		return sf::Color(bord * 255 / amount, 255, 255 - bord * 255 / amount);
	};
	int ix = 0;
	for (long double x = -region; ix < image_size; x += (2*region / image_size), ix++)
	{
		int iy = 0;
		for (long double y = -region; iy < image_size; y += (2*region / image_size), iy++)
		{
			
			int amount = newtonMethodForEquationsWithCountingSteps(func,
				jacobian, 0.0001, Vector(x, y));
			image.setPixel(ix, iy, color(amount, 20));

		}
		std::cout << (double(ix) / image_size) * 100 << "%\n";
	}
	return image;
}