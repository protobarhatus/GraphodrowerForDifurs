#include"SFML\Graphics.hpp"
#include "SFML Graphic Functions.h"
#include "..\Math and data works\Math.h"
#define HORIZONTALLY false
#define VERTICALLY true
#define SIN_30 0.5
#define COS_30 0.8660254038
#define DEGREE90_IN_RAD 1.5707963268
sf::Color operator*(sf::Color l, int r)
{
	return sf::Color::Color(l.r * r, l.g * r, l.b * r, l.a * r);
}
MImage::MImage()
{
	sprite.setTexture(&texture);
}
MImage::MImage(MImage & copy)
{
	this->image = sf::Image(copy.image);
	this->texture = sf::Texture(copy.texture);
	this->sprite = sf::RectangleShape(copy.sprite);
	this->sprite.setTexture(&texture);
	this->window = copy.window;
}
MImage::MImage(MImage && copy)
{
	this->image = sf::Image(copy.image);
	this->texture = sf::Texture(copy.texture);
	this->sprite = sf::RectangleShape(copy.sprite);
	this->sprite.setTexture(&texture);
	this->window = copy.window;
}
MImage MImage::operator=(MImage & copy)
{
	this->image = sf::Image(copy.image);
	this->texture = sf::Texture(copy.texture);
	this->sprite = sf::RectangleShape(copy.sprite);
	this->sprite.setTexture(&texture);
	this->window = copy.window;
	return *this;
}
MImage MImage::operator=(const MImage & copy)
{
	this->image = sf::Image(copy.image);
	this->texture = sf::Texture(copy.texture);
	this->sprite = sf::RectangleShape(copy.sprite);
	this->sprite.setTexture(&texture);
	this->window = copy.window;
	return *this;
}
MImage MImage::operator=(MImage && copy)
{
	this->image = sf::Image(copy.image);
	this->Update();
	this->window = copy.window;
	return *this;
}

MImage::MImage(sf::Image itexture) { 
	image = itexture;
	
	sprite = sf::RectangleShape(sf::Vector2f(image.getSize().x, image.getSize().y)); 
	Update();
}

MImage::MImage(sf::Image itexture, sf::RenderWindow * iwindow) { 
	image = itexture;
	
	sprite = sf::RectangleShape(sf::Vector2f(image.getSize().x, image.getSize().y));
	Update();
	window = iwindow; 
}
MImage::MImage(std::string file_name) { 
	image.loadFromFile(file_name);
	
	sprite = sf::RectangleShape(sf::Vector2f(image.getSize().x, image.getSize().y));
	Update();
}
void MImage::SetTexture(sf::Image & _image) { 
	image = _image;
	
	sprite = sf::RectangleShape(sf::Vector2f(image.getSize().x, image.getSize().y));
	Update();
}
void MImage::loadFromFile(std::string file_name) { 
	image.loadFromFile(file_name);
	
	sprite = sf::RectangleShape(sf::Vector2f(image.getSize().x, image.getSize().y));
	Update();
}
void MImage::Update()
{
	texture.loadFromImage(image);
	sprite = sf::RectangleShape((sf::Vector2f)texture.getSize());
	sprite.setTexture(&texture);
}
void MImage::RepaintColor(sf::Color color_to_fill, sf::Color color_to_change)
{
	ChangeColor(&this->image, color_to_fill, color_to_change);
	Update();
}
void MImage::RepaintColorWithSavingAlfaComponent(sf::Color color_to_fill, sf::Color color_to_change)
{
	for (int x = 0; x < image.getSize().x; ++x)
	{
		for (int y = 0; y < image.getSize().y; ++y)
		{
			sf::Color pixel = image.getPixel(x, y);
			if (areEqualWithoutAlfaComponent(pixel, color_to_change))
			{
				image.setPixel(x, y, sf::Color::Color(color_to_fill.r, color_to_fill.g, color_to_fill.b, pixel.a));
			}

		}
	}
	Update();
}
void MImage::RepaintByDivision(sf::Color multiplier, sf::Color divider)
{
	ChangeColorByDivision(this->image, multiplier, divider);
	Update();
}



void ChangeColorByDivision(sf::Image & image, sf::Color multiplier, sf::Color divider)
{
	for (int x = 0; x < image.getSize().x; ++x)
	{
		for (int y = 0; y < image.getSize().y; ++y)
		{
			sf::Color pixel = image.getPixel(x, y);
			image.setPixel(x, y, sf::Color::Color((double)pixel.r / divider.r * multiplier.r, (double)pixel.g / divider.g * multiplier.g,
			(double)pixel.b / divider.b * multiplier.b, pixel.a));
			

		}
	}
}

bool areEqualWithoutAlfaComponent(sf::Color a, sf::Color b)
{
	return a.r == b.r && a.g == b.g && a.b == b.b;
}

sf::Image * MImage::GetImage()
{
	return &this->image;
}

sf::CircleShape PutUnderTextureCircle(sf::Texture & texture, sf::Color color, int radius, int OutLineThickness, sf::Color OutLineColor)
{
	sf::CircleShape circle(radius);
	circle.setFillColor(color);
	circle.setOutlineThickness(OutLineThickness);
	circle.setOutlineColor(OutLineColor);
	circle.setTexture(&texture);
	return circle;
}
//смещает l к r на величину, обратнопропорциональную альфа-компноненту l. если a = 255, смещения не происходит (возвращается l), если a = 0, возвращается r. alpha возвращаемого цвета берется максимальная из l и r
sf::Color shiftColor(sf::Color l, sf::Color r)
{
	sf::Color new_color;
	int shift_coefficient = (255 - l.a);

	double red_shift_proportion = double(l.r - r.r) / 255;
	new_color.r = l.r - red_shift_proportion * shift_coefficient;

	double green_shift_proportion = double(l.g - r.g) / 255;
	new_color.g = l.g - green_shift_proportion * shift_coefficient;

	double blue_shift_proportion = double(l.b - r.b) / 255;
	new_color.b = l.b - blue_shift_proportion * shift_coefficient;

	new_color.a = max(l.a, r.a);
	return new_color;
}
void PutCircleUnderImageWithLowerOrEqRadius(MImage & image, sf::Color circle_color, int radius, int outline_thickness, sf::Color outline_color)
{
	sf::Image * im = image.GetImage();
	Point circle_center(image.GetWidth() / 2, image.GetHeight() / 2);
	for (int x = 0; x < image.GetWidth(); ++x)
	{
		for (int y = 0; y < image.GetHeight(); ++y)
		{
			
			if (im->getPixel(x, y).a == 0 && isPointInCircle(Point(x, y), circle_center, radius))
			{
				im->setPixel(x, y, circle_color);
			}
			else if (im->getPixel(x, y).a == 0 && isPointInCircle(Point(x, y), circle_center, radius + outline_thickness))
			{
				im->setPixel(x, y, outline_color);
			}
		}
	}
	image.Update();
}
void PutCircleUnderImageWithBiggerRadius(MImage & image, sf::Color circle_color, int radius, int outline_thickness, sf::Color outline_color)
{
	sf::Image old_image = sf::Image(*image.GetImage());
	sf::Image * new_image = image.GetImage();
	int X_difference = (radius + outline_thickness) * 2 - image.GetWidth();
	int Y_difference = (radius + outline_thickness) * 2 - image.GetHeight();
	new_image->create(image.GetWidth() + X_difference, image.GetHeight() + Y_difference);
	Point circle_center(image.GetWidth() / 2, image.GetHeight() / 2);
	for (int x = 0; x < image.GetWidth(); ++x)
	{
		for (int y = 0; y < image.GetHeight(); ++y)
		{
			bool is_point_in_area_of_old_image = isPointInRect(Point(x, y), Point(X_difference / 2, Y_difference / 2), old_image.getSize().x - 1, old_image.getSize().y - 1);
			bool has_old_image_pixel_there;
			bool is_point_in_circle = isPointInCircle(Point(x, y), circle_center, radius);
			sf::Color old_image_pixel;
			if (is_point_in_area_of_old_image)
			{
				old_image_pixel = old_image.getPixel(x - X_difference / 2, y - Y_difference / 2);
				has_old_image_pixel_there = old_image_pixel.a != 0;
			}
			else
				has_old_image_pixel_there = false;
			if (is_point_in_area_of_old_image && has_old_image_pixel_there)
			{
				if (is_point_in_circle)
					new_image->setPixel(x, y, shiftColor(old_image_pixel, circle_color));
				else
					new_image->setPixel(x, y, old_image_pixel);
			}
			else if (is_point_in_circle)
			{
				new_image->setPixel(x, y, circle_color);
			}
			else if (isPointInCircle(Point(x, y), circle_center, radius + outline_thickness))
			{
				new_image->setPixel(x, y, outline_color);
			}
			else
			{
				new_image->setPixel(x, y, sf::Color::Transparent);
			}
		}
	}
	
	image.Update();
}
void PutCircleUnderImage(MImage & image, sf::Color circle_color, int radius, int outline_thickness, sf::Color outline_color)
{
	if ((radius + outline_thickness) * 2 <= min(image.GetWidth(), image.GetHeight()))
	{
		PutCircleUnderImageWithLowerOrEqRadius(image, circle_color, radius, outline_thickness, outline_color);
	}
	else
	{
		PutCircleUnderImageWithBiggerRadius(image, circle_color, radius, outline_thickness, outline_color);
	}
}
MImage  PutCircleUnderImageAndReturnIt(MImage image, sf::Color circle_color, int radius, int outline_thickness, sf::Color outline_color)
{
	
	PutCircleUnderImage(image, circle_color, radius, outline_thickness, outline_color);
	
	
	return image;
}
//false - horizontally, true - vertically
sf::Texture MakeHealthBar(int health_in_percent, int width, int height, bool bearings)
{
	sf::Texture bar;
	sf::Image ibar;
	ibar.create(width, height);
	if (bearings == HORIZONTALLY)
	{
		for (int x = 0; x <= (double)width / 100 * health_in_percent; ++x)
		{
			for (int y = 0; y < height; ++y)
			{
				ibar.setPixel(x, y, sf::Color::Green);
			}
		}
		for (int x = (double)width / 100 * health_in_percent + 1; x < width; ++x)
		{
			for (int y = 0; y < height; ++y)
			{
				ibar.setPixel(x, y, sf::Color::Red);
			}
		}
	}
	if (bearings == VERTICALLY)
	{
		for (int y = 0; y <= (double)height / 100 * health_in_percent; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				ibar.setPixel(x, y, sf::Color::Green);
			}
		}
		for (int y = (double)height / 100 * health_in_percent + 1; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				ibar.setPixel(x, y, sf::Color::Red);
			}
		}
	}
	bar.update(ibar);
	return bar;
}
//for 4-bytes format
sf::Texture ChangeColor(sf::Texture texture, sf::Color color_to_fill, sf::Color color_to_change)
{
	sf::Image image;
	image.loadFromMemory(&texture, texture.getSize().x * texture.getSize().y * 4);
	for (int x = 0; x < image.getSize().x; ++x)
	{
		for (int y = 0; y < image.getSize().y; ++y)
		{
			if (image.getPixel(x, y) == color_to_change)
			{
				image.setPixel(x, y, color_to_fill);
			}
		}
	}
	sf::Texture final_texture;
	final_texture.loadFromImage(image);
	return final_texture;
}
void ChangeColor(sf::Image * texture, sf::Color color_to_fill, sf::Color color_to_change)
{

	for (int x = 0; x < texture->getSize().x; ++x)
	{
		for (int y = 0; y < texture->getSize().y; ++y)
		{
			sf::Color pixel = texture->getPixel(x, y);
			if (pixel == color_to_change)
			{
				texture->setPixel(x, y, color_to_fill);
			}

		}
	}

}

void BUF_DrawString(std::string str, int X, int Y, int fontsize_in_pixels, sf::RenderWindow * window, sf::Color font_color, sf::Text::Style style, std::string font)
{

	sf::Font ffont;
	if (!ffont.loadFromFile(font))
	{
		throw ("Error with font load, check file " + font);
	}
	sf::Text text;
	text.setFont(ffont);
	text.setString(str);
	text.setCharacterSize(fontsize_in_pixels);
	text.setFillColor(font_color);
	text.setPosition(sf::Vector2f(X, Y));
	text.setStyle(style);
	window->draw(text);
}
void BUF_DrawString(std::string str, int X, int Y, int fontsize_in_pixels, sf::RenderWindow * window, sf::Color font_color, std::string font)
{

	sf::Font ffont;
	if (!ffont.loadFromFile(font))
	{
		throw ("Error with font load, check file " + font);
	}
	sf::Text text;
	text.setFont(ffont);
	text.setString(str);
	text.setCharacterSize(fontsize_in_pixels);
	text.setFillColor(font_color);
	text.setPosition(sf::Vector2f(X, Y));
	window->draw(text);
}
void drawString(std::wstring str, int X, int Y, int fontsize_in_pixels, sf::RenderWindow * window, sf::Color font_color, std::string font)
{

	sf::Font ffont;
	if (!ffont.loadFromFile(font))
	{
		throw ("Error with font load, check file " + font);
	}
	sf::Text text;
	text.setFont(ffont);
	text.setString(str);
	text.setCharacterSize(fontsize_in_pixels);
	text.setFillColor(font_color);
	text.setPosition(sf::Vector2f(X, Y));
	window->draw(text);
}
void drawString(std::string str, int X, int Y, int fontsize_in_pixels, sf::RenderWindow * window, sf::Color font_color, std::string font)
{

	sf::Font ffont;
	if (!ffont.loadFromFile(font))
	{
		throw ("Error with font load, check file " + font);
	}
	sf::Text text;
	text.setFont(ffont);
	text.setString(str);
	text.setCharacterSize(fontsize_in_pixels);
	text.setFillColor(font_color);
	text.setPosition(sf::Vector2f(X, Y));
	window->draw(text);
}
void BUF_DrawString(std::string str, int X, int Y, int fontsize_in_pixels, sf::RenderWindow * window, sf::Color font_color, sf::Font font)
{
	sf::Text text;
	text.setFont(font);
	text.setString(str);
	text.setCharacterSize(fontsize_in_pixels);
	text.setFillColor(font_color);
	text.setPosition(sf::Vector2f(X, Y));
	window->draw(text);
}
void BUF_DrawString(std::string str, int X, int Y, int fontsize_in_pixels, sf::RenderWindow * window, sf::Color font_color, sf::Text::Style style, sf::Font font)
{
	sf::Text text;
	text.setFont(font);
	text.setString(str);
	text.setCharacterSize(fontsize_in_pixels);
	text.setFillColor(font_color);
	text.setPosition(sf::Vector2f(X, Y));
	text.setStyle(style);
	window->draw(text);
}








void BUF_DrawString(std::string str, int X, int Y, int fontsize_in_pixels, sf::RenderTarget * window, sf::Color font_color, sf::Text::Style style, std::string font)
{

	sf::Font ffont;
	if (!ffont.loadFromFile(font))
	{
		throw ("Error with font load, check file " + font);
	}
	sf::Text text;
	text.setFont(ffont);
	text.setString(str);
	text.setCharacterSize(fontsize_in_pixels);
	text.setFillColor(font_color);
	text.setPosition(sf::Vector2f(X, Y));
	text.setStyle(style);
	window->draw(text);
}
void BUF_DrawString(std::string str, int X, int Y, int fontsize_in_pixels, sf::RenderTarget * window, sf::Color font_color, std::string font)
{

	sf::Font ffont;
	if (!ffont.loadFromFile(font))
	{
		throw ("Error with font load, check file " + font);
	}
	sf::Text text;
	text.setFont(ffont);
	text.setString(str);
	text.setCharacterSize(fontsize_in_pixels);
	text.setFillColor(font_color);
	text.setPosition(sf::Vector2f(X, Y));
	window->draw(text);
}
void drawString(std::wstring str, int X, int Y, int fontsize_in_pixels, sf::RenderTarget * window, sf::Color font_color, std::string font)
{

	sf::Font ffont;
	if (!ffont.loadFromFile(font))
	{
		throw ("Error with font load, check file " + font);
	}
	sf::Text text;
	text.setFont(ffont);
	text.setString(str);
	text.setCharacterSize(fontsize_in_pixels);
	text.setFillColor(font_color);
	text.setPosition(sf::Vector2f(X, Y));
	window->draw(text);
}
void drawString(std::string str, int X, int Y, int fontsize_in_pixels, sf::RenderTarget * window, sf::Color font_color, std::string font)
{

	sf::Font ffont;
	if (!ffont.loadFromFile(font))
	{
		throw ("Error with font load, check file " + font);
	}
	sf::Text text;
	text.setFont(ffont);
	text.setString(str);
	text.setCharacterSize(fontsize_in_pixels);
	text.setFillColor(font_color);
	text.setPosition(sf::Vector2f(X, Y));
	window->draw(text);
}
void BUF_DrawString(std::string str, int X, int Y, int fontsize_in_pixels, sf::RenderTarget * window, sf::Color font_color, sf::Font font)
{
	sf::Text text;
	text.setFont(font);
	text.setString(str);
	text.setCharacterSize(fontsize_in_pixels);
	text.setFillColor(font_color);
	text.setPosition(sf::Vector2f(X, Y));
	window->draw(text);
}
void BUF_DrawString(std::string str, int X, int Y, int fontsize_in_pixels, sf::RenderTarget * window, sf::Color font_color, sf::Text::Style style, sf::Font font)
{
	sf::Text text;
	text.setFont(font);
	text.setString(str);
	text.setCharacterSize(fontsize_in_pixels);
	text.setFillColor(font_color);
	text.setPosition(sf::Vector2f(X, Y));
	text.setStyle(style);
	window->draw(text);
}


void DrawLine(int X1, int Y1, int X2, int Y2, sf::Color Color, sf::RenderWindow &window)
{
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(X1, Y1)),
		sf::Vertex(sf::Vector2f(X2, Y2))
	};
	line[0].color = Color;
	line[1].color = Color;
	window.draw(line, 2, sf::Lines);
}
void DrawLine(int X1, int Y1, int X2, int Y2, sf::Color Color, sf::RenderWindow *window)
{
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(X1, Y1)),
		sf::Vertex(sf::Vector2f(X2, Y2))
	};
	line[0].color = Color;
	line[1].color = Color;
	window->draw(line, 2, sf::Lines);
}
//rather slow metod becouse it drawing sf::RectangleShape and computes its vertexes using trigonometry
void DrawLine(double X1, double Y1, double X2, double Y2, int width, sf::Color color, sf::RenderWindow *window)
{
	sf::RectangleShape line(sf::Vector2f(DistanceInCartesianCoordinates(X1, Y1, X2, Y2), width));
	double angle_with_y_axis_up = FigureOutAngleWithInventeredYInRad(X1, Y1, X2, Y2) + DEGREE90_IN_RAD;
	line.setPosition(X1 + ((double)width / 2)* abs(cos(angle_with_y_axis_up)) * SignMod(sin(angle_with_y_axis_up - DEGREE90_IN_RAD)), Y1 - ((double)width / 2) * sin(angle_with_y_axis_up));
	line.rotate(RadiansToDegree(angle_with_y_axis_up) - 90);
	line.setOutlineColor(color);
	line.setFillColor(color);
	window->draw(line);
}
void DrawLine(double X1, double Y1, double X2, double Y2, int width, double angle_with_y_axis_up, sf::Color color, sf::RenderWindow *window)
{
	sf::RectangleShape line(sf::Vector2f(DistanceInCartesianCoordinates(X1, Y1, X2, Y2), width));
	
	line.setPosition(X1 + ((double)width / 2)* abs(cos(angle_with_y_axis_up)) * SignMod(sin(angle_with_y_axis_up - DEGREE90_IN_RAD)), Y1 - ((double)width / 2) * sin(angle_with_y_axis_up));
	line.rotate(RadiansToDegree(angle_with_y_axis_up) - 90);
	line.setOutlineColor(color);
	line.setFillColor(color);
	window->draw(line);
}
void DrawLine(double X1, double Y1, double length, int width, sf::Color color, sf::RenderWindow *window, double angle_with_y_axis_up)
{
	sf::RectangleShape line(sf::Vector2f(length, width));

	line.setPosition(X1 + ((double)width / 2)* abs(cos(angle_with_y_axis_up)) * SignMod(sin(angle_with_y_axis_up - DEGREE90_IN_RAD)), Y1 - ((double)width / 2) * sin(angle_with_y_axis_up));
	line.rotate(RadiansToDegree(angle_with_y_axis_up) - 90);
	line.setOutlineColor(color);
	line.setFillColor(color);
	window->draw(line);
}
void DrawHex(int X, int Y, int R, sf::Color Color, sf::RenderWindow & window)
{
	sf::Vertex HEX[]
	{
		sf::Vertex(sf::Vector2f(X + R * COS_30, Y - R * SIN_30), Color),
		sf::Vertex(sf::Vector2f(X + R * COS_30, Y + R * SIN_30), Color),
		sf::Vertex(sf::Vector2f(X, Y + R), Color),
		sf::Vertex(sf::Vector2f(X - R * COS_30, Y + R * SIN_30), Color),
		sf::Vertex(sf::Vector2f(X - R * COS_30, Y - R * SIN_30), Color),
		sf::Vertex(sf::Vector2f(X, Y - R), Color),
		sf::Vertex(sf::Vector2f(X + R * COS_30, Y - R * SIN_30), Color),

	};
	window.draw(HEX, 7, sf::LineStrip);
}
void DrawHex(int X, int Y, int R, sf::Color &Color, sf::RenderWindow & window)
{
	sf::Vertex HEX[]
	{
		sf::Vertex(sf::Vector2f(X + R * COS_30, Y - R * SIN_30), Color),
		sf::Vertex(sf::Vector2f(X + R * COS_30, Y + R * SIN_30), Color),
		sf::Vertex(sf::Vector2f(X, Y + R), Color),
		sf::Vertex(sf::Vector2f(X - R * COS_30, Y + R * SIN_30), Color),
		sf::Vertex(sf::Vector2f(X - R * COS_30, Y - R * SIN_30), Color),
		sf::Vertex(sf::Vector2f(X, Y - R), Color),
		sf::Vertex(sf::Vector2f(X + R * COS_30, Y - R * SIN_30), Color),

	};
	window.draw(HEX, 7, sf::LineStrip);
}
void DrawHex(int &X, int &Y, int &R, sf::Color &Color, sf::RenderWindow & window)
{
	sf::Vertex HEX[]
	{
		sf::Vertex(sf::Vector2f(X + R * COS_30, Y - R * SIN_30), Color),
		sf::Vertex(sf::Vector2f(X + R * COS_30, Y + R * SIN_30), Color),
		sf::Vertex(sf::Vector2f(X, Y + R), Color),
		sf::Vertex(sf::Vector2f(X - R * COS_30, Y + R * SIN_30), Color),
		sf::Vertex(sf::Vector2f(X - R * COS_30, Y - R * SIN_30), Color),
		sf::Vertex(sf::Vector2f(X, Y - R), Color),
		sf::Vertex(sf::Vector2f(X + R * COS_30, Y - R * SIN_30), Color),

	};
	window.draw(HEX, 7, sf::LineStrip);
}
void DrawHex(int X, int Y, int R, sf::Color Color, sf::RenderWindow * window)
{
	sf::Vertex HEX[]
	{
		sf::Vertex(sf::Vector2f(X + R * COS_30, Y - R * SIN_30), Color),
		sf::Vertex(sf::Vector2f(X + R * COS_30, Y + R * SIN_30), Color),
		sf::Vertex(sf::Vector2f(X, Y + R), Color),
		sf::Vertex(sf::Vector2f(X - R * COS_30, Y + R * SIN_30), Color),
		sf::Vertex(sf::Vector2f(X - R * COS_30, Y - R * SIN_30), Color),
		sf::Vertex(sf::Vector2f(X, Y - R), Color),
		sf::Vertex(sf::Vector2f(X + R * COS_30, Y - R * SIN_30), Color),

	};
	window->draw(HEX, 7, sf::LineStrip);
}
//рисует стрелку, собранную из линий.
void Buf_drawNarrowArrow(sf::RenderWindow * window, int Xbeginig, int Ybegining, int Xend, int Yend, int length_of_arrowhead_lines, sf::Color color, double degree_angle_between_arrow_and_arrowhead)
{
	DrawLine(Xbeginig, Ybegining, Xend, Yend, color, window);
	std::array<Point, 2> arrowhead_lines_ends = figureOutVertexesOfLineMakesGivenInsideAngleWithGivenLine(Point(Xbeginig, Ybegining), Point(Xend, Yend), degree_angle_between_arrow_and_arrowhead, length_of_arrowhead_lines);
	DrawLine(Xend, Yend, arrowhead_lines_ends[0].X, arrowhead_lines_ends[0].Y, color, window);
	DrawLine(Xend, Yend, arrowhead_lines_ends[1].X, arrowhead_lines_ends[1].Y, color, window);

}
