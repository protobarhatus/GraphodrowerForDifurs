#pragma once

#include"SFML\Graphics.hpp"
#include"Math.h"

class MImage
{
	sf::Texture texture;
	//image для возможности изменять объект
	sf::Image image;
	sf::RectangleShape sprite;
	sf::RenderWindow * window;
	
public:
	MImage(sf::Image itexture);
	MImage();
	MImage(sf::Image itexture, sf::RenderWindow * iwindow);
	MImage(std::string file_name);
	MImage(MImage & copy);
	MImage(MImage && copy);
	MImage operator=(MImage &copy);
	MImage operator=(const MImage &copy);
	MImage operator=(MImage &&copy);
	void Draw(int X, int Y) { sprite.setPosition(X, Y); window->draw(sprite); }
	void Draw(int X, int Y, sf::RenderWindow * iwindow) { sprite.setPosition(X, Y);
	iwindow->draw(sprite); }
	void Draw() { window->draw(sprite); }
	void Draw(sf::RenderWindow * win) { win->draw(sprite); }
	void SetPosition(int X, int Y) { sprite.setPosition(X, Y); }

	void SetTexture(sf::Image & _image);

	void loadFromFile(std::string file_name);
	void setSmooth(bool smooth_level) { texture.setSmooth(smooth_level); }
	int GetWidth() { return this->image.getSize().x; }
	int GetHeight() { return this->image.getSize().y; }
	int GetWidth() const { return this->image.getSize().x; }
	int GetHeight() const { return this->image.getSize().y; }
	void RepaintColor(sf::Color color_to_fill, sf::Color color_to_change = sf::Color::Color(0, 0, 0, 1));
	void RepaintColorWithSavingAlfaComponent(sf::Color color_to_fill, sf::Color color_to_change = sf::Color::Color(0, 0, 0, 1));
	void RepaintByDivision(sf::Color multiplier, sf::Color divider);
	void Rotate(float angle) { sprite.rotate(angle); }
	void SetRotation(float angle) { sprite.setRotation(angle); }
	//получение указателя на image для изменения объекта. следует осторожно работать с ним. после окончания изменения рисунка, следует обновить его функцией update
	sf::Image * GetImage();
	void Update();
};

void ChangeColorByDivision(sf::Image & image, sf::Color multiplier, sf::Color divider);
sf::CircleShape PutUnderTextureCircle(sf::Texture & texture, sf::Color color, int radius, int OutLineThickness, sf::Color OutLineColor = sf::Color::Black);
void ChangeColor(sf::Image * texture, sf::Color color_to_fill, sf::Color color_to_change = sf::Color::Color(0, 0, 0, 1));
void BUF_DrawString(std::string str, int X, int Y, int fontsize_in_pixels, sf::RenderWindow * window, sf::Color font_color, std::string font);
void BUF_DrawString(std::string str, int X, int Y, int fontsize_in_pixels, sf::RenderWindow * window, sf::Color font_color, sf::Text::Style style, std::string font);
void BUF_DrawString(std::string str, int X, int Y, int fontsize_in_pixels, sf::RenderWindow * window, sf::Color font_color, sf::Text::Style style, sf::Font font);
void BUF_DrawString(std::string str, int X, int Y, int fontsize_in_pixels, sf::RenderWindow * window, sf::Color font_color, sf::Font font);
void BUF_DrawString(std::string str, int X, int Y, int fontsize_in_pixels, sf::RenderTarget * window, sf::Color font_color, std::string font);
void BUF_DrawString(std::string str, int X, int Y, int fontsize_in_pixels, sf::RenderTarget * window, sf::Color font_color, sf::Text::Style style, std::string font);
void BUF_DrawString(std::string str, int X, int Y, int fontsize_in_pixels, sf::RenderTarget * window, sf::Color font_color, sf::Text::Style style, sf::Font font);
void BUF_DrawString(std::string str, int X, int Y, int fontsize_in_pixels, sf::RenderTarget * window, sf::Color font_color, sf::Font font);
bool areEqualWithoutAlfaComponent(sf::Color a, sf::Color b);
void PutCircleUnderImage(MImage & image, sf::Color circle_color, int radius, int outline_thickness, sf::Color outline_color = sf::Color::Black);
MImage  PutCircleUnderImageAndReturnIt(MImage image, sf::Color circle_color, int radius, int outline_thickness, sf::Color outline_color = sf::Color::Black);
void DrawLine(int X1, int Y1, int X2, int Y2, sf::Color Color, sf::RenderWindow &window);
void DrawLine(int X1, int Y1, int X2, int Y2, sf::Color Color, sf::RenderWindow *window);
void DrawHex(int X, int Y, int R, sf::Color Color, sf::RenderWindow & window);
void DrawHex(int X, int Y, int R, sf::Color &Color, sf::RenderWindow & window);
void DrawHex(int &X, int &Y, int &R, sf::Color &Color, sf::RenderWindow & window);
void Buf_drawNarrowArrow(sf::RenderWindow * window, int Xbeginig, int Ybegining, int Xend, int Yend, int length_of_arrowhead_lines, sf::Color color, double degree_angle_between_arrow_and_arrowhead);
void DrawHex(int X, int Y, int R, sf::Color Color, sf::RenderWindow * window);
void DrawLine(double X1, double Y1, double X2, double Y2, int width, sf::Color color, sf::RenderWindow *window);
void DrawLine(double X1, double Y1, double X2, double Y2, int width, double angle_with_y_axis_up, sf::Color color, sf::RenderWindow *window);
void DrawLine(double X1, double Y1, double length, int width, sf::Color color, sf::RenderWindow *window, double angle_with_y_axis_up);

void drawString(std::wstring str, int X, int Y, int fontsize_in_pixels, sf::RenderWindow * window, sf::Color font_color = sf::Color::Black, std::string font = "Microsoft Sans Serif.ttf");
void drawString(std::string str, int X, int Y, int fontsize_in_pixels, sf::RenderWindow * window, sf::Color font_color = sf::Color::Black, std::string font = "Microsoft Sans Serif.ttf");