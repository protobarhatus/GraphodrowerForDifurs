#pragma once
#include "ButchersTable.h"

std::pair<Vector, std::pair<long double, long double>> dormanPrinceMethod(long double tol, long double factmax, long double factmin, long double fact, Vector d, long double t, long double step, const std::function<Vector(long double, Vector)>& func);

Vector integrateFirstOrderSystem(const std::function<Vector(long double t, const Vector&)>& derivative, long double tol, long double start, long double end,
	long double step, Vector start_values, const std::function<void(long double t, const Vector&)>& setPoint);


