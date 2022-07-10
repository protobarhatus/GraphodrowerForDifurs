#pragma once
class Vector;
class Vector2
{
public:
	Vector2(long double x, long double y);
	Vector2();
	~Vector2();
	Vector2(const Vector2 &);
	Vector2(const Vector& vec);
	Vector2& operator=(const Vector& vec);
	long double x() const;
	long double y() const;
	long double length() const;
	Vector2 operator+(Vector2 b) const;
	Vector2 operator-(Vector2 b) const;
	Vector2 operator*(long double val ) const;


	Vector2& operator+=(Vector2 b);
	Vector2& operator-=(Vector2 b);
	Vector2& operator*=(long double val);
	
	Vector2& operator=(int val);
	Vector2 create(int num);
private:
	long double _x, _y;
};

Vector2 min(const Vector2 & a, const Vector2 & b);
Vector2 max(const Vector2 & a, const Vector2 & b);

Vector2 operator*(long double val, const Vector2 & vec);