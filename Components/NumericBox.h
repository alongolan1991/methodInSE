#pragma once
#include "UIComponent.h"
#include "../Common/Action.h"
#include "Button.h"
#include "../Common/IOConsoleException.h"

class NumericBox : public UIComponent {

	/*===========Nested Classes - START===========================*/
	class IncrementAction : public Action {
		NumericBox &parent;  // Reference to parent

	public:
		IncrementAction(NumericBox &numBox) : parent(numBox) {}
		virtual void action() override;
	};

	class DecrementAction : public Action {
		NumericBox &parent;  // Reference to parent

	public:
		DecrementAction(NumericBox &numBox) : parent(numBox) {}
		virtual void action() override;
	};
	/*===========Nested Classes - END=============================*/

	/*===========NumericBox Defenition=============================*/
	int value, minimum, maximum;
	Button incBtn, decBtn;
	IncrementAction incrementor;
	DecrementAction decrementor;

	void drawValue() const;

public:

	NumericBox(int _val = 0, int _min = -INT_MIN, int _max = INT_MAX, short x_pos = 0, short y_pos = 0, BorderType border = None,
		Color tColor = Black, Color bColor = White, UIComponent *parent = NULL);

	virtual void draw() override;
	virtual void addComponent(UIComponent * component) {};
	virtual void removeAll() {};

	virtual void setHeight(int _h) override {};
	virtual void setWidth(int _w)  override {};
	virtual void setPosition(short pos_x, short pos_y, bool special = false)  override;
	void setVisible(bool visible) override;
	bool isInteractable() override { return true; }

	// setters
	void setValue(int);
	void setMin(int);
	void setMax(int);

	// getters
	int getValue() const { return value; }
	int getMin()   const { return minimum; }
	int getMax()   const { return maximum; }
};
