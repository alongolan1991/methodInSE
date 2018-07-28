#pragma once
#include <Windows.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <string>
#define CCTRL ConsoleController::init()
#define CCTRLDESTROY ConsoleController::destroy()

/* 
Console control wrapper class (singleton)
Use CCTRL to avoid typing ConsoleController everytime

Yftah
*/

typedef enum BorderType { None, Dotted, Solid, Double } BorderType;
typedef enum Color {
	Black = 0x0000, Blue = 0x0001, Green = 0x0002, Red = 0x0004, Cyan = 0x0003, Purple = 0x0005, Orange = 0x0006, White = 0x0007
	, BrightBlue = 0x0009, BrightGreen = 0x000A, BrightRed = 0x000B, BrightCyan = 0x000C, BrightPurple = 0x000D, BrightOrange = 0x000E, BrightWhite = 0x000F
} Color;

class UIComponent;	//Handling dependency injection
class MessageWindow;

class ConsoleController {
private:
	static ConsoleController *instance;	// singleton instance

	// Windows STD handlers & internal helpers
	HANDLE hOutput;						// output handler
	HANDLE hInput;						// input handler
	DWORD mode;							// current console mode
	WORD attr;							// current console text attributes
	CONSOLE_CURSOR_INFO cursorInfo;		// current console info

	// View to draw
	UIComponent* view;

	// Default Color
	Color defaultTextColor = White, defaultBackgroundColor = Black;

	// TODO: add event thread, listeners, etc	--yftah
	std::vector<UIComponent*> observers;
	std::vector<MessageWindow*> messages;	// messages requiring user input
	int focusedIndex;
	int last_msg_response;

	// CTOR
	ConsoleController();
public:
	// singleton initializer & destroy wrapper (call destroy() at the end of the program)
	static ConsoleController& init();
	static void destroy();

	// Message Window -+
	void messageDialog(std::string message, BorderType border = Solid, Color tColor = Orange, Color bColor = Blue);
	void resetLastMessage();

	// setters
	void setPosition(COORD c);
	void setColors(short foregroundColor, short backgroundColor, bool foreground_intensity = true, bool background_intensity = false);
	void setDefaultColors(Color tColor, Color bColor);
	void setMouseEnabled(bool isEnabled);
	void setCursorVisible(bool isVisible);
	void setCursorSize(DWORD size);
	void setView(UIComponent* component);
	void restoreDefaultColors() { setColors(defaultTextColor, defaultBackgroundColor); };


	// getters
	COORD getPosition() const;
	COORD getConsoleSize() const;
	short getTextColor() const;
	short getBackgroundColor() const;
	bool isMouseEnabled();
	bool isCursorVisible();
	DWORD getCursorSize();
	int getLastMessageResponse() const;

	~ConsoleController();

	// Observer functions
	void listenToUserEvents();
	bool isIntersects(COORD mousePos, UIComponent* comp);
	void attachObserver(UIComponent* ob);
	void detachObserver(UIComponent* ob);
};

