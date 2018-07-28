#pragma once
#include "UIComponent.h"
#include <vector>
#include <string>
using namespace std;

class ComboBox : public UIComponent {
	// Instance to keep original state
	ComboBox* _originalState;

	// Selection options
	vector<string> options;
	int selected_index;
	int temp_index;
	bool open_down;
	bool open;

	// Copy CTOR
	ComboBox(ComboBox* o);

	void calcOpenDirection();
public:
	ComboBox(string* options, int len, short pos_x, short pos_y, short width, BorderType border = Double, Color tColor = White, Color bColor = Black, UIComponent* parent = NULL);
	void draw();

	void setPosition(short width, short height, bool special = false);

	// General UI Methods
	bool canGetFocus() { return true; }
	bool isInteractable() { return true; }
	void mouseClicked(MOUSE_EVENT_RECORD);
	void keyPressed(KEY_EVENT_RECORD);

	// ComboBox Specific
	void toggle();
	string getValue() const;
	
	void onFocus() override;
	void onBlur() override;

	~ComboBox();
};

