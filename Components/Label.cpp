#include "Label.h"
#include <iostream>

Label::Label(std::string _text, short x_pos, short y_pos, short w, short h, BorderType border,
	Color tColor, Color bColor, UIComponent *parent) : UIComponent(x_pos, y_pos, w, h, border, tColor, bColor), text(_text) {
	if (text.length() > width) text = text.substr(0, width);
}


void Label::draw() {
	applyColors();
	UIComponent::draw();
	ConsoleController ctrl = CCTRL;
	// clear background
	COORD c = { position.X + 1, position.Y + 1 };
	short text_len = static_cast<short>(text.length());
	ctrl.setPosition(c);
	//GFX.moveTo(c.X, c.Y);
	for (short i = 0; i < height; i++) {
		for (short j = 0; j < width; j++) {
			std::cout << " ";
		}
		ctrl.setPosition({ position.X + 1, position.Y + i + 1});
	}

	// print centered text
	c = { position.X + 1 + width / 2 - text_len / 2, position.Y + 1 };
	ctrl.setPosition(c);
	//GFX.moveTo(c.X, c.Y);
	std::cout << text;

	postDraw();
}

void Label::setText(std::string _text) {
	text = _text;
	if (text.length() > width) text = text.substr(0, width);
	draw();
}