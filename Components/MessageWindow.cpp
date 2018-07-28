#include "MessageWindow.h"


MessageWindow::MessageWindow(std::string _text, short x_pos, short y_pos, short w, short h,
	BorderType border, Color tColor, Color bColor, UIComponent *parent) 
	: UIComponent(x_pos, y_pos, w, h, border, tColor, bColor), text(_text),
	okBtn(&okAction, "OK", x_pos + w/6, y_pos + h - 3, w / 6, 2, Solid, Orange, Blue),
	cancelBtn(&cancelAction, "Cancel", x_pos + w -w/3, y_pos + h - 3, w / 6, 2, Solid, Orange, Blue),
	okAction(*this), cancelAction(*this) {

	//CCTRL.attachObserver(&okBtn);
	//CCTRL.attachObserver(&cancelBtn);
}

void MessageWindow::draw() {
	applyColors();
	UIComponent::draw();
	ConsoleController ctrl = CCTRL;
	COORD c = { position.X + 1, position.Y + 1 };
	short text_len = static_cast<short>(text.length());
	ctrl.setPosition(c);
	for (short i = 0; i < height; i++) {
		for (short j = 0; j < width; j++) {
			std::cout << " ";
		}
		ctrl.setPosition({ position.X + 1, position.Y + i + 1 });
	}

	// print centered text
	c = { position.X + 1 + width / 2 - text_len / 2, position.Y + 1 };
	ctrl.setPosition(c);
	std::cout << text;

	okBtn.draw();
	cancelBtn.draw();

	postDraw();
}

void MessageWindow::setVisible(bool visible) {
	this->is_visible = visible;
	if (visible) {
		if (this->isInteractable()) {
			CCTRL.attachObserver(&okBtn);
			CCTRL.attachObserver(&cancelBtn);
		}
		draw();
	}
	else {
		if (this->isInteractable()) {
			CCTRL.detachObserver(&okBtn);
			CCTRL.detachObserver(&cancelBtn);
		}
		removeFromScreen();
	}
}

void MessageWindow::setPosition(short pos_x, short pos_y, bool special) {
	this->position = { pos_x, pos_y };
	okBtn.setPosition(pos_x + width / 6, pos_y + height - 3, pos_y);
	cancelBtn.setPosition(pos_x + width - width / 3, pos_y + height - 3);
}
