#pragma once
#include "Plot.h"
#include <vector>
#include "MyLib/MyLib.h"
#include "geometry 3d.h"
class Plot3D
{
public:
	Plot3D();
	~Plot3D();
	void setRangeParams(double begin, double end, double step);
	void setFunction(const std::function < Vector3d(double)> & parametrical_function);
	void draw(sf::RenderWindow * window, int x_pos, int y_pos, int width, int height);
	void draw(sf::RenderTarget * target, int x_pos, int y_pos, int width, int height);
private:
	void drawProjection(sf::RenderTarget& target, int x, int y, int width, int height, const Plane & plane);
	void countPlot();
	std::vector<std::pair<Vector3d, Vector3d>> lines;
	double max_x = std::numeric_limits<double>::min(), max_y = std::numeric_limits<double>::min(), max_z = std::numeric_limits<double>::min();
	double min_x = std::numeric_limits<double>::max(), min_y = std::numeric_limits<double>::max(), min_z = std::numeric_limits<double>::max();
	double t_min, t_max, step;
	std::function<Vector3d(double)> par_function;
	bool has_counted = false;
};

