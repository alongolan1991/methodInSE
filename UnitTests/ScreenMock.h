#pragma once
#include "../Common/ConsoleController.h"

#define SCREEN ScreenMock::init()

/*
 * This class mocks the console so that a "sample" of what has been drawn can be taken.
 */
class ScreenMock {

	static ScreenMock *instance;
	std::stringstream *screen; // stringstreams array

	ScreenMock();

public:

	static ScreenMock& init();
	static void destroy();

	void setY(int index) { 
		std::cout.rdbuf(screen[index].rdbuf()); // redirect to a specific stringstream
	}

	void setX(int index) { 
		for (int i = 0; i < index; ++i) std::cout << " "; // resemble setX by writing spaces
	}

	std::stringstream* getScreen() const { return screen; }
};