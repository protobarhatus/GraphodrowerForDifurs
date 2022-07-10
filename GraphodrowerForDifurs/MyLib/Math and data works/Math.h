#pragma once
#include<array>
struct Point
{
	double X;
	double Y;
	Point(double x, double y)
	{
		X = x;
		Y = y;
	}
	bool operator==(const Point & point)
	{
		return this->X == point.X && this->Y == point.Y;
	}
	bool operator!=(const Point & point)
	{
		return this->X != point.X || this->Y != point.Y;
	}
	Point() {}
};
bool operator<(const Point a, const Point b);
//double DegreeToRadians(float degrees);
double DegreeToRadians(double degrees);
int round_to_the_largest_int(double value);
int Round(double a);

void swap(int &a, int &b);

int *RandomValuesWithFixedAverage(int Average, int Amount_of_Values, int MaxValue, int MinValue);

inline double Xincline(double Xcenter, float Radius, float incline);
inline double Yincline(double Ycenter, float Radius, float incline);
double RadiansToDegree(double radians);
// double RadiansToDegree(float radians);
int PointRelativelyStraight(Point point, Point A, Point B);
int PointRelativelyStraightInInvertedY(Point point, Point A, Point B);
bool isPointInRect(Point point, Point rect_leftupper_vertex, int RectWidth, int RectHeight);

bool isPointInCircle(Point point, Point circle_center, int Radius);

double DistanceInCartesianCoordinates(double X1, double Y1, double X2, int Y2);
double FigureOutAngleWithInventeredYInRad(double X1, double Y1, double X2, double Y2);

double SignMod(double value);
int Discharge(int a, int b);
int min(int a, int b);
int max(int a, int b);

//если не передавать значение длины данной линии, оно расчитается по формуле Пифагора. Если передать - шаг пропустится
std::array<Point, 2> figureOutVertexesOfPerpendicularLine(Point begining_of_given_line, Point intersection_point, double second_line_length, double length = -1.0);
//вернет 2 точки, у которых точка пересечения перпендикуляра от них на данную прямую и данной прямой находится между начальной точкой 1 (данной) линии и точкой соединения \
или, по-другому говоря, из этих точек можно провести перпендикуляр на данную прямую так, что точка их пересечения заключается между началом данной прямой и точкой соединения прямых
std::array<Point, 2> figureOutVertexesOfLineMakesGivenInsideAngleWithGivenLine(Point begining_of_given_line, Point mutual_point, double angle_in_degrees, double second_line_length);

Point figureOutPointDividingLineSegmentInRatio(Point segment_begining, Point segment_end, double ratio);

bool equals(long double A, long double B);