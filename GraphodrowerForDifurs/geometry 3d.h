#pragma once
#include "MyLib/MyLib.h"
Vector3d getPointOfNormalToPlane(const Vector3d& base_point, const Plane & plane);
double distance(const Vector3d & point, const Plane & plane);
Plane getPlaneThroughPointPerpendicularToVector(const Vector3d & plain_point, const Vector3d & normal);
double distance(const Vector3d & vec, const Line3d & line);