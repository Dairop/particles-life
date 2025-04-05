#pragma once
#include "utils.h"
#include "engine.h"
#include "display.h"

extern sf::RenderWindow window;
extern unsigned int windowWidth;
extern unsigned int windowHeight;
extern float zoom;
extern sf::Vector2f camPos;

void updateEvents();
