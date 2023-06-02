#include "display.h"
#include "moteur.h"

int main() {
	initMoteur();
	initDisplay();

	while (true) {
		update();
		display();
	}

	return 0;
}