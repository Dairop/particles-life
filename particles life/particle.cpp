#include "particle.h"

std::array<float, 5> & type_particle::getGlobalValues() {
	return globalValues;
}

float type_particle::interactWith(type_particle &t2, float distance) {
	////float force = (std::tanh(t2.globalValues[3] + this->globalValues[3]*-0.6f + (t2.globalValues[0] > this->globalValues[1] * -1.3f || distance/100.0f < -t2.globalValues[2])));
	//float force = (t2.globalValues[0] > this->globalValues[2]) - (t2.globalValues[3] * distance/100.0f < this->globalValues[0]);
	//float force = ((t2.globalValues[1] + this->globalValues[1] > 0.2f) != (t2.globalValues[0] > this->globalValues[2]/2 + sin(distance/50.0f)))*2-1;

	const float maxSpeed = 0.5f;
	float force = _expression->applyFunction(distance, t2.getGlobalValues());
	if (abs(force) > maxSpeed) {
		force = ((force > 0.0f) ? 1.0f : -1.0f) * maxSpeed + force / 10.0f;
	}

	force = std::fmin(std::fmax(force, -maxSpeed * 1.1f), maxSpeed * 1.1f);

	if (distance > 130.0f) { //less effective when too far
		//force /= distance/130.0f;
	}

	return force;
}


void particle::interactWith(sf::Vector2f pos2, type_particle& t2, float distance) {
	sf::Vector2f direction = mult(sub(pos2, this->position), 1 / (distance+0.001f));
	this->speed = add(this->speed, mult(direction, this->type->interactWith(t2, distance)));
}

void particle::update(sf::Vector2f mapSize, unsigned int type_env) {
	speed.x += 0;
	speed.y += 0;
	this->position = add(this->position, mult(this->speed, 0.1 /*mass*/));

	//resistance
	float k = -60 * 3.14159 * 0.0091 /*fluid's viscosity*/ * 0.0015 /*particle's radius*/;
	float d = dist(sf::Vector2f(0, 0), this->speed);
	sf::Vector2f fluid_resistance = mult(mult(this->speed, 1 / (d + 0.0001)), k * d*d);
	//std::cout << fluid_resistance.x << " " << fluid_resistance.y << "    ";
	this->speed = add(this->speed, fluid_resistance);


	if (type_env == 0) {
		//rectangle & collisions
		this->speed.x = (this->position.x < 0) ? std::fabs(this->speed.x): this->speed.x;
		this->speed.x = (this->position.x > mapSize.x) ? -std::fabs(this->speed.x) : this->speed.x;
		this->speed.y = (this->position.y < 0) ? std::fabs(this->speed.y) : this->speed.y;
		this->speed.y = (this->position.y > mapSize.y) ? -std::fabs(this->speed.y) : this->speed.y;
		

		this->position.x = std::fmax(0, std::fmin(mapSize.x, this->position.x));
		this->position.y = std::fmax(0, std::fmin(mapSize.y, this->position.y));
	}
	else if (type_env == 1) {
		//thorus space, we add mapSize because the fmodf function can't work with negative numbers
		this->position.x = std::fmodf(this->position.x + mapSize.x, mapSize.x);
		this->position.y = std::fmodf(this->position.y + mapSize.y, mapSize.y);
	}
	else if (type_env == 2) {
		//normal box  with gravity
		this->speed.y += 2.0f;

		this->speed.x = (this->position.x < 0) ? std::fabs(this->speed.x) : this->speed.x;
		this->speed.x = (this->position.x > mapSize.x) ? -std::fabs(this->speed.x) : this->speed.x;
		this->speed.y = (this->position.y < 0) ? std::fabs(this->speed.y) : this->speed.y;
		this->speed.y = (this->position.y > mapSize.y) ? -std::fabs(this->speed.y) : this->speed.y;


		this->position.x = std::fmax(0, std::fmin(mapSize.x, this->position.x));
		this->position.y = std::fmax(0, std::fmin(mapSize.y, this->position.y));
	}
}