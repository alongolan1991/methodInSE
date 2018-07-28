#include "stdafx.h"
#include "ScreenMock.h"

// init static
ScreenMock *ScreenMock::instance = 0;

ScreenMock::ScreenMock() {
	int rows = CCTRL.getConsoleSize().Y - 1;
	screen = new std::stringstream[rows];
	for (int i = 0; i < rows; ++i)
		screen[i].str("");
}

ScreenMock& ScreenMock::init() {
	if (instance == 0) {
		instance = new ScreenMock();
	}
	return *instance;
}

void ScreenMock::destroy() {
	if (instance->screen) 
		delete[] instance->screen;
	if (instance) 
		delete instance;
}
