#include "display.h"


unsigned int windowWidth = 1850;
unsigned int windowHeight = 1000;
sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Particles life", sf::Style::Default);
float zoom = 1.0f;
sf::Vector2f camPos = sf::Vector2f(0.0f, 0.0f);
bool display_quadtree = false;



void initDisplay(const sf::Vector2f& sizeEnv) {
	windowWidth = 1850;
	windowHeight = 1000;
	window.setSize(sf::Vector2u(windowWidth, windowHeight));
	zoom = 0.3f;
	camPos = mult(sizeEnv, -0.5f);

	display_quadtree = true;
}



void display(const sf::Vector2f& sizeEnv, std::vector<particle*> vectParticles, quadtree& quad) { // les données à affichées sont passées par référence en paramètre dans cette fonction depuis le main 
	window.clear(sf::Color(255, 135, 135));
	
	sf::RectangleShape map;
	map.setSize(mult(sizeEnv, zoom));

	map.setFillColor(sf::Color::White);
	map.setPosition(add(mult(camPos, zoom), sf::Vector2f(windowWidth / 2, windowHeight / 2)));

	window.draw(map);

	if (display_quadtree) quad.display(window, camPos, zoom, sf::Vector2f(windowWidth / 2, windowHeight / 2));


	//particles
	sf::CircleShape cs;
	cs.setRadius(10.0f*zoom);
	
	for (const particle* p: vectParticles){
		cs.setFillColor(sf::Color::Black);
		cs.setPosition(
			add(mult(add(sub(p->getPosition(), sf::Vector2f(10.0f, 10.0f)), camPos), zoom), sf::Vector2f(windowWidth / 2, windowHeight / 2))
		);
		cs.setFillColor(p->getColor());

		window.draw(cs);
	}


	window.display();
}
