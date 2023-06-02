#include "display.h"
#include "moteur.h"
#include "events.h"

extern sf::Vector2f SIZE_ENV;


int main() {
	initMoteur();
	initDisplay(SIZE_ENV);

	while (true) {
		updateEvents();
		update();
		display(SIZE_ENV);
	}

	return 0;
}