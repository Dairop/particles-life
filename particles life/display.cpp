#include "display.h"


unsigned int windowWidth = 1920/2;
unsigned int windowHeight = 1080/2;
sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Particles life", sf::Style::Resize);
float zoom = 1.0f; // size of the map
sf::Vector2f camPos = sf::Vector2f(0.0f, 0.0f);


void initDisplay() {
	windowWidth = 1920/2;
	windowHeight = 1080/2;
	window.setSize(sf::Vector2u(windowWidth, windowHeight));
	zoom = 1.0f; // size of the map
	camPos = sf::Vector2f(0.0f, 0.0f);
}



void display() { // les donn�es � affich�es sont pass�es par r�f�rence en param�tre dans cette fonction depuis le main 
	window.clear(sf::Color(255, 135, 135));
	

	window.display();
}
