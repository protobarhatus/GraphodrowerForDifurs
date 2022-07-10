#include "Plot.h"
#include "SFML\Graphics.hpp"
#include <cmath>
#include <iostream>
#include "Plot3D.h"
#include "ButchersTable.h"
#include "MyLib/MyLib.h"
#include <complex>
#include <iomanip>
#include <iostream>











int main()
{
	std::cout.sync_with_stdio(false);
	std::cout << std::fixed;
	std::cout << std::setprecision(10);
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(800, 800), "Plot", sf::Style::Default, settings);


	


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
	
	system("pause");
	plot.saveToFile("G:\\\\q.png");
	return 0;
}



