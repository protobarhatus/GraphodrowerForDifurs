#include "Dorman_prince.h"

//норма вектора. сейчас - это сумма модулей составл€ющих
long double norma(long double y)
{
	return abs(y);
}
long double norma(Vector2 vec)
{
	return abs(vec.x()) + abs(vec.y());
}
long double _min(long double a, long double b)
{
	return a < b ? a : b;
}
long double _max(long double a, long double b)
{
	return a > b ? a : b;
}


std::pair<Vector, std::pair<long double, long double>> dormanPrinceMethod(long double tol, long double factmax, long double factmin, long double fact, Vector d, long double t, long double step, const std::function<Vector(long double, Vector)>& func)
{
	const long double p = 5;
	static ButchersTable table = dormanPrince();
	long double err = 0;
	step *= 2;
	Vector y1;
	do
	{
		step /= 2;
		y1 = ButcherRK(table, 0, d, t, step, func);
		Vector y1_dash = ButcherRK(table, 1, d, t, step, func);


		err = 1. / (pow(2, p) - 1) * norma(y1 - y1_dash) / norma(y1_dash);
		//err = pow(norma(y1 - y1_dash) / norma(y1_dash), 1. / (pow(2, p) - 1));


	} while (err > tol);
	if (err < tol / 1000.)
		step *= 2;
	//if (err < 1e-12)
	//	step *= 4;
	long double new_step = step * _min(factmax, _max(factmin, fact * pow(tol / err, 1. / (p + 1))));
	return { y1, {step, new_step} };
}


Vector integrateFirstOrderSystem(const std::function<Vector(long double t, const Vector&)>& derivative, long double tol, long double start, long double end,
	long double step, Vector start_values, const std::function<void(long double t, const Vector&)>& setPoint)
{
	while (start != end)
	{
		setPoint(start, start_values);
		auto r = dormanPrinceMethod(tol, 2, 0.5, 1, start_values, start, step, derivative);
		if (step >= end - start)
		{
			start_values = RK4(start_values, start, end - start, derivative);
			start = end;

		}
		else
		{
			start_values = r.first;
			//step = r.second.first;
			//start += step;
			start += r.second.first;
			step = r.second.second;
		}

	}
	return start_values;
}