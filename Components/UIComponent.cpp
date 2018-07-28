#include "UIComponent.h"

UIComponent::UIComponent(short x_pos, short y_pos, int w, int h, BorderType border, Color tColor, Color bColor, UIComponent *parent):
position{ x_pos, y_pos }, width(w), height(h), borderType(border),
textColor(tColor), backgroundColor(bColor), focus(false) {}

UIComponent & UIComponent::getRoot() {
	UIComponent *result = this;
	while (result->parent) {
		result = result->parent;
	}
	return *result;
}

void UIComponent::removeFromScreen() const {
	ConsoleController ctrl = CCTRL;
	//SetConsoleCursorPosition(h, position);
	ctrl.setPosition(position);
	//SetConsoleTextAttribute(h, 0);

	// FIXME: this assumes the UI is black -Yftah
	ctrl.setColors(0, 0);
	COORD c = { position.X, position.Y };
	for (short i = 0; i < height + 1; i++) {
		for (short j = 0; j < width + 2; j++) {
			std::cout << " ";
		}
		//SetConsoleCursorPosition(h, { c.X, ++c.Y });
		++c.Y;
		ctrl.setPosition(c);
	}
}

void UIComponent::draw() {

	drawBorder();
}

void UIComponent::setVisible(bool visible) {
	bool prev = this->is_visible;
	this->is_visible = visible;
	if (visible) {
		if (this->isInteractable() && !prev) {
			CCTRL.attachObserver(this);
		}
		draw();
	}
	else {
		if (this->isInteractable() && prev) {
			CCTRL.detachObserver(this);
		}
		removeFromScreen();
	}
}

void UIComponent::invertColors() {
	Color tmp = this->backgroundColor;
	bool itmp = fg_intensity;
	this->backgroundColor = this->textColor;
	this->textColor = tmp;
	this->fg_intensity = bg_intensity;
	this->bg_intensity = itmp;
}

void UIComponent::applyColors() {
	CCTRL.setColors(this->textColor, this->backgroundColor, fg_intensity, bg_intensity);
}

void UIComponent::setBorderType(BorderType border) {
	borderType = border; 
	
	if (is_visible == true)
		draw();
}

void UIComponent::setWidth(int _width) {
	if (_width >= 0) { 
		if (is_visible == true) 
			removeFromScreen(); 

		width = _width; 

		if (is_visible == true)
			draw(); 
	}
}

void UIComponent::setHeight(int _height) {
	if (_height >= 0) { 
		if (is_visible == true)
			removeFromScreen(); 
		
		height = _height;

		if (is_visible == true)
			draw(); }
}

void UIComponent::setPosition(short _x, short _y, bool special) {
	if (_x >= 0 && _y >= 0) {
		if (is_visible == true) 
			removeFromScreen();

		position = { _x, _y }; 
		
		if (is_visible == true)
			draw(); 
	}
}

void UIComponent::setTextColor(Color color) {
	textColor = color; 

	if (is_visible == true)
		draw();
}

void UIComponent::setBackgroundColor(Color color) {
	backgroundColor = color; 
	
	if (is_visible == true)
		draw();
}

void UIComponent::drawBorder() const {
	// SetConsoleCursorPosition(h, position);
	ConsoleController ctrl = CCTRL;
	ctrl.setPosition(position);
	// SetConsoleTextAttribute(h, textColor | FOREGROUND_INTENSITY | backgroundColor * 16);
	//ctrl.setColors(textColor, backgroundColor, fg_intensity, bg_intensity);
	short i;
	COORD c;
	BorderCharacters bc;
	switch (borderType) {
	case Dotted:
		bc = { '\xDA', '\xBF', '\xC0', '\xD9', '-', '|' };
		break;
	case Solid:
		bc = { '\xDA', '\xBF', '\xC0','\xD9', '\xC4', '\xB3' };
		break;
	case Double:
		bc = { '\xC9', '\xBB', '\xC8','\xBC', '\xCD', '\xBA' };
		break;
	case None: return;
	default: return;
	}

	std::cout << bc.leftTopCorner;

	for (i = 0; i < width; i++)
		std::cout << bc.horizontal;

	std::cout << bc.rightTopCorner;

	for (i = 1; i < height; i++) {
		c = { position.X, position.Y + i };
		//SetConsoleCursorPosition(h, c);
		ctrl.setPosition(c);
		std::cout << bc.vertical;
		c = { position.X + width + 1, position.Y + i };
		//SetConsoleCursorPosition(h, c);
		ctrl.setPosition(c);
		std::cout << bc.vertical;
	}

	c = { position.X, position.Y + i };
	//SetConsoleCursorPosition(h, c);
	ctrl.setPosition(c);
	std::cout << bc.leftBottomCorner;
	for (i = 0; i < width; i++) {
		std::cout << bc.horizontal;
	}
	std::cout << bc.rightBottomCorner;
}