#include "geometry 3d.h"
#include <cmath>
#include "Math.h"
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

bool isLineParallelToThePlane(const Line3d& line, const Plane& plane)
{
	return equals(scalar(line.getBaseVector(), plane.normal()), 0);
}

bool arePointsInSameHalfSpace(const Vector3d& a, const Vector3d& b, const Plane& plane)
{
	double a_res = plane.a() * a.x() + plane.b() * a.y() + plane.c() * a.z() + plane.d();
	double b_res = plane.a() * b.x() + plane.b() * b.y() + plane.c() * b.z() + plane.d();
	return a_res * b_res >= 0;
}

bool isPointInsideTriangle(const Vector3d& p, const Vector3d& a, const Vector3d& b, const Vector3d& c, const Plane& its_plane)
{
	Vector3d v1 = p + (b - a) * (p - a);
	Vector3d v2 = p + (c - b) * (p - b);
	Vector3d v3 = p + (a - c) * (p - c);

	return arePointsInSameHalfSpace(v1, v2, its_plane) && arePointsInSameHalfSpace(v1, v3, its_plane) && arePointsInSameHalfSpace(v2, v3, its_plane);
}

Vector3d getIntersection(const Line3d& line, const Plane& plane)
{
	Vector3d base = line.getBaseVector();
	Vector3d point = line.getPointOnLine();
	double t = -(plane.a() * point.x() + plane.b() * point.y() + plane.c() * point.z() + plane.d()) / (plane.a() * base.x() + plane.b() * base.y() + plane.c() * base.z());
	return point + base * t;
}

Vector3d projectionToLine(const Vector3d& point, const Line3d& line)
{
	
	Vector3d vec = (point - line.getPointOnLine());
	double ang_cos = scalar(vec, line.getBaseVector()) / (vec.length() * line.getBaseVector().length());
	return line.getPointOnLine() + line.getBaseVector() * ang_cos;
}
