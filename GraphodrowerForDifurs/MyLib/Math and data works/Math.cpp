#include<random>
#include<math.h>
#include <vector>
#include<C:\MyLib\\Math and data works\\Math.h>

#define RADIAN 57.2958

int round_to_the_largest_int(double value)
{
	//если не целое
	if (value != int(value))
	{
		value = int(value) + 1;
	}
	return value;
}
int min(int a, int b)
{
	return a < b ? a : b;
}
int max(int a, int b)
{
	return a > b ? a : b;
}
void swap(int &a, int &b)
{
	a += b;
	b = a - b;
	a -= b;
}
bool operator<(const Point a, const Point b)
{
	if (a.X < b.X)
		return true;
	else if (b.X < a.X)
		return false;
	else if (a.Y < b.Y)
		return true;
	else
		return false;
}
int *RandomValuesWithFixedAverage(int Average, int Amount_of_Values, int MaxValue, int MinValue)
{
		
        int *values = new int[Amount_of_Values]();
        int LastSummary = Average * Amount_of_Values;
	while (LastSummary > 0)
	{

		for (int i = 0; i < Amount_of_Values; ++i)
		{
			int L = round_to_the_largest_int(double(LastSummary) / double(Amount_of_Values));
			int T;
			do
			{
			T = rand() % (L + 1);
			} while (values[i] + T < MinValue || values[i] + T > MaxValue);
			values[i] += T;
			LastSummary -= T;
			if (LastSummary == 0)
				break;
		}
	}
	//перемешивание массива, т. к. среднее значений к концу уменьшается
	for (int i = Amount_of_Values - 1; i >= 1; --i)
	{
		swap(values[rand() % i], values[i]);
	}
	return values;
}

int Round(double a)
{
	if (a >= 0)
	{
		if (a < int(a) + 0.5)
			return int(a);
		else
			return int(a) + 1;
	}
	else
	{
		if (a > int(a) - 0.5)
			return int(a);
		else
			return int(a) + 1;
	}
}

int Discharge(int a, int b)
{
	for (int c = 0; c < b; c++)
	{
		a = a * 10;
	}

	return a;
}
double RadiansToDegree(double radians)
{

	return radians * RADIAN;
}
double DegreeToRadians(double degrees)
{

	return degrees / RADIAN;
}

inline double Xincline(double Xcenter, float Radius, float incline)
{
	return Xcenter + cos(DegreeToRadians(incline)) * Radius;
}
inline double Yincline(double Ycenter, float Radius, float incline)
{
	return Ycenter + sin(DegreeToRadians(incline)) * Radius;
}
bool equals(long double A, long double B)
{
	if (abs(A - B) /(A + B) < 0.0000001)
		return true;
	else
		return false;
}

// возвращает -1, если point ниже прямой AB, 0, если принадлежит и 1, если выше
int PointRelativelyStraight(Point point, Point A, Point B)
{
	if (equals((point.X - A.X) / (B.X - A.X), (point.Y - A.Y) / (B.Y - A.Y)))
		return 0;
	//если точка на прямой с тем же x, что и point, находится ниже point, то point выше прямой
	double y = ((point.X - A.X) * (B.Y - A.Y) + A.Y * B.X - A.Y * A.X) / (B.X - A.X);
	if (y > point.Y)
		return -1;
	else
		return 1;
}
// возвращает -1, если point ниже прямой AB, 0, если принадлежит и 1, если выше
int PointRelativelyStraightInInvertedY(Point point, Point A, Point B)
{
	if (equals((point.X - A.X) / (B.X - A.X), (point.Y - A.Y) / (B.Y - A.Y)))
		return 0;
	//если точка на прямой с тем же x, что и point, находится ниже point, то point выше прямой. в инвертированной ординате чем больше y, тем ниже точка
	double y = ((point.X - A.X) * (B.Y - A.Y) + A.Y * B.X - A.Y * A.X) / (B.X - A.X);
	if (y < point.Y)
		return -1;
	else
		return 1;
}
//вряд ли это относится к math, но лучше пока не придумал (или придумал но мне лень менять)
bool isPointInRect(Point point, Point rect_leftupper_vertex, int RectWidth, int RectHeight)
{
	if (point.X < rect_leftupper_vertex.X)
		return false;
	if (point.X > rect_leftupper_vertex.X + RectWidth)
		return false;
	if (point.Y < rect_leftupper_vertex.Y)
		return false;
	if (point.Y > rect_leftupper_vertex.Y + RectHeight)
		return false;
	return true;
}
//включая окружность 
bool isPointInCircle(Point point, Point circle_center, int Radius)
{
	return pow(circle_center.X - point.X, 2) + pow(circle_center.Y - point.Y, 2) <= pow(Radius, 2);
}

std::array<Point, 2> figureOutVertexesOfPerpendicularLine(Point begining_of_given_line, Point intersection_point, double second_line_length, double length)
{
	//проверка на то, передали ли значение length и надо ли его считать
	if (length == -1)
		length = sqrt(pow(begining_of_given_line.X - intersection_point.X, 2) + pow(begining_of_given_line.Y - intersection_point.Y, 2));
	double coe = second_line_length / length;
	std::array<Point, 2> equation_coefficients_from_points;
	equation_coefficients_from_points[0].X = coe * (begining_of_given_line.Y - intersection_point.Y) + intersection_point.X;
	equation_coefficients_from_points[0].Y = coe * (intersection_point.X - begining_of_given_line.X) + intersection_point.Y;

	equation_coefficients_from_points[1].X = coe * (intersection_point.Y - begining_of_given_line.Y) + intersection_point.X;
	equation_coefficients_from_points[1].Y = coe * (begining_of_given_line.X - intersection_point.X) + intersection_point.Y;

	return equation_coefficients_from_points;
}
//вернет 2 точки, у которых точка пересечения перпендикуляра от них на данную прямую и данной прямой находится между начальной точкой 1 (данной) линии и точкой соединения \
или, по-другому говоря, из этих точек можно провести перпендикуляр на данную прямую так, что точка их пересечения заключается между началом данной прямой и точкой соединения прямых
std::array<Point, 2> figureOutVertexesOfLineMakesGivenInsideAngleWithGivenLine(Point begining_of_given_line, Point mutual_point, double angle_in_degrees, double second_line_length)
{
	double length = sqrt(pow(begining_of_given_line.X - mutual_point.X, 2) + pow(begining_of_given_line.Y - mutual_point.Y, 2));
	double BD = second_line_length * cos(DegreeToRadians(angle_in_degrees));
	double K = (BD) / (length - BD);
	Point D((K * begining_of_given_line.X + mutual_point.X) / (K + 1), (K * begining_of_given_line.Y + mutual_point.Y) / (K + 1));
	return figureOutVertexesOfPerpendicularLine(begining_of_given_line, D, second_line_length * sin(DegreeToRadians(angle_in_degrees)));
}
double DistanceInCartesianCoordinates(double X1, double Y1, double X2, int Y2)
{
	return sqrt(pow(X2 - X1, 2) + pow(Y2 - Y1, 2));
}
double FigureOutAngleWithInventeredYInRad(double X1, double Y1, double X2, double Y2)
{

	if (X1 == X2 && Y2 > Y1)
		return DegreeToRadians(90);
	if (X1 == X2 && Y2 < Y1)
		return DegreeToRadians(270);

	double angle_between_nearest_axis = atan(abs(Y1 - Y2) / abs(X1 - X2));
	if (X2 > X1 && Y2 >= Y1)
		return angle_between_nearest_axis;
	else if (X2 < X1 && Y2 >= Y1)
		return DegreeToRadians(180) - angle_between_nearest_axis;
	else if (X2 < X1 && Y2 < Y1)
		return DegreeToRadians(180) + angle_between_nearest_axis;
	else if (X2 > X1 && Y2 < Y1)
		return DegreeToRadians(360) - angle_between_nearest_axis;
}
double SignMod(double value)
{
	return (value <= 0 ? (value == 0 ? 0 : -1) : 1);
}

//returns points that divides segment segment_begining-segment_end such that segment_begining-point / point-segment_end = ratio
Point figureOutPointDividingLineSegmentInRatio(Point segment_begining, Point segment_end, double ratio)
{
	return Point((segment_end.X * ratio + segment_begining.X) / (ratio + 1.0), (segment_end.Y * ratio + segment_begining.Y) / (ratio + 1));
}

