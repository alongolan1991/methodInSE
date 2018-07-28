#include "Button.h"

Button::Button(Action* act, std::string _text, short x_pos, short y_pos, int w, int h, BorderType border, Color tColor, Color bColor, UIComponent * parent)
	: actioner(act), Label(_text, x_pos, y_pos, w, h, border, tColor, bColor, parent) {
}

Button::~Button() {
	CCTRL.detachObserver(this);
}

void Button::click() const {

	if(actioner)
		actioner->action();
}

void Button::mouseClicked(MOUSE_EVENT_RECORD) {
	click();
}
