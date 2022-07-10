#pragma once
#include "Plot.h"
#include "MyLib/MyLib.h"

DrImage newtonsPools(const std::function<Vector(Vector)>& func, const std::function<Matrix(Vector x)>& jacobian, int image_size, double region);