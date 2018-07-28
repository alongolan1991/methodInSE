#pragma once
#include "UIComponent.h"
#include <string>
#include <sstream>
#include <iterator>

class TextBox : public UIComponent {

	std::string text;
	int maxSize;
	COORD lastIndexPosition;

	void deleteChar(COORD currPos) const;

public:

	TextBox(std::string _text, int _maxSize = 100, short x_pos = 0, short y_pos = 0, short w = 0, short h = 0,
		BorderType border = None, Color tColor = Black, Color bColor = White, UIComponent *parent = NULL);
	
	~TextBox();

	bool canGetFocus() { return true; }

	void mouseClicked(MOUSE_EVENT_RECORD);
	void keyPressed(KEY_EVENT_RECORD);

	void onFocus() override;
	void onBlur() override;
	bool isInteractable() { return true; }

	// setters:
	void setText(std::string);

	// getters:
	std::string getText() const { return text; }

	void draw() override;
	void virtual addComponent(UIComponent* component) {};
	void virtual removeAll() {};
};

