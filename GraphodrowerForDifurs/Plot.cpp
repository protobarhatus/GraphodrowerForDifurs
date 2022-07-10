
#include "Plot.h"
#include "MyLib/MyLib.h"


Plot::Plot(sf::RenderWindow * window)
{
	this->image.setWindow(window);
	this->image.setScalingAntialiasingCoefficient(2.);
	this->is_adaptive_scale_enabled = true;
}
Plot::Plot()
{
	this->image.setScalingAntialiasingCoefficient(2.);
	this->is_adaptive_scale_enabled = true;
}

Plot::~Plot()
{
}
void Plot::setSize(int x, int y)
{
	this->image.create(x, y, sf::Color::White);
}
void Plot::setCoordinatesCenter(int x, int y)
{
	
	this->x_coord_center = x;
	this->y_coord_center = y;
}
void Plot::drawAxis()
{
	int x = (x_coord_center > 0 ? x_coord_center : 0);
	x = (x < this->image.getWidth() ? x : this->image.getWidth() - 1);
	this->image.drawLine(x, 0, x , this->image.getHeight() - 1, sf::Color::Black);
	if (x > 0)
		this->image.drawLine(x - 1, 0, x - 1, this->image.getHeight() - 1, sf::Color::Black);
	else if (x == 0)
		this->image.drawLine(x + 1, 0, x + 1, this->image.getHeight() - 1, sf::Color::Black);
	else
		this->image.drawLine(1, 0, 1, this->image.getHeight() - 1, sf::Color::Black);

	int y = (y_coord_center > 0 ? y_coord_center : 0);
	y = y < this->image.getHeight() ? y : this->image.getHeight() - 1;
	this->image.drawLine(0, y, this->image.getWidth() - 1, y, sf::Color::Black);
	if (y > 0)
		this->image.drawLine(0, y - 1, this->image.getWidth() - 1, y - 1, sf::Color::Black);
	else if (y == 0)
		this->image.drawLine(0, y + 1, this->image.getWidth() - 1, y + 1, sf::Color::Black);
	else
		this->image.drawLine(0, 1, this->image.getWidth() - 1,1, sf::Color::Black);
}
void Plot::setCoordinatesCenterAsCenter()
{
	this->setCoordinatesCenter(this->image.getWidth() / 2, this->image.getHeight() / 2);
}

std::pair<int, int> Plot::fromPlotToImageCoords(double x, double y)
{
	int im_x = this->x_coord_center + x * (this->image.getWidth() - this->x_coord_center) / this->x_scale;
	int im_y = this->y_coord_center - y * (this->y_coord_center) / this->y_scale;
	if (im_y == this->image.getHeight())
		--im_y;
	return std::pair<int, int>(im_x, im_y);
}
bool Plot::inImage(int x, int y)
{
	return x >= 0 && y >= 0 && x < this->image.getWidth() && y < this->image.getHeight();
}
void Plot::setFunction(const std::function<double(double) > & func)
{
	this->type = Plot::PLOT_TYPE_FUNCTIONAL;
	this->functional_plot_f = func;
}
void Plot::setFunction(const std::function<Vector2(double)> & func)
{
	this->type = Plot::PLOT_TYPE_PARAMETRICAL;
	this->parametrical_plot_f = func;
}
void Plot::setFunctionAndCompute(const std::function<Vector2(double)>& func)
{
	this->setFunction(func);
	this->uses_many_plots = true;
	this->saveToBuffer();
}
void Plot::setRangeParams(double begin, double iend, double istep)
{
	this->start = begin;
	this->end = iend;
	this->step = istep;
}
bool eq(double a, double b)
{
	return abs(a - b) <= 0.000001 * (a + b);
}
void Plot::setStep(long double st)
{
	this->step = st;
}
void Plot::draw()
{
	if (this->uses_many_plots)
		this->drawFromBuffer();
	else
		this->drawOnImage();
	this->display();
}
void Plot::display()
{
	this->image.draw(0, 0);
	this->signScale(this->image.getWindow(), 0, 0);
}
void Plot::draw(sf::RenderTarget & texture, int x_bias, int y_bias)
{
	this->drawOnImage();
	this->image.draw(x_bias, y_bias, &texture);
	this->signScale(&texture, x_bias, y_bias);
}
void Plot::signScale(sf::RenderTarget * target, int x_bias, int y_bias)
{
	int x;
	if (this->x_coord_center <= this->image.getWidth() - 50)
		x = this->x_coord_center > 0 ? this->x_coord_center + 2 : 2;
	else
		x = (this->x_coord_center < this->image.getWidth() ? this->x_coord_center - 50 : this->image.getWidth() - 50);
	
	std::string str = string(this->y_scale).substr(0, 5);
	BUF_DrawString(str, x + x_bias, 2 + y_bias, 15, target, sf::Color::Black, "Microsoft Sans Serif.ttf");

	int y;
	if (this->y_coord_center <= this->image.getHeight() - 17)
		y = this->y_coord_center > 0 ? this->y_coord_center + 2 : 2;
	else
		y = (this->y_coord_center < this->image.getHeight() ? this->y_coord_center - 17 : this->image.getHeight() - 17);
	str = string(this->x_scale).substr(0, 5);
	BUF_DrawString(str, this->image.getWidth() - 50 + x_bias, y + y_bias, 15, target, sf::Color::Black, "Microsoft Sans Serif.ttf");
}
void Plot::drawOnImage()
{
	std::vector<std::pair<Vector2, Vector2>> plot_lines;
	auto addLinesToBuffer = [&plot_lines](Vector2 a, Vector2 b)
	{
		if (plot_lines.empty())
			plot_lines.push_back(std::pair<Vector2, Vector2>(a, b));
		if (eq(a.x(), b.x()))
		{
			if (eq(a.x(), plot_lines.back().first.x()) && eq(a.x(), plot_lines.back().second.x()))
				plot_lines.back().second = b;
			else
				plot_lines.push_back(std::pair<Vector2, Vector2>(a, b));
		}
		long double coe1 = (b.y() - a.y()) / (b.x() - a.x());
		long double coe2 = (plot_lines.back().second.y() - plot_lines.back().first.y()) / (plot_lines.back().second.x() - plot_lines.back().first.x());
		if (abs(coe2 - coe1) <= 0.00001)
		{
			plot_lines[plot_lines.size() - 1].second = b;
		}
		else
		{
			plot_lines.push_back(std::pair<Vector2, Vector2>(a, b));
		}
	};

	Vector2 last_point = getPoint(start);
	if (is_adaptive_scale_enabled && !this->have_initialized_bounds)
	{
		min_bound = last_point;
		max_bound = last_point;
		this->have_initialized_bounds = true;
	}
	for (long double param = start + step; param <= end; param += step)
	{
		Vector2 point = getPoint(param);
		if (is_adaptive_scale_enabled)
		{
			min_bound = min(min_bound, point);
			max_bound = max(max_bound, point);
		}
		addLinesToBuffer(last_point, point);
		last_point = point;
	}
	this->countVisualParams();
	this->drawAxis();
	for (auto &it : plot_lines)
	{
		auto first = this->fromPlotToImageCoords(it.first.x(), it.first.y());
		auto second = this->fromPlotToImageCoords(it.second.x(), it.second.y());
		if (inImage(first.first, first.second) && inImage(second.first, second.second))
			this->image.drawLine(first.first, first.second, second.first, second.second, sf::Color::Black);
		if (this->draw_points)
			this->image.drawCircle(second.first, second.second, 3, sf::Color::Color::Red);
	}
}
void Plot::saveToBuffer()
{
	this->diff_plots.push_back(std::vector<Vector2>());
	std::vector<Vector2>& plot_lines = this->diff_plots.back();
	

	Vector2 last_point = getPoint(start);
	if (is_adaptive_scale_enabled && !this->have_initialized_bounds)
	{
		min_bound = last_point;
		max_bound = last_point;
		this->have_initialized_bounds = true;
	}
	addPointToBuffer(plot_lines, last_point);
	for (double param = start + step; param <= end; param += step)
	{
		Vector2 point = getPoint(param);
		if (is_adaptive_scale_enabled)
		{
			min_bound = min(min_bound, point);
			max_bound = max(max_bound, point);
		}
		addPointToBuffer(plot_lines, point);
		last_point = point;
	}
}
void Plot::setDrawingPoints(bool enabled)
{
	this->draw_points = enabled;
}
void Plot::drawFromBuffer()
{
	this->countVisualParams();
	this->drawAxis();
	for (auto& plot_lines : this->diff_plots)
	{
		if (plot_lines.size() == 0)
			continue;
		auto last_point = plot_lines.begin();
		auto fp = this->fromPlotToImageCoords(last_point->x(), last_point->y());
		if (this->draw_points && inImage(fp.first, fp.second))
			this->image.drawCircle(fp.first, fp.second, 3, sf::Color::Color::Red);
		for (auto next_point = next(last_point); next_point != plot_lines.end(); ++next_point)
		{
			auto first = this->fromPlotToImageCoords(last_point->x(), last_point->y());
			auto second = this->fromPlotToImageCoords(next_point->x(), next_point->y());
			if (inImage(first.first, first.second) && inImage(second.first, second.second))
				this->image.drawLine(first.first, first.second, second.first, second.second, sf::Color::Black);
			if (this->draw_points && inImage(second.first, second.second))
				this->image.drawCircle(second.first, second.second, 3, sf::Color::Color::Red);
			last_point = next_point;
		}
		
	}
}
Vector2 Plot::getPoint(double param)
{
	if (this->type == PLOT_TYPE_FUNCTIONAL)
		return Vector2(param, this->functional_plot_f(param));
	if (this->type == PLOT_TYPE_PARAMETRICAL)
		return this->parametrical_plot_f(param);
}
void Plot::setAdaptiveScale(bool enabled)
{
	this->is_adaptive_scale_enabled = enabled;
}
void Plot::countVisualParams()
{
	if (this->is_adaptive_scale_enabled)
	{
		this->min_bound = Vector2(this->min_bound.x() > 0 ? this->min_bound.x() * 0.95 : this->min_bound.x() * 1.05,
			this->min_bound.y() > 0 ? this->min_bound.y() * 0.95 : this->min_bound.y() * 1.05);
		this->max_bound = Vector2(this->max_bound.x() > 0 ? this->max_bound.x() * 1.05 : this->max_bound.x() * 0.95,
			this->max_bound.y() > 0 ? this->max_bound.y() * 1.05 : this->max_bound.y() * 0.95);
	}
	this->x_coord_center = this->min_bound.x()*this->image.getWidth() / (this->min_bound.x() - this->max_bound.x());
	this->y_coord_center = this->image.getHeight() - this->min_bound.y()*this->image.getHeight() / (this->min_bound.y() - this->max_bound.y());

	this->x_scale = this->max_bound.x();
	this->y_scale = this->max_bound.y();
}
void Plot::setConstantBorders(const Vector2 & min, const Vector2 & max)
{
	this->min_bound = min;
	this->max_bound = max;
	this->is_adaptive_scale_enabled = false;
}
void Plot::saveToFile(const std::string & name)
{
	this->image.saveToFile(name);
}

void Plot::initializePointPlotDrawing()
{
	this->diff_plots.push_back(std::vector<Vector2>());
	this->uses_many_plots = true;
}

void Plot::addPoint(const Vector2& point)
{
	this->addPointToBuffer(this->diff_plots.back(), point);
}



void Plot::addPointToBuffer(std::vector<Vector2>& plot_lines, const Vector2 & b)
{
	if (is_adaptive_scale_enabled)
	{
		min_bound = min(min_bound, b);
		max_bound = max(max_bound, b);
		this->have_initialized_bounds = true;
	}
		if (plot_lines.size() < 2)
		{
			plot_lines.push_back(b);
			return;
		}
		Vector2 a = plot_lines.back();
		Vector2 c = *(----plot_lines.end());
		if (eq(a.x(), b.x()))
		{
			if (eq(a.x(), c.x()))
				plot_lines.back() = b;
			else
				plot_lines.push_back(b);
			return;
		}

		double coe1 = (b.y() - a.y()) / (b.x() - a.x());
		double coe2 = (a.y() - c.y()) / (a.x() - c.x());
		if (abs(coe2 - coe1) <= 0.01)
		{
			plot_lines[plot_lines.size() - 1] = b;
		}
		else
		{
			plot_lines.push_back(b);
		}
}
