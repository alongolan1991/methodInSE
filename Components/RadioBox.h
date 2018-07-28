#pragma once
#include "ListComponent.h"
#include "../Common/IOConsoleException.h"
#include <vector>
#include <string>
using namespace std;


class RadioBox : public ListComponent {

	short selected_index;

public:

	RadioBox(string* options, int len, short pos_x, short pos_y, short width, BorderType border = Solid, Color tColor = White, Color bColor = Black, UIComponent* parent = NULL);

	bool selectedItem(int index) override;
	bool clearSelection(int index) override;
	bool isChecked(int index) override;

	//Special methids for RadioBox
	const string getCheckedString() const;

};
