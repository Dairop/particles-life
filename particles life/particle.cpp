#include "particle.h"


float type_particle::interactWith(type_particle &t2, float distance) {
	if (distance < 30.0f) return -1.0f; // collisions
	float force = 0.15f * (std::sin(t2.globalValues[0] * distance * 0.01f + this->globalValues[3]));
	if (distance > 130.0f) { //less effective when too far
		force /= distance/130.0f;
	}
	return force;
}


void particle::interactWith(sf::Vector2f pos2, type_particle& t2, float distance) {
	sf::Vector2f direction = mult(sub(pos2, this->position), 1 / (distance+0.001f));
	this->speed = add(this->speed, mult(direction, this->type->interactWith(t2, distance)));
}

void particle::update(sf::Vector2f mapSize) {
	this->position = add(this->position, this->speed);

	//border collisions
	//this->position.x = std::fmax(0.0f, std::fmin(this->position.x, mapSize.x));
	//this->position.y = std::fmax(0.0f, std::fmin(this->position.y, mapSize.y));

	//thorus space, we add mapSize because the fmodf function can't work with negative numbers
	this->position.x = std::fmodf(this->position.x+mapSize.x, mapSize.x);
	this->position.y = std::fmodf(this->position.y+mapSize.y, mapSize.y);
		

	this->speed = mult(this->speed, 0.90f);
}