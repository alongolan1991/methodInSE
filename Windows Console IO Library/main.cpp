#include "../Components/Label.h"
#include "../Components/MessageWindow.h"
#include "../Components/NumericBox.h"
#include "../Components/Button.h"
#include "../Components/TextBox.h"
#include "../Components/Components.h"
#include "../Components/CheckList.h"
#include "../Components/RadioBox.h"
#include <iostream>
#include <thread>
using namespace std;

static Label* msgDisplay;

class ActionTest : public Action {

	virtual void action() override {
		CreateThread(NULL, 0, submitForm, NULL, 0, NULL);
	}
	static DWORD WINAPI submitForm(LPVOID lpParam) {
		CCTRL.messageDialog("Are You Sure You Want To Submit ?", Double);
		while (CCTRL.getLastMessageResponse() == -1 && msgDisplay != NULL);
		if (CCTRL.getLastMessageResponse() == 1) {
			msgDisplay->setVisible(true);
		}
		else {
			msgDisplay->setVisible(false);
		}
		CCTRL.resetLastMessage();
		return 1;
	}
};

void main() {
	
	string radioBoxParams[]  = { "a", "b" };
	string checkListParams[] = { "a", "b", "c", "d", "e", "f" };
	string comboBoxParams[]  = {"a", "b", "c", "d", "e", "f", "g", "h"};

	UIComponent* panel = new Panel(0, 0, CCTRL.getConsoleSize().X - 1, CCTRL.getConsoleSize().Y , Solid, White, Black, NULL);
	UIComponent* headlineLabel = new Label("GUI", 13, 0, 10, 2, Solid, Blue, White);
			
	UIComponent* fullNameLabel = new Label("Text-Box: ", 0, 3, 10, 2);
	UIComponent* fullNameInput = new TextBox("", 15, 13, 3, 30, 2, Solid, White, Black);

	UIComponent* radioLabel   = new Label("Radio-Box: ", 0, 6, 10, 2);
	UIComponent* genderInput   = new RadioBox(radioBoxParams, 2, 13, 6, 22, Solid, White, Black);

	UIComponent* colorsLabel   = new Label("Combo-Box: ", 0, 10, 10, 2);
	UIComponent* colorsInput   = new ComboBox(comboBoxParams, 8, 13, 10, 30, Solid, White, Black);

	UIComponent* ageLabel	   = new Label("Numeric-Box: ", 0, 13, 12, 2);
	UIComponent* ageInput	   = new NumericBox(16, 10, 100, 13, 13, Solid, White, Black);

	UIComponent* hobbiesLabel  = new Label("Check-List: ", 0, 16, 12, 2);
	UIComponent* hobbiesInput  = new CheckList(checkListParams, 6, 13, 16, 22, Solid, White, Black);

	UIComponent* resultLabel   = new Label("Form Submitted", CCTRL.getConsoleSize().X / 2 - 7, 22, 14, 2, Dotted, BrightPurple, Black);
	msgDisplay = dynamic_cast<Label*>(resultLabel);
	ActionTest submitAction;
	UIComponent* submitButton  = new Button(&submitAction, "Button", 13, 25, 10, 2, Solid, Blue, White);

	panel->addComponent(headlineLabel);
	panel->addComponent(fullNameLabel);
	panel->addComponent(fullNameInput);
	panel->addComponent(ageLabel);
	panel->addComponent(ageInput);
	panel->addComponent(radioLabel);
	panel->addComponent(genderInput);
	panel->addComponent(hobbiesLabel);
	panel->addComponent(hobbiesInput);
	panel->addComponent(colorsLabel);
	panel->addComponent(colorsInput);
	panel->addComponent(submitButton);
	CCTRL.setView(panel);
	CCTRL.listenToUserEvents();
	delete panel;
	CCTRLDESTROY;
}