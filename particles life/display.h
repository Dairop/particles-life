#pragma once
#include "utils.h"


extern unsigned int windowWidth;
extern unsigned int windowHeight;
extern sf::RenderWindow window;
extern sf::Vector2f SIZE;
extern float zoom; // size of the map
extern sf::Vector2f camPos;


void initDisplay();

void display();

