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

void particle::update(sf::Vector2f mapSize, unsigned int type_env) {
	this->position = add(this->position, this->speed);

	this->speed = mult(this->speed, 0.90f);

	if (type_env == 0) {
		//rectangle & collisions
		this->position.x = std::fmax(0, std::fmin(mapSize.x, this->position.x));
		this->position.y = std::fmax(0, std::fmin(mapSize.y, this->position.y));
	}
	else if (type_env == 1) {
		//thorus space, we add mapSize because the fmodf function can't work with negative numbers
		this->position.x = std::fmodf(this->position.x + mapSize.x, mapSize.x);
		this->position.y = std::fmodf(this->position.y + mapSize.y, mapSize.y);
	}

	const float MAX_SPEED = 15;
}