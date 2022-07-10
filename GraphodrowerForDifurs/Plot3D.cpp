#define OWN_ANALIT
#include "Plot3D.h"
#include <cmath>
#include "geometry 3d.h"

Plot3D::Plot3D()
{
}


Plot3D::~Plot3D()
{
}
void Plot3D::setRangeParams(double begin, double end, double step)
{
	this->t_min = begin;
	this->t_max = end;
	this->step = step;
	this->has_counted = false;
}

void Plot3D::setFunction(const std::function < Vector3d(double)> & parametrical_function)
{
	this->has_counted = false;
	this->par_function = parametrical_function;
	this->countPlot();
}
double max(double a, double b)
{
	return a > b ? a : b;
}
double min(double a, double b)
{
	return a < b ? a : b;
}
void Plot3D::countPlot()
{
	this->has_counted = true;
	this->lines.clear();
	Vector3d last_point = this->par_function(this->t_min);

	this->max_x = last_point.x();
	this->max_y = last_point.y();
	this->max_z = last_point.z();
	this->min_x = last_point.x();
	this->min_y = last_point.y();
	this->min_z = last_point.z();

	for (double t = t_min + step; t <= t_max; t += step)
	{
		Vector3d next_point = this->par_function(t);
		this->max_x = max(next_point.x(), this->max_x);
		this->max_y = max(next_point.y(), this->max_y);
		this->max_z = max(next_point.z(), this->max_z);

		this->min_x = min(next_point.x(), this->min_x);
		this->min_y = min(next_point.y(), this->min_y);
		this->min_z = min(next_point.z(), this->min_z);

		this->lines.push_back(std::pair<Vector3d, Vector3d>(last_point, next_point));
		last_point = next_point;
	}

}

#include <iostream>
void Plot3D::drawProjection(sf::RenderTarget& target, int x, int y, int width, int height, const Plane & plane)
{
	Plot plot;
	plot.setRangeParams(0.0, this->lines.size(), 1.0);
	plot.setSize(width, height);
	std::vector<std::pair<Vector3d, Vector3d>> & th_lines = this->lines;
	plot.setFunction([&plane, &th_lines](double t) -> Vector2 {
		int ind = round(t);
		Vector2 res, r1;
		if (ind == th_lines.size())
			res =  plane.getCoordinatesInPlanesUnitsVector(getPointOfNormalToPlane(th_lines.back().second, plane));
		else
			res =  plane.getCoordinatesInPlanesUnitsVector(getPointOfNormalToPlane(th_lines[ind].first, plane));
		return res;
	});
	plot.draw(target, x, y);
}
void Plot3D::draw(sf::RenderWindow * window, int x, int y, int width, int height)
{
	this->draw(dynamic_cast<sf::RenderTarget*>(window), x, y, width, height);
}
void Plot3D::draw(sf::RenderTarget * window, int x, int y, int width, int height)
{
	//плоскость XOZ
	this->drawProjection(*window, x + 0, y + 0, width / 2, height / 2, Plane(0, 1, 0, 0, true));
	//плоскость YOZ
	this->drawProjection(*window, x + width - width / 2, y + 0, width / 2, height / 2, Plane(1, 0, 0, 0, true));
	//плоскость XOY
	this->drawProjection(*window, x + 0, y + height - height / 2, width / 2, height / 2, Plane(0, 0, 1, 0, true));


}