#include "display.h"
#include "moteur.h"

int main() {
	initDisplay();

	while (true) {
		display();
	}

	return 0;
}