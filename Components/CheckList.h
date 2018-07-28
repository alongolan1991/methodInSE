#pragma once
#include "ListComponent.h"
#include <vector>
#include <string>
using namespace std;


class CheckList : public ListComponent {

	vector<bool> selected_items;

public:

	CheckList(string* options, int len, short pos_x, short pos_y, short width, BorderType border = Solid, Color tColor = White, Color bColor = Black, UIComponent* parent = NULL);

	bool selectedItem(int index) override;
	bool clearSelection(int index) override;
	bool isChecked(int index) override;

	//Special methids for RadioBox
	const vector<string> getCheckedStrings() const;
	bool addSelectedItem(string item);
	bool removeSelectedItem(string item);
	bool checkItems(vector<int> indexes);
};