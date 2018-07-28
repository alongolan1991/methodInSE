#pragma once
#include "Label.h"
#include "../Common/Action.h"

class Button : public Label {
	Action* actioner;

public:
		Button(Action* act ,std::string _text, short x_pos = 0, short y_pos = 0, int w = 0, int h = 0,
			BorderType border = None, Color tColor = Black, Color bColor = White, UIComponent* parent = NULL);
		~Button();
		void click() const;
		bool isInteractable() { return true; }
		void mouseClicked(MOUSE_EVENT_RECORD);
};
