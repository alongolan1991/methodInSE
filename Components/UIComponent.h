#pragma once
#include "../Common/ConsoleController.h"
#include <iostream>

//typedef enum Color {
//	Black, Blue, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY, DARKGRAY, LIGHTBLUE,
//	LIGHTGREEN, LIGHTCYAN, LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE
//} Color;

struct BorderCharacters {
	char leftTopCorner, rightTopCorner, leftBottomCorner, rightBottomCorner, horizontal, vertical;
};

class UIComponent {

protected:
	UIComponent *parent;
	COORD position;
	short width, height;
	BorderType borderType;
	Color textColor, backgroundColor;
	bool fg_intensity = true, bg_intensity = false;
	bool focus;
	bool is_visible = false;

	void removeFromScreen() const;
	void drawBorder() const;

public:
	virtual ~UIComponent() = 0 {};
	UIComponent(short x_pos = 0, short y_pos = 0, int w = 0, int h = 0, BorderType border = None,
		Color tColor = Black, Color bColor = White, UIComponent *parent = NULL);

	// Component functionality methods
	UIComponent& getRoot();
	virtual void addComponent(UIComponent* component) {};
	virtual void removeComponent(UIComponent* component) {};
	virtual UIComponent* getComponentAt(int x, int y) { return NULL; };
	virtual void removeAll() {};
	virtual void draw() = 0;

	// General UI Methods
	virtual bool canGetFocus() { return false; }
	virtual void mouseClicked(MOUSE_EVENT_RECORD) {}
	virtual void keyPressed(KEY_EVENT_RECORD) {}
	virtual bool isTraversable() { return false; }
	virtual bool isAtEnd() { return true; };
	virtual bool isInteractable() { return false; }
	virtual void setVisible(bool visible);

	virtual void setFocus(bool focus) { this->focus = focus; }
	virtual void onFocus() {}
	virtual void onBlur() {}

	virtual void postDraw() { CCTRL.restoreDefaultColors(); }
	void invertColors();
	void applyColors();

	// setters:
	void setBorderType(BorderType border);
	void setParent(UIComponent* p) { parent = p; }
	virtual void setWidth(int _width);
	virtual void setHeight(int _height);
	virtual void setPosition(short _x, short _y, bool special = false);
	void setTextColor(Color color);
	void setBackgroundColor(Color color);

	// getters:
	BorderType getBorderType()		const { return borderType; }
	short	   getWidth()			const { return width; }
	short	   getHeight()			const { return height; }
	COORD      getDimentions()		const { return { width, height }; }
	COORD	   getPosition()		const { return position; }
	short	   getXPosition()		const { return position.X; }
	short	   getYPosition()		const { return position.Y; }
	Color	   getTextColor()		const { return textColor; }
	Color	   getBackgroundColor() const { return backgroundColor; }
	bool	   hasFocus() { return this->focus; }
};