#include "display.h"
#include "engine.h"
#include "events.h"

extern sf::Vector2f SIZE_ENV;
extern std::vector<particle> particles;

int main() {
	initEngine();
	initDisplay(SIZE_ENV);

	while (true) {
		updateEvents();
		update();
		display(SIZE_ENV, particles);
	}

	return 0;
}