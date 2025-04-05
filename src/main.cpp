#include "display.h"
#include "engine.h"
#include "events.h"

extern sf::Vector2f SIZE_ENV;
extern quadtree* mainQuadTree;
extern std::vector<particle*> vectAllParticles;


int main() {
	initEngine(time(nullptr));
	initDisplay(SIZE_ENV);


	while (true) {
		updateEvents();
		update();
		display(SIZE_ENV, vectAllParticles, *mainQuadTree);
	}
	return 0;
}