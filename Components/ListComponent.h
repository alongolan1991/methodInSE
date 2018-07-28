#pragma once
#include "UIComponent.h"
#include "../Common/IOConsoleException.h"
#include <vector>
#include <string>
using namespace std;

class ListComponent : public UIComponent {
protected:
	vector<string> options;
	short hovered_index;

	string checked_mark;
	string unchecked_mark;
public:

	ListComponent(string* options, int len, short pos_x, short pos_y, short width, string checked_mark, string unchecked_mark, 
		BorderType border = Solid, Color tColor = White, Color bColor = Black, UIComponent* parent = NULL);

	void draw();
	void invertNeeded(int i);
	void mouseClicked(MOUSE_EVENT_RECORD);
	void keyPressed(KEY_EVENT_RECORD);

	bool isTraversable() override { return true; }
	bool isAtEnd() override { return options.size() - 1 == hovered_index; }
	void onFocus() override;
	void onBlur()  override;

	virtual bool selectedItem(int index) = 0;
	virtual bool clearSelection(int index) = 0;
	virtual bool isChecked(int index) = 0;
	bool canGetFocus() { return true; }
	bool isInteractable() { return true; }

	void setHeight(int _height) { throw new HeightNotOverrideable(); }
};
