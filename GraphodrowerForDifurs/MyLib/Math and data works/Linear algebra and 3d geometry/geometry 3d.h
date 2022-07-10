#pragma once
#include "Vector3.h"
#include "Plane.h"
Vector3d getPointOfNormalToPlane(const Vector3d& base_point, const Plane & plane);
double distance(const Vector3d & point, const Plane & plane);
Plane getPlaneThroughPointPerpendicularToVector(const Vector3d & plain_point, const Vector3d & normal);
double distance(const Vector3d & vec, const Line3d & line);

bool isLineParallelToThePlane(const Line3d& line, const Plane& plane);

bool arePointsInSameHalfSpace(const Vector3d& a, const Vector3d& b, const Plane& plane);

bool isPointInsideTriangle(const Vector3d& point, const Vector3d& a, const Vector3d& b, const Vector3d& c, const Plane& its_plance);
//линия не параллельна плоскости
Vector3d getIntersection(const Line3d& line, const Plane& plane);

Vector3d projectionToLine(const Vector3d& point, const Line3d& line);