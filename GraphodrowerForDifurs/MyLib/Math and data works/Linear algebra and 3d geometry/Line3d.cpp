#include "Line3d.h"



Line3d::Line3d()
{
}


Line3d::~Line3d()
{
}
Line3d::Line3d(Vector3d point1, Vector3d point2)
{
	this->point_on_line = point1;
	Vector3d kovector = point2 - point1;
	this->base_vector = kovector * (1. / kovector.length());
	if (this->base_vector.x() + this->base_vector.y() + this->base_vector.z() < 0)
		this->base_vector *= -1;
}
Vector3d Line3d::getBaseVector() const
{
	return this->base_vector;
}
Vector3d Line3d::getPointOnLine() const
{
	return this->point_on_line;
}