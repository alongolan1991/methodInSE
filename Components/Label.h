#pragma once
#include "UIComponent.h"
#include <string>

class Label: public UIComponent {
private:
	std::string text;

public:
	Label(std::string _text, short x_pos = 0, short y_pos = 0, short w = 0, short h = 0,
		BorderType border = None, Color tColor = White, Color bColor = Black, UIComponent *parent = NULL);
	~Label() {};

	// setters:
	void setText(std::string);

	// getters:
	std::string getText() const { return text; }

	void draw() override;
	void virtual addComponent(UIComponent* component) {};
	void virtual removeAll() {};
};
