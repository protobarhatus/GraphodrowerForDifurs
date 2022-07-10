# GraphodrowerForDifurs

This is my shool project created back in 10th grade using C++/SFML. This is a library for numerical solution and drawing of *differential equations* and
*systems of differential equations*. It have a variaty of tools to solve and draw them.

## Theory
In mathematics, a differential equation is an equation that relates one or more unknown functions and their derivatives. In applications, the functions generally
represent physical quantities, the derivatives represent their rates of change, and the differential equation defines a relationship between the two. Such relations are
common; therefore, differential equations play a prominent role in many disciplines including engineering, physics, economics, and biology.

If first-order equation (or system) can be expressed like this: `dy/dx = f(x, y(x))` (y is a number or vector or vector of vectors etc)
then it can be solved using standart methods that are represented in my project.

## ButcherTable
### About Butcher's table
Butcher's table is method of defining exact method of Runge-Kutta family. It looks like that:

![image](https://user-images.githubusercontent.com/51193423/177999674-a176f02d-5612-4b25-a352-47fc98da61cc.png)



In programm, it is represented by `ButcherTable` class. To create table, use constructor function
`ButchersTable(const Table& table, int amount_of_result_strings, bool is_explicit)`, where `table` is `std::vector<std::vector<long double>>` object with
all of coefficients placed exactly as in picture. The `amount_of_result_strings` parameter is responsible for amount of *b* lines under the table (used for some
fancy methods like Dorman-Prince, but for standart methods it must be 1). The is_explicit parameter is showing if method is explicit.
Explicit methods are methods with triangle table that looks like that:

![image](https://user-images.githubusercontent.com/51193423/177998989-9fbda891-280d-4a46-bf0f-b61221d4f174.png)

!For current moment, only explicit method can be integrated. Inexplicit method will give an error.

### Presets
There are some standart methods that can be received via functions:
```cpp
ButchersTable dormanPrince();
ButchersTable gaussLegendreFourthOrder();
ButchersTable tableForRK4();
ButchersTable LStable3rdOrderImplicitRungeKutta();
ButchersTable fifthOrderRadau();
ButchersTable tableForRK6();
ButchersTable explicitEuler();
```

where RK4 and Rk6 are classical methods of corresponding order.
### Integration of equations
Lets imagine we have equation `dy/dt = f(t, y)`, where y is a Vector and we have value y(t<sub>0</sub>). To get a value of y(t<sub>0</sub> + *step*), use
function:
```cpp
Vector ButcherRK(const ButchersTable& table, int result_string, Vector d, long double t, long double step, const std::function < Vector(long double, const Vector&)>& f,
	std::vector<std::pair<double, Vector>> * saved_values_at_proceeded_times = nullptr);
```
where `table` is integration method, `result_string` is what line of `b` params in ButcherTable will be used (almost always its 0), `d` is t<sub>0</sub>,
`f` is `f(t, y)`, `saved_values_at_proceeded_times` is optional parameter. If its not nullptr, then values of y(t<sub>0</sub> + _b1 * step_),
y(t<sub>0</sub> + _b1 * step + b2 * step_) etc will be saved there (object must be created and deleted externally).

### Integration of systems of equation.
Lets imagine we have system of equations:

![image](https://user-images.githubusercontent.com/51193423/178008728-d2451fc8-9419-474f-aeb1-c5fb7b18d11d.png)

and values y<sub>1</sub>(t), ... , y<sub>n</sub>(t) at given time *t*.
Then, use function
```cpp
std::vector<Vector> ButcherRkForSystem(const ButchersTable& table, const std::vector<Vector>& start_values, long double t, long double step,
	const std::vector<std::function<Vector(long double, const std::vector<Vector>& variables)>>& funcs_for_each_var);
```
where table is integration method, start_values is an array of y<sub>1</sub>(t), ... y<sub>n</sub>(t), funcs_for_each_var is an array of functions
f<sub>1</sub>(x, y<sub>1</sub>, ..., y<sub>n</sub>), ..., f<sub>n</sub>(x, y<sub>1</sub>, y<sub>n</sub>)

Also, you can use function
```cpp
Vector RK4(const Vector& start_values, long double start, long double step, std::function<Vector(long double, const Vector&)> func);
```

for Runge-Kutta of 4th order.

## Dormand–Prince method

Dormand-Prince method is method based on *Butcher's table*. More specifically, it uses six function evaluations to calculate fourth- and fifth-order accurate solutions.
The difference between these solutions is then taken to be the error of the (fourth-order) solution. This error estimate is very convenient for adaptive stepsize
integration algorithms.

To use it, include file `Dormand_prince.h`. Then use function
```cpp
std::pair<Vector, std::pair<long double, long double>> dormanPrinceMethod(long double tol, long double factmax, long double factmin, long double fact, Vector d0,
long double t, long double step, const std::function<Vector(long double, Vector)>& func);
```

Where `tol` is responsible for accuracy, `factmax` and `factmin` are max and min multipliers of step, `fact` is responcible for step multiplication,
`d0` is start value, `t` is current time moment (independent variable), `step` is an initial step suggestion (step_size on last iteration) and func
is *f* in y' = f(t, y).

It returns three objects: y(t + step_made), step_made and step_new, where *step_new* is a suggested step for a new iteration.

## Drawing

Any method of plotting can be used to draw equations, but the simple instrument of plotting is also made in this project based on SFML. It's classes
`Plot` and `Plot3D`.

### Plot

#### Drawing single function
Class `Plot` can draw functions of two types: y(x) and x(t), y(t).
To use it, first create an object:
```cpp
Plot plot;
OR
Plot plot(&window);
```
Before drawing anything it, one should necessarily call this functions:
```cpp
void plot.setSize(w, h); // sets size of the image in pixels
void plot.setRangeParams(double begin, double end, double step); // settings of the parametr (in case of y(x), this is "x", in case of x,y(t) this is t)
```

Also, should be called one of this functions:
```cpp
void plot.setConstantBorders(const Vector2 & min, const Vector2 & max); // sets a zone in math coordinates that will be drawed
OR
void plot.setAdaptiveScale(bool enabled); // allows to automaticly set this zone
```

Also, optionally, one may use
`void plot.setDrawingPoints(bool enabled);` to draw points as a red dots.

Also, to set center of coordinates (point (0, 0)) to a specific place on the image, functions
```cpp
void plot.setCoordinatesCenter(int x, int y);
void plot.setCoordinatesCenterAsCenter();
```
can be called. If not, center will be placed somewhere automaticly

Then, to set a function to draw, one of the following should be used
```cpp
void plot.setFunction(const std::function<double(double)> & func); // y(x)
void plot.setFunction(const std::function<Vector2(double)> & func); // x(t), y(t)
```


And, finally, to draw, these functions must be called:
```cpp
void plot.draw();                // If window was set before, otherwise - crash
void plot.draw(sf::RenderTarget & texture, int x_bias, int y_bias);
```

Also, plot can be saved to the file `plot.saveToFile(filename);`

#### Drawing of many functions on one plot

Drawing of many functions supported only for functions of the type x(t), y(t). 
For it, instead of ` void plot.setFunction(const std::function<Vector2(double)> & func);`, use
`void setFunctionAndCompute(const std::function<Vector2(double)>& func);`

Other steps are the same

### Plot3D

`Plot3D` is similar to `Plot`, but it draws functions x(t), y(t), z(t). It actually DOESNT draw 3D pictures, instead
it draws *projections* of plot to XoY, YoZ, XoZ planes.

Usage of it is a simpler, but it also has less options:
First, create object
```cpp
Plot3D plot
```

Then, call
```cpp
void setRangeParams(double begin, double end, double step);
```

Then, set your function
```cpp
void setFunction(const std::function < Vector3d(double)> & parametrical_function);
```

And, finally, use one if these:
```cpp
void draw(sf::RenderWindow * window, int x_pos, int y_pos, int width, int height);
void draw(sf::RenderTarget * target, int x_pos, int y_pos, int width, int height);
```

## Examples

### Van der Pol oscillator

![image](https://user-images.githubusercontent.com/51193423/178153913-17049f89-fff8-45db-95b7-3c91f92d8fa5.png)

Drawn in Coordinates (y', y)

(Note: second-order equation is integrated as first order system { (y')' = f(t, y, y', y'') && (y)' = y'}

```cpp
Plot plot(&window);
plot.setAdaptiveScale(true);
	
long double step = 0.1;
plot.setRangeParams(0, 200, step);
plot.setSize(800, 800);
  
double d = 0.3, om = 1.0, a = 1.0;
Vector point(0.1, 0.1);
plot.setFunction([d, om, a, &point, step](double t)->Vector2 {
	Vector old_point = point;
	point = RK4(point, t, step, [d, om, a](long double, const Vector& v) {
		return Vector(2 * d * v.x() * (1.0 - a * v.y() * v.y()) - om * om * v.y(), v.x());
		});
	return old_point;
	});
	
plot.draw();
	
window.display();
```

![image](https://user-images.githubusercontent.com/51193423/178154474-a6b854fe-b938-4380-9c40-7956931b62f3.png)

### Lorenz attractor

![image](https://user-images.githubusercontent.com/51193423/178154789-c3020a6c-7672-4326-a5ca-68013ad41f77.png)

```cpp
	Plot3D plot;
	double step = 0.01;
	plot.setRangeParams(0, 200, step);

	double s = 10, r = 28, b = 8.0 / 3.0;
	Vector p(1, 1, 1);
	plot.setFunction([s, r, b, &p, step](double t)->Vector3d {
		Vector old_p = p;
		p = RK4(p, t, step, [s, r, b](long double t, const Vector& p) {
			return Vector(s * (p.y() - p.x()), p.x() * (r - p.z()) - p.y(), p.x() * p.y() - b * p.z());
			});
		return old_p;
		});
	
	plot.draw(&window, 0, 0, 800, 800);
	window.display();
```

![image](https://user-images.githubusercontent.com/51193423/178156510-e7aa74b4-9681-473b-af90-df12856d1fba.png)



## Newton's pools (or Newton's fractals)
### Newton's method
In numerical analysis, Newton's method, also known as the Newton–Raphson method, named after Isaac Newton and Joseph Raphson, is a root-finding algorithm which
produces successively better approximations to the roots (or zeroes) of a real-valued function. The most basic version starts with a single-variable function f defined
for a real variable x, the function's derivative f′, and an initial guess x0 for a root of f. If the function satisfies sufficient assumptions and the initial guess is
close, then

![image](https://user-images.githubusercontent.com/51193423/178156688-e2b66b00-5773-4da3-baa1-16e4d7bd00eb.png)

### Newton's pools

Let f(x) == 0 is an equation of complex variable that have multiple roots. Let p is some point on complex plane. Newton's method will converge to some root
in some amount of steps. So, newton's pool is a graph that shows for each point on it how many steps does it take to converge to root and to which
specifically root it will converge. These graph has a fractal structure, so its also called Newton's fractal.

To build a Newton's pool, first include `Newtons_pool.h`. Then, use a function

```cpp
DrImage newtonsPools(const std::function<Vector(Vector)>& func, const std::function<Matrix(Vector x)>& jacobian, int image_size, double r);
```

where `func` is 2d function {Fx(x, y), Fy(x, y)} and `jacobian` is a matrix of dFx(x, y) /dx , dFx(x, y)/ dy, dFy(x, y) / dx, dFy(x, y)/dy,
`image_size` is size of image in pixels and drawed region is a square from (-r, -r) to (r, r)

DrImage may be drawed by function
```cpp
void DrImage::draw(int x, int y, sf::RenderWindow * window);
```

For example, for f(x) = x^5 - 1:
```cpp
DrImage image = newtonsPools([](Vector x) {return pow(x, 5) - Vector(1, 0); }, [](Vector x) {
		Matrix result(2, 2);
		result[0][0] = 5 * pow(x.x(), 4) - 30 * pow(x.x(), 2) * pow(x.y(), 2) + 5 * pow(x.y(), 4);
		result[1][0] = 20 * pow(x.x(), 3) * x.y() - 20 * x.x() * pow(x.y(), 3);
		result[0][1] = -result[1][0];
		result[1][1] = result[0][0];
		return result;
		}, 800, 2);
image.draw(0, 0, &window);
  ```
  
  ![image](https://user-images.githubusercontent.com/51193423/178157723-a9fd3ed1-d211-44ae-b005-1e5a771cd755.png)

 






