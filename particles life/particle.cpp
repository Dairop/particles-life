#include "particle.h"


float type_particle::interactWith(type_particle &t2, float distance) {
	return std::sin(t2.globalValues[0] * distance + this->globalValues[3]);
}


void particle::interactWith(type_particle& t2, float distance) {
	this->type->interactWith(t2, distance);
}

void particle::update(sf::Vector2f mapSize) {
	this->position = add(this->position, this->speed);

	//border collisions
	//this->position.x = std::fmax(0.0f, std::fmin(this->position.x, mapSize.x));
	//this->position.y = std::fmax(0.0f, std::fmin(this->position.y, mapSize.y));

	this->speed = mult(this->speed, 1.0f);
}