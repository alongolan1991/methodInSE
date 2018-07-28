#include "ListComponent.h"

ListComponent::ListComponent(string * options, int len, short pos_x, short pos_y, short width, string checked_mark, string unchecked_mark, BorderType border, Color tColor, Color bColor, UIComponent * parent)
	: UIComponent(pos_x, pos_y, width + 4, 2, border, tColor, bColor, parent), hovered_index(-1),
		checked_mark(checked_mark), unchecked_mark(unchecked_mark) {
	for (int i = 0; i < len; ++i) {
		this->options.push_back(options[i]);
	}
	height = this->options.size() + 1;
	//CCTRL.attachObserver(this);
}

void ListComponent::draw()
{
	//height = static_cast<short> (options.size() + 1);
	UIComponent::draw();
	ConsoleController ctrl = CCTRL;
	COORD c = { position.X + 1, position.Y + 1 };
	ctrl.setPosition(c);

	for (int i = 0; i < options.size(); i++) {
		applyColors();
		for (int i = width; i > 0; --i) putchar(' ');
		ctrl.setPosition(c);
		string checkMark = isChecked(i) ? checked_mark : unchecked_mark;
		invertNeeded(i);
		for (int i = width; i > 0; --i) putchar(' ');
		ctrl.setPosition(c);
		cout << checkMark << options.at(i).substr(0, width - 4);
		ctrl.setPosition({ c.X, ++c.Y });
		invertNeeded(i);
		postDraw();
	}
}

void ListComponent::invertNeeded(int i)
{
	if (i == hovered_index) {
		invertColors();
		applyColors();
	}
}

void ListComponent::mouseClicked(MOUSE_EVENT_RECORD mouseEvent)
{
	setFocus(true);

	COORD pos = mouseEvent.dwMousePosition;

	if (pos.Y != position.Y && pos.Y != position.Y + height && pos.X != position.X) {
		int index = pos.Y - position.Y - 1;
		if (isChecked(index))
		{
			clearSelection(index);
		}
		else {
			selectedItem(index);
		}
	}
}

void ListComponent::keyPressed(KEY_EVENT_RECORD keyEvent) {
	switch (keyEvent.wVirtualKeyCode) {
	case VK_UP:
		if (--hovered_index == -1) hovered_index = options.size() - 1;
		CCTRL.setPosition({ position.X + 2, position.Y + hovered_index + 1 });
		break;
	case VK_TAB:
		//TBD
	case VK_DOWN:
		hovered_index = (++hovered_index) % options.size();
		CCTRL.setPosition({ position.X + 2, position.Y + hovered_index + 1 });
		break;
	case VK_SPACE:
	case VK_RETURN:
		if (isChecked(hovered_index))
		{
			int tmpSelectedIndex = hovered_index;
			clearSelection(hovered_index);
			hovered_index = tmpSelectedIndex;
		}
		else {
			selectedItem(hovered_index);
		}
		break;
	}
	draw();
}

void ListComponent::onFocus() {
	setFocus(true);
	CCTRL.setPosition({ position.X + 2, position.Y + 1 + hovered_index });
}

void ListComponent::onBlur() {
	hovered_index = -1;
	setFocus(false);
	draw();
}






