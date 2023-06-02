#include "display.h"
#include "moteur.h"

extern sf::Vector2f SIZE_ENV;


int main() {
	initMoteur();
	initDisplay();

	while (true) {
		update();
		display(SIZE_ENV);
	}

	return 0;
}