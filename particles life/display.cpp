#include "display.h"


unsigned int windowWidth = 1920/2;
unsigned int windowHeight = 1080/2;
sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Particles life", sf::Style::Resize);
sf::Vector2f SIZE = sf::Vector2f(1000.0f, 700.0f);
float zoom = 1.0f; // size of the map
sf::Vector2f camPos = sf::Vector2f(0.0f, 0.0f);


void initDisplay() {
	windowWidth = 1920/2;
	windowHeight = 1080/2;
	window.setSize(sf::Vector2u(windowWidth, windowHeight));
	SIZE = sf::Vector2f(1000.0f, 700.0f);
	zoom = 1.0f; // size of the map
	camPos = sf::Vector2f(0.0f, 0.0f);
}



void display() {
	window.clear(sf::Color(255, 135, 135));
	

	window.display();
}
