#include"My SFML Forms.h"
#include "../MyLib.h"
void Label::SetPosition(int x, int y)
{
	this->X = x;
	this->Y = y;
	this->text_output_component.SetPosition(this->X, this->Y);
	this->fone.setPosition(this->X, this->Y);
}
void FormElementTextOutputComponent::SetPosition(int x, int y)
{
	
	this->text.setPosition(x, y);
}
ElementDrawer::ElementDrawer()
{
	this->elements_counter = 0;
}
ElementDrawer::~ElementDrawer()
{
	
}
void FormElementTextOutputComponent::SetX(int x)
{
	this->text.setPosition(x, this->text.getPosition().y);
	
}
void FormElementTextOutputComponent::SetY(int y)
{
	this->text.setPosition(this->text.getPosition().x, y);
}
void Label::SetX(int x)
{
	this->X = x;
	this->text_output_component.SetPosition(this->X, this->Y);
	this->fone.setPosition(this->X, this->Y);
}
void Label::SetY(int y)
{
	this->Y = y;
	this->text_output_component.SetPosition(this->X, this->Y);
	this->fone.setPosition(this->X, this->Y);
}
FormElement::FormElement(ElementDrawer * drawer_object)
{
	
	
	this->drawer = drawer_object;
	drawer->AddElement(this);
}
FormElement::~FormElement()
{
	this->drawer->dDeleteElement(this->id);
	
}
void ElementDrawer::AddElement(FormElement * element)
{
	element->SetID(this->elements_counter);
	++this->elements_counter;
	this->elements.push_back(element);
}

void ElementDrawer::dDeleteElement(int id)
{
	DeleteElement<FormElement*>(this->elements, id);
	
	--this->elements_counter;
}
void FormElement::SetID(int ID)
{
	this->id = ID;
}
void ElementDrawer::Redraw()
{
	for (int i = 0; i < this->elements.size(); ++i)
	{
		if (this->elements[i]->visible)
			this->elements[i]->Draw(this->window);
	}
}
void FormElement::Show()
{
	this->visible = true;
	this->drawer->Redraw();
}
void FormElement::Hide()
{
	this->visible = false;
	this->drawer->Redraw();
}
void ElementDrawer::SetWindow(sf::RenderWindow * win)
{
	this->window = win;
}
void FormElementTextOutputComponent::SetFontFolder(std::string folder)
{
	this->font_folder = folder;
}
void Label::SetFontFolder(std::string folder)
{
	this->text_output_component.SetFontFolder(folder);
}
Label::Label(ElementDrawer * drawer_object) : FormElement(drawer_object)
{
	
	this->text_output_component.SetFillColor(sf::Color::Black);
	
}
void Label::SetString(std::string str)
{
	this->text_output_component.SetString(str);
}
void FormElementTextOutputComponent::SetString(std::string str)
{
	this->text.setString(str);
}
void Label::SetFont(sf::Font & f)
{
	this->text_output_component.SetFont(f);
}
void FormElementTextOutputComponent::SetFont(sf::Font & f)
{
	this->font = f;
}
void Label::SetFont(std::string font_file)
{
	this->text_output_component.SetFont(font_file);
}
void FormElementTextOutputComponent::SetFont(std::string font_file)
{
	if (!this->font.loadFromFile(this->font_folder + font_file))
	{
		throw ("Something wrong with font file " + this->font_folder + font_file + " or font file is incorrect.");
	}
	this->text.setFont(this->font);
}
void Label::SetFontSize(int size_in_pixels)
{
	this->text_output_component.SetFontSize(size_in_pixels);
}
void FormElementTextOutputComponent::SetFontSize(int size_in_pixels)
{
	this->text.setCharacterSize(size_in_pixels);
}
void FormElementTextOutputComponent::SetFillColor(sf::Color color)
{
	this->text.setFillColor(color);
}
void Label::SetFillColor(sf::Color color)
{
	this->text_output_component.SetFillColor(color);
}
void Label::Draw(sf::RenderWindow * window)
{
	this->text_output_component.Draw(window);
}
void FormElementTextOutputComponent::Draw(sf::RenderWindow * window)
{
	window->draw(this->text);
}
void EventFormElement::SetEventID(int i)
{
	this->event_id = i;
}
EventFormElement::EventFormElement(ElementDrawer * drawer_obj, EventController * event_obj) : FormElement(drawer_obj)
{
	this->event_controller = event_obj;
	this->event_controller->AddElement(this);
}
EventFormElement::~EventFormElement()
{
	this->event_controller->DeletefElement(this->event_id);
}
void EventController::AddElement(EventFormElement * element)
{
	this->elements.push_back(element);
	element->SetID(this->elements_counter);
	++this->elements_counter;
}
void EventController::DeletefElement(int id)
{
	DeleteElement<EventFormElement*>(this->elements, id);
	--this->elements_counter;
}
EventController::EventController()
{
	this->elements_counter = 0;
}
EventController::~EventController()
{

}
void EventController::RememberMousePosition(sf::Event & event)
{
	this->mouse_last_position = Point(event.mouseButton.x, event.mouseButton.y);
}
void EventController::GetEvent(sf::Event & event)
{
	bool is_click = false;
	if (event.type == sf::Event::MouseButtonPressed)
	{
		this->RememberMousePressing();
		this->RememberMousePosition(event);
	}
	if (event.type == sf::Event::MouseButtonReleased)
	{
		is_click = this->isClick();
	}
	for (int i = 0; i < this->elements_counter; ++i)
	{
		this->elements[i]->GetEvent(event, is_click, this->mouse_last_position);
	}
}
void EventFormElement::SetEventParams(void * p)
{
	this->params = p;
}
void Button::SetImage(MImage & im)
{
	this->image = im;
	this->Width = im.GetWidth();
	this->Height = im.GetHeight();
}
void Button::SetImage(MImage && im)
{
	this->image = im;
	this->Width = im.GetWidth();
	this->Height = im.GetHeight();
}
void Button::Draw(sf::RenderWindow * window)
{
	this->image.Draw(window);
}
bool Button::isPointInClickZone(int x, int y)
{
	return x > this->X && x < this->X + this->Width && y > this->Y && y < this->Y + this->Height;
}
void Button::GetEvent(sf::Event & event, bool is_click, Point last_mouse_position)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		
		return;
	}
	if (event.type == sf::Event::MouseButtonReleased && isPointInClickZone(event.mouseButton.x, event.mouseButton.y))
	{
		if (is_click)
			this->ClickEventFunction(event, this->params);
	}
}
void Button::SetPosition(int x, int y)
{
	this->X = x;
	this->Y = y;
	this->image.SetPosition(this->X, this->Y);
}
void Button::SetX(int x)
{
	this->X = x;
	this->image.SetPosition(this->X, this->Y);
}
void Button::SetY(int y)
{
	this->Y = y;
	this->image.SetPosition(this->X, this->Y);
}
Button::Button(ElementDrawer * drawer_obj, EventController * event_obj) : EventFormElement(drawer_obj, event_obj)
{
	this->X = 0;
	this->Y = 0;
	this->Width = 0;
	this->Height = 0;
}
void Button::SetClickEventFunction(void(*pClickEventFunction)(sf::Event & event, void * par))
{
	this->ClickEventFunction = pClickEventFunction;
}
void EventController::RememberMousePressing()
{
	this->time = clock();
}

bool EventController::isClick()
{
	
	if (clock() - this->time <= this->time_between_events_in_click)
		return true;
	return false;
}
