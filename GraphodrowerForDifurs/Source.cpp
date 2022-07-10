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
	
	system("pause");
	
	return 0;
}



