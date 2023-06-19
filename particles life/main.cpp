#include "display.h"
#include "engine.h"
#include "events.h"

extern sf::Vector2f SIZE_ENV;
extern quadtree* mainQuadTree;



int main() {
	int seed = time(nullptr);
	std::cout << "seed : "<<seed << std::endl;
	srand(seed);

	initEngine();
	initDisplay(SIZE_ENV);

	while (true) {
		updateEvents();
		update();
		display(SIZE_ENV, mainQuadTree);
	}

	return 0;
}