#include "DrImage.h"
#include <assert.h>
#include "..\Math and data works/Math.h"
DrImage::DrImage()
{
	this->texture.setSmooth(true);
}

DrImage::~DrImage()
{
}

DrImage::DrImage(unsigned int width, unsigned int height, sf::Color default_color)
{
	this->texture.setSmooth(true);
	image.create(width, height, default_color);
	this->need_to_change_texture = true;
}
DrImage::DrImage(sf::Image && im)
{
	this->texture.setSmooth(true);
	this->image = std::move(im);
	this->need_to_change_texture = true;
}
void DrImage::create(unsigned int width, unsigned int height, sf::Color default_color)
{
	this->texture.setSmooth(true);
	if (this->is_scaling_antialiasing_enabled)
		image.create(width * antia_coe, height * antia_coe, default_color);
	else
		image.create(width, height, default_color);
	
	this->need_to_change_texture = true;
}
void DrImage::update()
{
	if (!need_to_change_texture)
		return;
	this->texture.loadFromImage(image);
	this->texture.setSmooth(true);
	sprite = sf::RectangleShape((sf::Vector2f)texture.getSize());
	this->sprite.setTexture(&texture);
	
	this->need_to_change_texture = false;
}
void DrImage::setWindow(sf::RenderWindow * win)
{
	this->window = win;
}
void DrImage::draw(int x, int y)
{
	assert(this->window != nullptr);
	this->draw(x, y, this->window);
}
void DrImage::draw(int x, int y, sf::RenderWindow * win)
{
	this->draw(x, y, dynamic_cast<sf::RenderTarget*>(win));
}
void DrImage::draw(int x, int y, sf::RenderTexture * texture)
{
	this->draw(x, y, dynamic_cast<sf::RenderTarget*>(texture));
}
sf::RenderWindow* DrImage::getWindow()
{
	return this->window;
}
void DrImage::draw(int x, int y, sf::RenderTarget * canvas)
{
	this->update();
	if (this->is_scaling_antialiasing_enabled)
	{
		this->drawReduced(x, y, this->antia_coe, canvas);
		return;
	}

	sf::FloatRect view = canvas->getView().getViewport();
	this->sprite.setPosition(x + view.left, y + view.top);
	canvas->draw(this->sprite);
}

void DrImage::drawVertikalLine(unsigned int x, unsigned int y1, unsigned int y2, sf::Color color)
{
	if (y2 < y1)
		std::swap(y2, y1);
	if (y2 >= image.getSize().y)
		y2 = image.getSize().y - 1;
	for (int y = y1; y <= y2; ++y)
		this->image.setPixel(x, y, color);
}
void DrImage::drawHorizontalLine(unsigned int x1, unsigned int x2, unsigned int y, sf::Color color)
{
	if (x2 < x1)
		std::swap(x2, x1);
	if (x2 >= image.getSize().x)
		x2 = image.getSize().x - 1;
	for (int x = x1; x <= x2; ++x)
		this->image.setPixel(x, y, color);
}
void DrImage::drawReduced(unsigned int x, unsigned int y, double scale, sf::RenderWindow * win)
{
	this->drawReduced(x, y, scale, dynamic_cast<sf::RenderTarget*>(win));
}
void DrImage::drawReduced(unsigned int x, unsigned int y, double scale, sf::RenderTarget * canvas)
{
	this->update();
	sf::FloatRect view = canvas->getView().getViewport();
	this->sprite.setPosition(x + view.left, y + view.top);
	this->sprite.setTexture(&this->texture);
	this->sprite.scale(sf::Vector2f(1. / scale, 1. / scale));
	canvas->draw(this->sprite);
}
void DrImage::setLinesAntialiasing(bool enabled)
{
	this->is_antialiasing_enabled = enabled;
}
void DrImage::drawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, sf::Color color)
{
	if (this->is_scaling_antialiasing_enabled)
	{
		x1 *= antia_coe; y1 *= antia_coe;
		x2 *= antia_coe; y2 *= antia_coe;
	}
	this->need_to_change_texture = true;
	if (y1 == y2)
	{
		this->drawHorizontalLine(x1, x2, y1, color);
		return;
	}
	if (x1 == x2)
	{
		this->drawVertikalLine(x1, y1, y2, color);
		return;
	}
	
	if (x2 < x1)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}
	if (this->is_antialiasing_enabled)
		this->drawAntialiasedLine(x1, y1, x2, y2, color);
	else
		this->drawNotAntialiasedLine(x1, y1, x2, y2, color);
		
	
	
	
}
void DrImage::drawNotAntialiasedLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, sf::Color color)
{
	float coe = (float(y2) - float(y1)) / (x2 - x1);
	float b = y1 - x1 * coe;
	if (x2 >= image.getSize().x)
		x2 = image.getSize().x - 1;
	unsigned int last_y = y1;
	unsigned int cur_y;
	int y_dir = (y2 > y1 ? 1 : -1);
	for (int cur_x = x1; cur_x <= x2; ++cur_x)
	{
		cur_y = cur_x * coe + b;
		if (cur_y >= image.getSize().y && last_y >= image.getSize().y)
		{
			if (cur_y > last_y)
				return;
			last_y = cur_y;
			continue;
		}
		for (int y = (cur_y == last_y ? last_y : last_y + y_dir); int(y) * y_dir <= int(cur_y) * y_dir; y += y_dir)
			if (y < image.getSize().y)
				this->image.setPixel(cur_x, y, color);
		last_y = cur_y;
	}
}
void DrImage::drawAntialiasedLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, sf::Color color)
{
	if (abs(int(x2) - int(x1)) > abs(int(y2) - int(y1)))
		this->algorithmVuOnXAxis(x1, y1, x2, y2, color);
	else if (abs(int(y2) - int(y1)) > abs(int(x2) - int(x1)))
		this->algorithmVuOnYAxis(x1, y1, x2, y2, color);
	else
		this->drawAntialiasedBisectrix(x1, y1, x2, y2, color);
}
//смещает l к r на величину, обратнопропорциональную альфа-компноненту l. если a = 255, смещения не происходит (возвращается l), если a = 0, возвращается r.
sf::Color _shiftColor(sf::Color l, sf::Color r)
{
	sf::Color new_color;
	int shift_coefficient = (255 - l.a);

	double red_shift_proportion = double(l.r - r.r) / 255;
	new_color.r = l.r - red_shift_proportion * shift_coefficient;

	double green_shift_proportion = double(l.g - r.g) / 255;
	new_color.g = l.g - green_shift_proportion * shift_coefficient;

	double blue_shift_proportion = double(l.b - r.b) / 255;
	new_color.b = l.b - blue_shift_proportion * shift_coefficient;

	new_color.a = 255;
	return new_color;
}
void DrImage::algorithmVuOnXAxis(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, sf::Color color)
{
	float coe = (float(y2) - float(y1)) / (x2 - x1);
	float b = y1 - x1 * coe;
	int y_dir = (y2 > y1 ? 1 : -1);
	this->image.setPixel(x1, y1, color);
	this->image.setPixel(x2, y2, color);
	for (int x = x1 + 1; x < x2; ++x)
	{
		float real_y = coe * x + b;
		int pix_y = (y_dir == 1 ? trunc(real_y) : ceil(real_y));
		float distance = abs(real_y - pix_y);
		//this->image.setPixel(x, pix_y, sf::Color::Color(color.r, color.g, color.b, 255 * (1.0 - distance)));
		//this->image.setPixel(x, pix_y + y_dir, sf::Color::Color(color.r, color.g, color.b, 255 * distance));

		this->image.setPixel(x, pix_y, _shiftColor(sf::Color::Color(color.r, color.g, color.b, 255 * (1.0 - distance)), this->image.getPixel(x, pix_y)));
		this->image.setPixel(x, pix_y + y_dir, _shiftColor(sf::Color::Color(color.r, color.g, color.b, 255 * distance), this->image.getPixel(x, pix_y + y_dir)));
		
	}
}
void DrImage::algorithmVuOnYAxis(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, sf::Color color)
{
	if (y2 < y1)
	{
		std::swap(y1, y2);
		std::swap(x1, x2);
	}
	float coe = (float(x2) - float(x1)) / (y2 - y1);
	float b = x1 - y1 * coe;
	int x_dir = (x2 > x1 ? 1 : -1);
	this->image.setPixel(x1, y1, color);
	this->image.setPixel(x2, y2, color);
	for (int y = y1 + 1; y < y2; ++y)
	{
		float real_x = coe * y + b;
		int pix_x = (x_dir == 1 ? trunc(real_x) : ceil(real_x));
		float distance = abs(real_x - pix_x);
		//this->image.setPixel(x, pix_y, sf::Color::Color(color.r, color.g, color.b, 255 * (1.0 - distance)));
		//this->image.setPixel(x, pix_y + y_dir, sf::Color::Color(color.r, color.g, color.b, 255 * distance));
		this->image.setPixel(pix_x, y, _shiftColor(sf::Color::Color(color.r, color.g, color.b, 255 * (1.0 - distance)), this->image.getPixel(pix_x, y)));
		this->image.setPixel(pix_x + x_dir, y, _shiftColor(sf::Color::Color(color.r, color.g, color.b, 255 * distance), this->image.getPixel(pix_x + x_dir, y)));
	}
}
void DrImage::drawAntialiasedBisectrix(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, sf::Color color)
{

#define getShiftedColor(X, Y, Z) _shiftColor(sf::Color::Color(color.r, color.g, color.b, (X)), this->image.getPixel((Y), (Z)))
	//все константы подобраны по наблюдениям в paint.net для черных линий на белом фоне
	if (x1 > 0)
		this->image.setPixel(x1 - 1, y1, getShiftedColor(8, x1 - 1, y1));
	if (y1 > 0)
		this->image.setPixel(x1, y1 - 1, getShiftedColor(60, x1, y1 - 1));
	if (y1 < this->image.getSize().y - 1)
		this->image.setPixel(x1, y1 + 1, getShiftedColor(16, x1, y1 + 1));

	this->image.setPixel(x1, y1, getShiftedColor(227, x1, y1));

	int dir = (y2 > y1 ? 1 : -1);
	for (int x = x1 + 1; x < x2; ++x)
	{
		int y = y1 + (x - x1) * dir;
		this->image.setPixel(x, y, getShiftedColor(239, x, y));
		this->image.setPixel(x, y - 1, getShiftedColor(60, x, y - 1));
		this->image.setPixel(x, y + 1, getShiftedColor(84, x, y + 1));
	}

	this->image.setPixel(x2, y2, getShiftedColor(227, x2, y2));

	if (x2 < this->image.getSize().x - 1)
		this->image.setPixel(x2 - 1, y2, getShiftedColor(8, x2 - 1, y2));
	if (y2 > 0)
		this->image.setPixel(x2, y2 - 1, getShiftedColor(60, x2, y2 - 1));
	if (y2 < this->image.getSize().y - 1)
		this->image.setPixel(x2, y2 + 1, getShiftedColor(16, x2, y2 + 1));
}
void DrImage::drawLine(sf::Vector2i a, sf::Vector2i b, sf::Color color)
{
	this->drawLine(a.x, a.y, b.x, b.y, color);
}

void DrImage::blackWhiteAntialiasing()
{
	this->need_to_change_texture = true;
	sf::Vector2u size = this->image.getSize();
	sf::Image copy = this->image;
	for (int x = 0; x < size.x; ++x)
	{
		for (int y = 0; y < size.y; ++y)
		{
			if (this->image.getPixel(x, y) == sf::Color::Black)
				continue;
			int amount_of_black_pixels = 0;
			if (x > 0 && this->image.getPixel(x - 1, y) == sf::Color::Black)
				++amount_of_black_pixels;
			if (y > 0 && this->image.getPixel(x, y - 1) == sf::Color::Black)
				++amount_of_black_pixels;
			if (x < size.x - 1 && this->image.getPixel(x + 1, y) == sf::Color::Black)
				++amount_of_black_pixels;
			if (y < size.y - 1 && this->image.getPixel(x, y + 1) == sf::Color::Black)
				++amount_of_black_pixels;
			if (amount_of_black_pixels < 2)
				continue;
			double col = (1. / 2.) * sqrt(amount_of_black_pixels);
			int res = 255 - 255 * col;
			copy.setPixel(x, y, sf::Color::Color(res, res, res, 255));
		}
	}
	this->image = std::move(copy);
}

unsigned int DrImage::getWidth() const
{
	return this->image.getSize().x / this->antia_coe;
}

unsigned int DrImage::getHeight() const
{
	return this->image.getSize().y / this->antia_coe;
}
void DrImage::saveToFile(std::string name)
{
	this->image.saveToFile(name);
}
void DrImage::setScalingAntialiasingCoefficient(double coe)
{
	this->antia_coe = coe;
	this->is_scaling_antialiasing_enabled = true;
}
void DrImage::drawCircle(int x, int y, int rad, sf::Color color)
{
	if (this->is_scaling_antialiasing_enabled)
	{
		x *= this->antia_coe;
		y *= this->antia_coe;
		rad *= this->antia_coe;
	}
	int xc = x;
	int yc = y;
	x = x - rad;
	y = y - rad;
	for (; x <= xc + rad; ++x)
	{
		for (y = yc - rad; y <= yc + rad; ++y)
		{
			if (x < this->image.getSize().x && x >= 0 && y < this->image.getSize().y && y >= 0 &&
				isPointInCircle(Point(x, y), Point(xc, yc), rad))
				this->image.setPixel(x, y, color);
		}
	}
}
//shit
void DrImage::drawCircumference(int x, int y, int rad, int width, sf::Color color)
{
	int outter_rad = rad + width / 2;
	int inner_rad = outter_rad - width;
	if (this->is_scaling_antialiasing_enabled)
	{
		x *= this->antia_coe;
		y *= this->antia_coe;
		rad *= this->antia_coe;
		outter_rad *= this->antia_coe;
		inner_rad *= this->antia_coe;
	}
	int xc = x;
	int yc = y;
	x = x - outter_rad;
	y = y - outter_rad;
	for (; x <= xc + outter_rad; ++x)
	{
		for (y = yc - outter_rad; y <= yc + outter_rad; ++y)
		{
			if (x < this->image.getSize().x && x >= 0 && y < this->image.getSize().y && y >= 0 &&
				isPointInCircle(Point(x, y), Point(xc, yc), outter_rad) && !isPointInCircle(Point(x, y), Point(xc, yc), inner_rad))
				this->image.setPixel(x, y, color);
		}
	}
}

void DrImage::drawSquare(int x_left, int y_top, int width, int height, sf::Color color)
{
	int x_bound = min(x_left + width, this->getWidth());
	int y_bound = min(y_top + height, this->getHeight());
	this->drawLine(x_left, y_top, x_left, y_bound - 1, color);
	this->drawLine(x_left, y_bound - 1, x_bound - 1, y_bound - 1, color);
	this->drawLine(x_bound - 1, y_top, x_bound - 1, y_bound - 1, color);
	this->drawLine(x_left, y_top, x_bound - 1, y_top, color);
}

bool DrImage::setPixel(int x, int y, sf::Color color)
{
	if (!inImage(x, y))
		return false;
	this->need_to_change_texture = true;
	this->image.setPixel(x, y, color);
	return true;
}

bool DrImage::inImage(int x, int y)
{
	return x >= 0 && x < this->getWidth() && y >= 0 && y < this->getHeight();
}
