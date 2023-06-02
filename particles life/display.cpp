#include "display.h"


unsigned int windowWidth = 1920;
unsigned int windowHeight = 1080;
sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Particles life", sf::Style::Resize);
float zoom = 1.0f; // size of the map
sf::Vector2f camPos = sf::Vector2f(0.0f, 0.0f);


void initDisplay() {
	windowWidth = 1920;
	windowHeight = 1080;
	window.setSize(sf::Vector2u(windowWidth, windowHeight));
	zoom = 1.0f; // size of the map
	camPos = sf::Vector2f(0.0f, 0.0f);
}



void display(sf::Vector2f& sizeEnv) { // les données à affichées sont passées par référence en paramètre dans cette fonction depuis le main 
	window.clear(sf::Color(255, 135, 135));
	
	sf::RectangleShape map;
	map.setSize(sizeEnv);
	map.setFillColor(sf::Color::White);
	map.setPosition(camPos);

	window.draw(map);

	window.display();
}
