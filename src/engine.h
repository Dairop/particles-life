#pragma once
#include "utils.h"
#include "quadtree.h"
#include "particle.h"

extern std::vector<particle*> vectAllParticles;

void initEngine(int seed);
void initEngine();
void update(); //calcul d'une nouvelle iteration


