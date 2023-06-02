#include "display.h"


unsigned int windowWidth = 1850;
unsigned int windowHeight = 1000;
sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Particles life", sf::Style::Default);
float zoom = 1.0f; // size of the map
sf::Vector2f camPos = sf::Vector2f(0.0f, 0.0f);




void initDisplay() {
	windowWidth = 1850;
	windowHeight = 1000;
	window.setSize(sf::Vector2u(windowWidth, windowHeight));
	zoom = 1.0f; // size of the map
	camPos = sf::Vector2f(0.0f, 0.0f);
}



void display(sf::Vector2f& sizeEnv) { // les données à affichées sont passées par référence en paramètre dans cette fonction depuis le main 
	window.clear(sf::Color(255, 135, 135));
	
	sf::RectangleShape map;
	map.setSize(mult(sizeEnv, zoom));
	map.setFillColor(sf::Color::White);
	map.setPosition(add(mult(camPos, zoom), sf::Vector2f(windowWidth / 2, windowHeight / 2)));

	window.draw(map);

	window.display();
}
