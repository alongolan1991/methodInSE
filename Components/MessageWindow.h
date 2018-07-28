#pragma once
#include "Button.h"


class MessageWindow : public UIComponent {

private:

	class OkAction : public Action {
		MessageWindow& parent;
	public:
		OkAction(MessageWindow& m) : parent(m) {}
		virtual void action() override { parent.result = 1; /*std::cout << "OK!"; */}
	};

	class CancelAction : public Action {
		MessageWindow& parent; 
	public:
		CancelAction(MessageWindow& m) : parent(m) {}
		virtual void action() override {parent.result = 0; /*std::cout << "CANCEL!";*/}
	};

	int result = -1;
	std::string text;
	Button okBtn, cancelBtn;
	OkAction okAction;
	CancelAction cancelAction;

public:
	MessageWindow(std::string _text, short x_pos = 0, short y_pos = 0, short w = 0, short h = 0,
		BorderType border = Solid, Color tColor = Black, Color bColor = White, UIComponent *parent = NULL);
	
	void draw() override;
	void addComponent(UIComponent* component) {};
	void removeAll() {};

	bool isInteractable() { return true; }

	UIComponent& getOkBtn() { return okBtn; }
	UIComponent& getCancelBtn() { return cancelBtn; }

	void setVisible(bool visible) override;

	void setPosition(short pos_x, short pos_y, bool special = false) override;

	int getResult() { return result; }
};