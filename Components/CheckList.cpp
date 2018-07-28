#include "CheckList.h"

CheckList::CheckList(string* options, int len, short pos_x, short pos_y, short width, BorderType border, Color tColor, Color bColor, UIComponent* parent)
	: ListComponent(options, len, pos_x, pos_y, width + 4, "[*] ", "[ ] ", border, tColor, bColor, parent),
	selected_items(len, false) {
}

bool CheckList::selectedItem(int index) {
	if (index < 0 || index >= options.size() || selected_items[index]) {
		return false;
	}
	selected_items[index] = true;
	hovered_index = index;
	applyColors();
	draw();
	return true;
}

bool CheckList::clearSelection(int index) {
	if (isChecked(index)) {
		hovered_index = -1;
		selected_items[index] = false;
		draw();
		return true;
	}
	return false;
}

bool CheckList::isChecked(int index) 
{
	return selected_items[index];
}

const vector<string> CheckList::getCheckedStrings() const 
{
	vector<string> current_selected_items;
	for (int i = 0; i < selected_items.size(); i++) 
		if (selected_items[i])
			current_selected_items.push_back(options[i]);
	return current_selected_items;
}

bool CheckList::addSelectedItem(string item)
{
	for (int i = 0; i < options.size(); i++) {
		if (options[i] == item) {
			if (selected_items[i] == false) {
				selected_items[i] = true;
				//hovered_index = index;
				return true;
			}
		}
	}
	return false;
}

bool CheckList::removeSelectedItem(string item)
{
	for (int i = 0; i < options.size(); i++) {
		if (options[i] == item) {
			if (selected_items[i] == true) {
				selected_items[i] = false;
				//hovered_index = index;
				return true;
			}
		}
	}
	return false;
}

bool CheckList::checkItems(vector<int> indexes)
{
	for (int i = 0; i < indexes.size(); i++) {
		if (indexes[i] < 0 || indexes[i] >= options.size())
			return false;
		selected_items[indexes[i]] = true;
	}
	return true;
}
