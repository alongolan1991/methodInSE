#include "stdafx.h"
#include "LabelMock.h"

void LabelMock::draw() {

	short text_len = static_cast<short>(getText().length());
	// print centered text
	//c = { position.X + 1 + width / 2 - text_len / 2, position.Y + 1 };
	//ctrl.setPosition(c);
	SCREEN.setY(position.Y + 1);
	SCREEN.setX(position.X + 1 + width / 2 - text_len / 2);
	std::cout << getText();
}