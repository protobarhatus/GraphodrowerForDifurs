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
	plot.setRangeParams(0, 1000, step);
	plot.setDrawingPoints(true);
	plot.setSize(800, 800);
	
	//plot.initializePointPlotDrawing();
	
	/*draw smth*/


	
	
	plot.draw();
	//plot.draw(&window, 0, 0, 800, 800);
	window.display();
	
	system("pause");
	plot.saveToFile("G:\\\\q.png");
	return 0;
}



