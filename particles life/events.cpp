#include "events.h"


sf::Event event;


void updateEvents() {
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) { window.close(); return; }
		else if (event.type == sf::Event::Resized) {
			// update the view to the new size of the window
			sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			window.setView(sf::View(visibleArea));
			windowWidth = event.size.width;
			windowHeight = event.size.height;
		}

		else if (event.type == sf::Event::MouseWheelMoved) {
			int mdt = std::max(-1, std::min(event.mouseWheel.delta, 1));
			zoom *= 1 + ((float)mdt) / 15;
		}


		else if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Up)	camPos.y -= 20 / zoom;
			if (event.key.code == sf::Keyboard::Down) camPos.y += 20 / zoom;
			if (event.key.code == sf::Keyboard::Left) camPos.x -= 20 / zoom;
			if (event.key.code == sf::Keyboard::Right) camPos.x += 20 / zoom;
		}

		else if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
			
			}
		}
	}
}