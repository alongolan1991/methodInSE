#include "RadioBox.h"


RadioBox::RadioBox(string* options, int len, short pos_x, short pos_y, short width, BorderType border, Color tColor, Color bColor, UIComponent* parent)
	: ListComponent(options, len, pos_x, pos_y, width + 4, "(*) ", "( ) ", border, tColor, bColor, parent), selected_index(-1) {

}

const string RadioBox::getCheckedString() const {

	if (selected_index != -1) {
		return NULL;
	}

	return options.at(selected_index);
}

bool RadioBox::selectedItem(int index) {

	if (index < 0 || index >= options.size() || index == selected_index) {
		return false;
	}
	selected_index = index;
	hovered_index = selected_index;
	applyColors();
	draw();
	return true;
}

bool RadioBox::clearSelection(int index) {

	if (selected_index == -1) {
		return false;
	}
	selected_index = hovered_index = -1;
	draw();
	return true;
}

bool RadioBox::isChecked(int index)
{
	return index == selected_index;
}

