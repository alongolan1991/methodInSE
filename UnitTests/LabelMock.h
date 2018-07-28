#pragma once
#include "../Components/Label.h"
#include "ScreenMock.h"

class LabelMock : public Label {

public:

	LabelMock(std::string _text, short x_pos = 0, short y_pos = 0, short w = 0, short h = 0,
		BorderType border = None, Color tColor = Black, Color bColor = White, UIComponent *parent = NULL)
		: Label(_text, x_pos, y_pos, w, h, border, tColor, bColor, parent) {}
		
	void draw();
};