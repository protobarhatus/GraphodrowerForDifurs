#pragma once
#include<vector>
#include<memory>
#include <string>
#include"SFML/Graphics.hpp"

#include"SFML Graphic Functions.h"
#include"..\Math and data works\Math.h"

#include<ctime>
class ElementDrawer;
class EventController;

class FormElement
{
protected:

	int X, Y;
	int id;
	bool visible = true;
	friend ElementDrawer;
	ElementDrawer * drawer;
	void SetID(int ID);
	virtual void Draw(sf::RenderWindow * window) = 0;
public:
	virtual void SetPosition(int x, int y) = 0;
	virtual void SetX(int x) = 0;
	virtual void SetY(int y) = 0;
	void Show();
	void Hide();
	~FormElement();
	FormElement(ElementDrawer * drawer_object);
};
class FormElementTextOutputComponent
{
private:
	sf::Text text;
	sf::Font font;
	static std::string font_folder;
	
public:
	//this is static parameter
	void SetFontFolder(std::string folder);
	void SetString(std::string str);
	void SetFont(sf::Font & f);
	//font_file will be added to font_folder that have to be already determined
	void SetFont(std::string font_file);
	void SetFontSize(int size_in_pixels);

	void SetFillColor(sf::Color color);
	void SetPosition(int x, int y);
	void SetX(int x);
	void SetY(int y);
	void Draw(sf::RenderWindow * window);
};
class Label : public FormElement
{
private:
	FormElementTextOutputComponent text_output_component;
	virtual void Draw(sf::RenderWindow * window) override;
	sf::RectangleShape fone;
public:

	virtual void SetPosition(int x, int y) override;
	virtual void SetX(int x) override;
	virtual void SetY(int y) override;
	//this is static parameter
	void SetFontFolder(std::string folder);
	Label(ElementDrawer * drawer_object);
	void SetString(std::string str);
	void SetFont(sf::Font & f);
	//font_file will be added to font_folder that have to be already determined
	void SetFont(std::string font_file);
	void SetFontSize(int size_in_pixels);
	
	void SetFillColor(sf::Color color);
};
class EventFormElement : public FormElement
{
private:
	
protected:
	friend EventController;
	EventController * event_controller;
	int event_id;
	void SetEventID(int i);
	virtual void Draw(sf::RenderWindow * window) = 0;
	void * params;
	virtual void GetEvent(sf::Event & event, bool is_click, Point last_mouse_position) = 0;
	
public:
	virtual void SetPosition(int x, int y) = 0;
	virtual void SetX(int x) = 0;
	virtual void SetY(int y) = 0;
	void SetEventParams(void * p);
	EventFormElement(ElementDrawer * drawer_obj, EventController * event_obj);
	~EventFormElement();
};

class Button : public EventFormElement
{
private:
	int Width;
	int Height;
	void(*ClickEventFunction)(sf::Event & event, void * parametres);
	MImage image;
	virtual void Draw(sf::RenderWindow * window) override;
	virtual void GetEvent(sf::Event & event, bool is_click, Point last_mouse_position) override;
	bool isPointInClickZone(int X, int Y);
public:
	void SetImage(MImage & image);
	void SetImage(MImage && image);
	virtual void SetPosition(int x, int y) override;
	virtual void SetX(int x) override;
	virtual void SetY(int y) override;
	void SetClickEventFunction(void(*pClickEventFunction)(sf::Event & event, void * par));
	Button(ElementDrawer * drawer_obj, EventController * event_obj);
};
class TextBox : public EventFormElement
{
private:
	int Width;
	int amount_of_lines;
	bool is_onelined;
	bool focus;
};




//Class, responsible for drawing all elements. 
class ElementDrawer
{
private:
	std::vector< FormElement*> elements;
	int elements_counter = 0;
	friend FormElement;
	void AddElement(FormElement * element);
	void dDeleteElement(int id);
	sf::RenderWindow * window;
public:
	ElementDrawer();
	~ElementDrawer();
	//You have to call this function after all clearing form if you want formelements to be displayed
	void Redraw();
	void SetWindow(sf::RenderWindow * win);
};
class EventController
{
private:
	const int time_between_events_in_click = 100;
	std::vector<EventFormElement *> elements;
	int elements_counter = 0;
	friend EventFormElement;
	void AddElement(EventFormElement * element);
	void DeletefElement(int id);
	time_t time = 0;
	void RememberMousePressing();
	//call this functions after getting mouseButtonReleased event to discern mouseclick and mousemoving
	bool isClick();
	Point mouse_last_position;
	void RememberMousePosition(sf::Event & event);
public:
	EventController();
	~EventController();
	void GetEvent(sf::Event & event);
};
