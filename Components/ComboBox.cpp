#include "ComboBox.h"
#include <iostream>
using namespace std;

ComboBox::ComboBox(ComboBox * o) : UIComponent(o->position.X, o->position.Y, o->width, 2, o->borderType, o->textColor, o->backgroundColor, o->parent), _originalState(NULL), options(NULL), selected_index(-1), open_down(o->open_down), open(false) {}

void ComboBox::calcOpenDirection() {
	if (parent == NULL)
		open_down = CCTRL.getConsoleSize().Y > position.Y + this->options.size() + 1;
	else {
		open_down = parent->getYPosition() + parent->getHeight() > position.Y + this->options.size() + 3;
		int list_start_position = position.Y - (this->options.size() + 1);
		bool cannot_open_up = parent->getYPosition() >= list_start_position;
		if (!open_down && cannot_open_up) {
			open_down = true;
		}
	}
}

ComboBox::ComboBox(string* options, int len, short pos_x, short pos_y, short width, BorderType border, Color tColor, Color bColor, UIComponent* parent) : UIComponent(pos_x, pos_y, width, 2, border, tColor, bColor, parent) ,options(vector<string>()), selected_index(-1), open(false) {
	for (int i = 0; i < len; ++i) {
		this->options.push_back(options[i]);
	}
	//open_down = CCTRL.getConsoleSize().Y > position.Y + this->options.size() + 1;
	calcOpenDirection();
	_originalState = new ComboBox(this);
	bg_intensity = false;
	//CCTRL.attachObserver(this);
}

void ComboBox::draw() {
	applyColors();
	ConsoleController& ctrl = CCTRL;
	if (open) {
		if (open_down) {
			// Box opens downwards
			ctrl.setPosition({ position.X + 1, position.Y + 1 });
			if (selected_index != -1)
				cout << this->options.at(selected_index).substr(0, width - 3);
			ctrl.setPosition({ position.X + 1, position.Y + 2 });
			for (int i = width; i > 0; --i) putchar('-');
			for (vector<string>::iterator it = options.begin(); it != options.end(); ++it) {
				if (it - options.begin() == temp_index) invertColors(), applyColors();
				ctrl.setPosition({ position.X + 1, position.Y + 3 + static_cast<short>(it - options.begin()) });
				for (int i = width; i > 0; --i) putchar(' ');
				ctrl.setPosition({ position.X + 1, position.Y + 3 + static_cast<short>(it - options.begin()) });
				cout << it->substr(0, width);
				if (it - options.begin() == temp_index) invertColors(), applyColors();
			}
			ctrl.setPosition({ this->getXPosition() + width - 1 , this->getYPosition() + 1 });
		} else {
			// Box opens upwards
			for (vector<string>::iterator it = options.begin(); it != options.end(); ++it) {
				if (it - options.begin() == temp_index) invertColors(), applyColors();
				ctrl.setPosition({ position.X + 1, position.Y + 1 + static_cast<short>(it - options.begin())});
				for (int i = width; i > 0; --i) putchar(' ');
				ctrl.setPosition({ position.X + 1, position.Y + 1 + static_cast<short>(it - options.begin()) });
				cout << it->substr(0, width);
				if (it - options.begin() == temp_index) invertColors(), applyColors();

			}
			ctrl.setPosition({ position.X + 1, position.Y + height - 2 });
			for (int i = width; i > 0; --i) putchar('-');
			ctrl.setPosition({ position.X + 1, position.Y + height - 1 });
			if (selected_index != -1)
				cout << this->options.at(selected_index).substr(0, width - 3);
			ctrl.setPosition({ this->getXPosition() + width - 1 , this->getYPosition() + height - 1 });
		}
		
	} else {
		ctrl.setPosition({ this->getXPosition() + 1 , this->getYPosition() + 1 });
		for (int i = width; i > 0; --i) putchar(' ');
		ctrl.setPosition({ this->getXPosition() + 1 , this->getYPosition() + 1 });
		if (selected_index != -1)
			cout << this->options.at(selected_index).substr(0, width - 3);
		ctrl.setPosition({ this->getXPosition() + width - 1 , this->getYPosition() + 1 });
	}
	UIComponent::draw();
	postDraw();
}

void ComboBox::setPosition(short pos_x, short pos_y, bool special) {
	this->position.X = pos_x;
	this->position.Y = pos_y;
	calcOpenDirection();
	if (special)
		this->_originalState->position = position;;
}


void ComboBox::mouseClicked(MOUSE_EVENT_RECORD e) {
	//open_down = CCTRL.getConsoleSize().Y > position.Y + options.size() + 1;
	setFocus(true);
	if (open) {
		if (CCTRL.isIntersects(e.dwMousePosition, this->_originalState)) {
			if (e.dwMousePosition.X > this->_originalState->position.X
				&& e.dwMousePosition.X < this->_originalState->position.X + this->_originalState->width
				&& e.dwMousePosition.Y > this->_originalState->position.Y
				&& e.dwMousePosition.Y < this->_originalState->position.Y + this->_originalState->height) {
				this->toggle();
			}
		} else if (CCTRL.isIntersects(e.dwMousePosition, this) 
			&& e.dwMousePosition.X > position.X 
			&& e.dwMousePosition.X < position.X + width
			&& e.dwMousePosition.Y > position.Y
			&& e.dwMousePosition.Y < position.Y + height) {
			if (open_down) {
				selected_index = e.dwMousePosition.Y - position.Y - 3;
			} else {
				selected_index = e.dwMousePosition.Y - position.Y - 1;
			}
			this->toggle();
		}
	}
	else {
		if (CCTRL.isIntersects(e.dwMousePosition, this)
			&& e.dwMousePosition.X > position.X
			&& e.dwMousePosition.X <= position.X + width
			&& e.dwMousePosition.Y >= position.Y
			&& e.dwMousePosition.Y <= position.Y + height) {
			this->toggle();
		}
	}
}

void ComboBox::keyPressed(KEY_EVENT_RECORD e) {
	switch (e.wVirtualKeyCode) {
	case VK_UP:
		if (this->open && this->temp_index > 0)
			this->temp_index--;
		break;
	case VK_DOWN:
		if (this->open && this->temp_index + 1 < this->options.size())
			this->temp_index++;
		break;
	case VK_RETURN:
	case VK_SPACE:
		if (this->open && this->temp_index > -1) {
			this->selected_index = this->temp_index;
			this->toggle();
		}
		break;
	default:
		break;
	}
	this->draw();
}

void ComboBox::toggle() {
	open = !open;
	if (open) {
		temp_index = selected_index;
		height += options.size() + 1;
		if (!open_down) {
			position.Y = position.Y - options.size() - 1;
		}
		this->draw();
	} else {
		height = _originalState->height;
		position = _originalState->position;
		if (open_down) {
			for (short i = 0; i < options.size() + 1; ++i) {
				CCTRL.setPosition({ position.X, position.Y + 3 + i });
				for (short j = width + 2; j > 0; --j) putchar(' ');
			}
		} else {
			for (short i = 0; i < options.size() + 1; ++i) {
				CCTRL.setPosition({ position.X, position.Y - static_cast<short>(options.size()) - 1 + i });
				for (short j = width + 2; j > 0; --j) putchar(' ');
			}
		}
		this->draw();

		UIComponent *curr = NULL;
		vector<UIComponent*> redrawn_items;
		std::vector<UIComponent*>::iterator pos;
		if (open_down) {
			for (int i = height + 1; i < height + options.size() + 1; ++i) {
				for (int j = 0; j < width; ++j) {
					curr = this->getRoot().getComponentAt(position.X + j, position.Y + i);
				
					if (curr != NULL) {
						pos = std::find(redrawn_items.begin(), redrawn_items.end(), curr);
						if (redrawn_items.size() == 0 || pos == redrawn_items.end()) {
							redrawn_items.push_back(curr);
							curr->draw();
						}
					}
				}
			}
			if (&this->getRoot() != this && this->getRoot().getYPosition() + this->getRoot().getHeight() + 1 <= this->getYPosition() + this->getHeight() + options.size() + 1) {
				char c = ' ';
				switch (getRoot().getBorderType()) {
				case Dotted:
					c = '-';
					break;
				case Solid:
					c = '\xC4';
					break;
				case Double:
					c = '\xCD';
					break;
				default:
					break;
				}
				applyColors();
				CCTRL.setPosition({this->getXPosition(), this->parent->getYPosition() + this->parent->getHeight()});
				cout << string(this->width + 2, c);
			}
		} else {
			for (int i = 0; i < options.size() + 1; ++i) {
				for (int j = 0; j < width; ++j) {
					curr = this->getRoot().getComponentAt(position.X + j, position.Y - options.size() - 1 + i);
					if (curr != NULL) {
						pos = std::find(redrawn_items.begin(), redrawn_items.end(), curr);
						if (redrawn_items.size() == 0 || pos == redrawn_items.end()) {
							redrawn_items.push_back(curr);
							curr->draw();
						}
					}
				}
			}
		}
		//this->getRoot().draw();
	}
}

string ComboBox::getValue() const {
	return selected_index != -1 ? this->options.at(selected_index) : "No item selected";
}

void ComboBox::onFocus() {
	setFocus(true);
	this->toggle();
	if (this->open) {
		this->temp_index = 0;
		this->draw();
	}
}

void ComboBox::onBlur() {
	setFocus(false);
	if (this->open)
		this->toggle();
}

ComboBox::~ComboBox() {
	CCTRL.detachObserver(this);
	if (_originalState) delete _originalState;
}
