#pragma once
#include "MyLib/MyLib.h"
#include <SFML\Graphics.hpp>
#include <functional>

class Plot
{
public:
	Plot(sf::RenderWindow * window);
	Plot();
	~Plot();
	void setSize(int x, int y);
	void setCoordinatesCenter(int x, int y);
	void setCoordinatesCenterAsCenter();
	
	void setFunction(const std::function<double(double)> & func);
	void setFunction(const std::function<Vector2(double)> & func);
	void setAdaptiveScale(bool enabled);
	void setRangeParams(double begin, double end, double step);
	void draw();
	void draw(sf::RenderTexture & texture, int x_bias, int y_bias);
	void setConstantBorders(const Vector2 & min, const Vector2 & max);
	void display();
	//нужно для отрисовки нескольких графиков на одном полотне. не рисует, а сохраняет результат в буфер
	void setFunctionAndCompute(const std::function<Vector2(double)>& func);

	void setStep(long double step);
	void setDrawingPoints(bool enabled);
	void saveToFile(const std::string& name);
	void initializePointPlotDrawing();
	void addPoint(const Vector2 & point);

private:
	void addPointToBuffer(std::vector<Vector2>& plot_lines, const Vector2 & b);
	bool uses_many_plots = false;
	bool draw_points = false;
	std::vector<std::vector<Vector2>> diff_plots;
	bool have_initialized_bounds = false;
	void signScale(sf::RenderTarget * canvas, int x_bias, int y_bias);
	void countVisualParams();
	void drawOnImage();
	void saveToBuffer();
	void drawFromBuffer();
	Vector2 getPoint(double param);
	enum PlotType {
		PLOT_TYPE_FUNCTIONAL,
		PLOT_TYPE_PARAMETRICAL,
	};
	PlotType type;
	long double start, end, step;
	std::function<double(double)> functional_plot_f;
	std::function<Vector2(double)> parametrical_plot_f;
	bool inImage(int x, int y);
	std::pair<int, int> fromPlotToImageCoords(double x, double y);
	double x_scale, y_scale;
	void drawAxis();
	int x_coord_center;
	int y_coord_center;

	bool is_adaptive_scale_enabled;
	Vector2 min_bound, max_bound;
	DrImage image;
};

