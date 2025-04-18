#pragma once
#include "utils.h"
#include "quadtree.h"

extern unsigned int windowWidth;
extern unsigned int windowHeight;
extern sf::RenderWindow window;
extern float zoom;
extern sf::Vector2f camPos;



void initDisplay(const sf::Vector2f& sizeEnv);

void display(const sf::Vector2f& sizeEnv, std::vector<particle*> vectParticles, quadtree& quad);

