#define OWN_ANALIT
#include "geometry 3d.h"
#include <cmath>
#define sq(X) ((X)*(X))
double distance(const Vector3d & point, const Plane & plane)
{
	return abs(plane.a() * point.x() + plane.b() * point.y() + plane.c() * point.z() + plane.d()) / sqrt(sq(plane.a()) + sq(plane.b()) + sq(plane.c()));
}
Vector3d getPointOfNormalToPlane(const Vector3d& base_point, const Plane & plane)
{
	double t = -(plane.a() * base_point.x() + plane.b() * base_point.y() + plane.c() * base_point.z() + plane.d()) / (sq(plane.a()) + sq(plane.b()) + sq(plane.c()));
	return base_point + plane.normal() * t;
}
Plane getPlaneThroughPointPerpendicularToVector(const Vector3d & plain_point, const Vector3d & normal)
{
	Plane plain;
	plain.setA(normal.x());
	plain.setB(normal.y());
	plain.setC(normal.z());
	
	plain.setD(-plain.a() * plain_point.x() - plain.b() * plain_point.y() - plain.c() * plain_point.z());
	return plain;
}
double distance(const Vector3d & vec, const Line3d & line)
{
	Vector3d v = line.getPointOnLine() - vec;
	return (v * line.getBaseVector()).length() / line.getBaseVector().length();
}