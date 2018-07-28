#include "Panel.h"
#include "../Common/IOConsoleException.h"
#include <algorithm>
#include <sstream>
#include <typeinfo>

void Panel::markOnMap(UIComponent *component) {
	int index = std::find(this->components.begin(), this->components.end(), component) - components.begin();
	bool safe = true;
	int colliding_index = -1;
	if (component->getXPosition() == 12 && component->getYPosition() == 11)
		cout << typeid(*component).name();
	for (int i = 0; i < component->getHeight() + 1; ++i) {
		for (int j = 0; j < component->getWidth(); ++j) {
			safe = isInRange(component, i, j);
			if (safe == false) {
				COORD c = getRelativePosition(component);
				colliding_index = component_map[c.Y + i][c.X + j];
				i = component->getHeight() + 1;
				break;
			}
		}
	}
	if (safe) {
		COORD rel_pos = getRelativePosition(component);
		for (int i = 0; i < component->getHeight() + 1; ++i) {
			for (int j = 0; j < component->getWidth(); ++j) {
				component_map[rel_pos.Y + i][rel_pos.X + j] = index;
			}
		}
	} else {
		stringstream ss; 
		ss << typeid(*component).name() << " is colliding with " << typeid(*components[colliding_index]).name();
		cerr << ss.str();
		throw new OverlapException();
	}
	
}

void Panel::unmarkOnMap(UIComponent * component) {
	COORD rel_pos = getRelativePosition(component);
	for (int i = 0; i < component->getHeight() + 1; ++i) {
		for (int j = 0; j < component->getWidth(); ++j) {
			component_map[rel_pos.Y + i][rel_pos.X + j] = -1;
		}
	}
}

bool Panel::isInRange(UIComponent * component, int y, int x) {
	COORD rel_pos = getRelativePosition(component);
	return rel_pos.Y + y < height && rel_pos.X + x < width && component_map[rel_pos.Y + y][rel_pos.X + x] == -1;
}

COORD Panel::getRelativePosition(UIComponent * component) {
	return {component->getXPosition() - this->getXPosition(), component->getYPosition() - this->getYPosition() };
}

Panel::Panel(short pos_x, short pos_y, short width, short height, BorderType border, Color tColor, Color bColor, UIComponent* parent) : UIComponent(pos_x, pos_y, width, height, border, tColor, bColor, parent) {
	for (int i = 0; i < height + 1; ++i) {
		component_map.push_back(new int[width]);
		for (int j = 0; j < width; ++j) {
			component_map[i][j] = -1;
		}
	}
	this->fg_intensity = true;
}

void Panel::addComponent(UIComponent * component) {
	if (component->getYPosition() + component->getHeight() + 2 > this->getHeight() || component->getWidth() + 2 + component->getXPosition() > this->getWidth())
		throw new DimentionException();

	if (component) {
		component->setParent(this);
		component->setPosition(this->getXPosition() + component->getXPosition() + 1, this->getYPosition() + component->getYPosition() + 1, true);
		this->components.push_back(component);
		this->markOnMap(component);
	}
}

void Panel::removeComponent(UIComponent * component) {
	std::vector<UIComponent*>::iterator position = std::find(this->components.begin(), this->components.end(), component);
	if (position != this->components.end()) {
		this->unmarkOnMap(this->components[position - this->components.begin()]);
		this->components.erase(position);
		delete component;
	}
}

UIComponent * Panel::getComponentAt(int x, int y) {
	if (x > -1 && y > - 1 && x < getWidth() && y < getHeight() && component_map[y][x] > -1 && component_map[y][x] < components.size()) {
		return components[component_map[y][x]];
	}
	return NULL;
}

void Panel::removeAll() {
	while (components.size() > 0) {
		this->removeComponent(components[0]);
	}
}

void Panel::draw() {
	applyColors();
	for (short i = 0; i < height + 1; ++i) {
		CCTRL.setPosition({ position.X, position.Y + i });
		for (short j = 0; j < width + 2; ++j) putchar(' ');
	}
	UIComponent::draw();
	for (vector<UIComponent *>::iterator it = components.begin(); it != components.end(); ++it) {
		(*it)->draw();
	}
	postDraw();
}

void Panel::setVisible(bool visible) {
	bool prev = this->is_visible;
	UIComponent::setVisible(visible);
	if (this->is_visible == prev) return;
	for each (UIComponent* comp in components) {
		comp->setVisible(visible);
	}
}


void Panel::setPosition(short pos_x, short pos_y, bool special) {
	if (pos_x >= 0 && pos_y >= 0) {
		if (is_visible) removeFromScreen();
		for each (UIComponent* component in components) {
			component->setPosition(component->getXPosition() - position.X + pos_x, component->getYPosition() - position.Y + pos_y, true);
		}
		this->position.X = pos_x;
		this->position.Y = pos_y;
		if (is_visible) draw();
	}
}

Panel::~Panel() {
	this->removeAll();
	for (int i = 0; i < height; ++i) {
		if (component_map[i]) delete [] component_map[i];
	}
}
