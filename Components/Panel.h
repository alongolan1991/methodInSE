#pragma once
#include "UIComponent.h"
#include <vector>

using namespace std;

class Panel : public UIComponent {
	// component list
	vector<UIComponent*> components;
	// component map
	vector<int *> component_map;

	// Private methods (for component map)
	void markOnMap(UIComponent *component);
	void unmarkOnMap(UIComponent *component);
	bool isInRange(UIComponent * component, int i, int j);
	COORD getRelativePosition(UIComponent * component);

public:
	Panel(short pos_x, short pos_y, short width, short height, BorderType border, Color tColor, Color bColor, UIComponent* parent);

	// General UI Methods
	virtual void addComponent(UIComponent* component);
	virtual void removeComponent(UIComponent* component);
	virtual UIComponent* getComponentAt(int x, int y);
	virtual void removeAll();
	virtual void draw();
	void setVisible(bool visible) override;

	// Setters
	void setPosition(short pos_x, short pos_y, bool special = false) override;

	// Debug method
	void printMap() {
		cout << endl;
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				component_map[i][j] > -1 ? cout << "c" : cout << "-";
				
			}
			cout << endl;
		}
	};

	~Panel();
};

