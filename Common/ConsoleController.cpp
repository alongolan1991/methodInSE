#include "ConsoleController.h"
#include "../Components/Components.h"
#include <algorithm>
#include "../Components/CheckList.h"
#include "../Components/RadioBox.h"

// init static
ConsoleController *ConsoleController::instance = 0;

// private CTOR for jesus
ConsoleController::ConsoleController() : hOutput(GetStdHandle(STD_OUTPUT_HANDLE)), hInput(GetStdHandle(STD_INPUT_HANDLE)), view(NULL), focusedIndex(-1), last_msg_response(-1) {
	GetConsoleMode(hInput, &mode);
	GetConsoleCursorInfo(hOutput, &cursorInfo);
	setCursorVisible(false);
}

// Init console controller
ConsoleController& ConsoleController::init() {
	if (instance == 0) {
		instance = new ConsoleController();
	}
	return *instance;
}

// destroy the controller
void ConsoleController::destroy() {
	if (instance != 0) delete instance;
}

// Prompt user to confirm with message
void ConsoleController::messageDialog(std::string message, BorderType border, Color tColor, Color bColor) {
	if (message.length() == 0) message = "Are you sure?";
	MessageWindow *messageWindow = new MessageWindow(message, getConsoleSize().X / 2 - 25, getConsoleSize().Y / 2 - 2.5, 50, 5, border, tColor, bColor);
	messages.push_back(messageWindow);
	messages[0]->draw();
}

void ConsoleController::resetLastMessage() {
	this->last_msg_response = -1;
}

// sets current cursor position
void ConsoleController::setPosition(COORD c) {
	SetConsoleCursorPosition(hOutput, c);
}

// sets text console attribute
void ConsoleController::setColors(short foregroundColor, short backgroundColor, bool foreground_intensity, bool background_intensity) {
	attr = foregroundColor | FOREGROUND_INTENSITY * foreground_intensity | 16 * backgroundColor | BACKGROUND_INTENSITY * background_intensity;
	SetConsoleTextAttribute(hOutput, attr);
}

void ConsoleController::setDefaultColors(Color tColor, Color bColor) {
	this->defaultTextColor = tColor;
	this->defaultBackgroundColor = bColor;
}

void ConsoleController::setMouseEnabled(bool isVisibile) {
	// TODO: FIXME: this does not seem to have any effect
	//std::cout << (mode | ENABLE_MOUSE_INPUT) << std::endl;
	mode = (isVisibile) ? mode | ENABLE_MOUSE_INPUT : mode & ~ENABLE_MOUSE_INPUT;
	//std::cout << mode << std::endl;
	SetConsoleMode(hInput, mode);
}

void ConsoleController::setCursorVisible(bool isVisible) {
	cursorInfo.bVisible = isVisible;
	SetConsoleCursorInfo(hOutput, &cursorInfo);
}

void ConsoleController::setCursorSize(DWORD size) {
	cursorInfo.dwSize = size;
	SetConsoleCursorInfo(hOutput, &cursorInfo);
}

void ConsoleController::setView(UIComponent * component) {
	this->view = component;
	if (this->view) this->view->setVisible(true);
}

COORD ConsoleController::getPosition() const {

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD coord = { -1,-1 };
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		coord.X = csbi.dwCursorPosition.X;
		coord.Y = csbi.dwCursorPosition.Y;
	}
	return coord;
}

COORD ConsoleController::getConsoleSize() const {
	CONSOLE_SCREEN_BUFFER_INFO csbi = { 0 };
	COORD c = { -1, -1 };
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		c.X = csbi.srWindow.Right;
		c.Y = csbi.srWindow.Bottom;;
	}
	return c;
}

short ConsoleController::getTextColor() const {
	CONSOLE_SCREEN_BUFFER_INFO csbi = { 0 };
	short result;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		result = csbi.wAttributes & 0x000F;
	}
	return result;
}

short ConsoleController::getBackgroundColor() const
{
	CONSOLE_SCREEN_BUFFER_INFO csbi = { 0 };
	short result;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		result = csbi.wAttributes / 16;
	}
	return result;
}

bool ConsoleController::isMouseEnabled() {
	return (mode & ENABLE_MOUSE_INPUT) > 0;
}

bool ConsoleController::isCursorVisible() {
	return cursorInfo.bVisible;
}

DWORD ConsoleController::getCursorSize() {
	return cursorInfo.dwSize;
}

int ConsoleController::getLastMessageResponse() const
{
	return last_msg_response;
}

// test code for events 
// TODO: event thread
void ConsoleController::listenToUserEvents() {
	INPUT_RECORD ir[5] = { 0 };
	DWORD num_read;
	int counter = 0;
	setMouseEnabled(true);
	this->setCursorVisible(false);
	while (1) {
		ReadConsoleInput(hInput, ir, 5, &num_read);
		CONSOLE_SCREEN_BUFFER_INFO cursor;
		bool tab_down = false;

		if (messages.size() > 0 && messages[0]->getResult() != -1) {
			messages[0]->setVisible(false);
			delete messages[0];
			messages.erase(messages.begin());
			view != NULL ? view->setVisible(true) : 1;
			if (messages.size() > 0)
				messages[0]->draw();
		}

		if (num_read) {
			for (int i = 0; i < (int)num_read; i++) {
				switch (ir[i].EventType) {
				case KEY_EVENT:
					KEY_EVENT_RECORD key = ir[i].Event.KeyEvent;
					if (key.bKeyDown) {
						// Block key events when message window is open
						if (messages.size() > 0) {
							continue;
						}
						switch (key.wVirtualKeyCode) {
						case VK_ESCAPE:
							return;
						case VK_TAB:
							if (observers.size() == 0) continue;
							if (focusedIndex == -1)
								++focusedIndex;

							if (observers[focusedIndex]->isTraversable()) {
								if (observers[focusedIndex]->hasFocus()) {
									if (!observers[focusedIndex]->isAtEnd()) {
										observers[focusedIndex]->keyPressed(key);
										break;
									}

								}
							}
							if (observers[focusedIndex]) observers[focusedIndex]->onBlur();
							focusedIndex = ++focusedIndex % observers.size();

							while (observers[focusedIndex]  &&  !observers[focusedIndex]->canGetFocus() ) {
								focusedIndex = ++focusedIndex % observers.size();
							}

							observers[focusedIndex]->onFocus();
							if (observers[focusedIndex]->isTraversable()) {
								if (observers[focusedIndex]->hasFocus()) {
									observers[focusedIndex]->keyPressed(key);
								}
							}

							break;


						default:
							if (focusedIndex > -1 && focusedIndex < observers.size() && observers[focusedIndex] && observers[focusedIndex]->canGetFocus()) {
								observers[focusedIndex]->keyPressed(key);
							}
							break;
						}
					}
					break;
				case MOUSE_EVENT:
					switch (ir[i].Event.MouseEvent.dwButtonState) {
					case RI_MOUSE_LEFT_BUTTON_DOWN:
						this->setCursorVisible(false);
						auto mousePos = ir[i].Event.MouseEvent.dwMousePosition;
						// Trap all mouse events if message window is open
						if (messages.size() > 0) {
							if (isIntersects(mousePos, &messages[0]->getOkBtn())) {
								messages[0]->getOkBtn().mouseClicked(ir[i].Event.MouseEvent);
								last_msg_response = messages[0]->getResult();
								
							}
							if (isIntersects(mousePos, &messages[0]->getCancelBtn())) {
								messages[0]->getCancelBtn().mouseClicked(ir[i].Event.MouseEvent);
								last_msg_response = messages[0]->getResult();
							}
							continue;
						}

						bool intersect = false;
						if (focusedIndex != -1 && isIntersects(mousePos, observers[focusedIndex])) {
							observers[focusedIndex]->mouseClicked(ir[i].Event.MouseEvent);
							break;
						}
						for (auto observer : observers) {
							if (isIntersects(mousePos, observer)) {
								intersect = true;
								if (observer->canGetFocus()) {
									if (focusedIndex != -1 && observers[focusedIndex]->hasFocus() )
										observers[focusedIndex]->onBlur();
									focusedIndex = counter;
								}
								observer->mouseClicked(ir[i].Event.MouseEvent);
							}
							counter++;
						}
						if (false == intersect) {
							if (focusedIndex != -1 && observers[focusedIndex]->hasFocus()) 
								observers[focusedIndex]->onBlur();
						}
						counter = 0;
						break;
					}	
					break;
				}
			}
		}
	}
}

bool ConsoleController::isIntersects(COORD mousePos, UIComponent* comp) {
	return (mousePos.X >= comp->getXPosition() && mousePos.X <= comp->getXPosition() + comp->getWidth() &&
		mousePos.Y >= comp->getYPosition() && mousePos.Y <= comp->getYPosition() + comp->getHeight());
}

void ConsoleController::attachObserver(UIComponent* comp) {
	std::vector<UIComponent*>::iterator position = std::find(this->observers.begin(), this->observers.end(), comp);
	if (position == observers.end())
		observers.push_back(comp);
}

void ConsoleController::detachObserver(UIComponent *ob) {
	std::vector<UIComponent*>::iterator position = std::find(this->observers.begin(), this->observers.end(), ob);
	if (position != this->observers.end())
		this->observers.erase(position);
}

ConsoleController::~ConsoleController() {}
