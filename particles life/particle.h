#pragma once
#include "utils.h"
#include "expression.h"

class type_particle;

class type_particle {
protected:
	unsigned int id;
	sf::Color color;
	std::array<float, 5> globalValues = { randFloatNP(), randFloatNP(), randFloatNP(), randFloatNP(), randFloatNP() }; // values between -1 and 1
	std::shared_ptr<expression> _expression;
public:
	type_particle() {
		id = rand() % RAND_MAX;
		globalValues = {randFloatNP(), randFloatNP(), randFloatNP(), randFloatNP(), randFloatNP()};
		sf::Vector3f _c = HSVtoRGB(randFloat() * 360.0f, 1.0f, 1.0f);
		color = sf::Color(_c.x, _c.y, _c.z);
		_expression = std::make_shared<expression>("0.01*&*($1+2*$2-3*$3)");
		std::cout << "test : " << _expression->applyFunction(2, { 1,2,0,5,-4 }) << std::endl;
		//generate the function
	}

	//returns the norm of the force vector
	float interactWith(type_particle& t2, float distance);
	sf::Color getColor() const { return color; }
	std::array<float, 5> & getGlobalValues();
};






class particle
{
protected:	
	sf::Vector2f position;
	sf::Vector2f speed;
	type_particle* type;

public:
	particle(type_particle* _type): type(_type) {
		speed = sf::Vector2f(randFloatNP(), randFloatNP());
	}
	particle(sf::Vector2f mapSize, type_particle* _type) : type(_type) {
		speed = sf::Vector2f(randFloatNP(), randFloatNP());
		position = sf::Vector2f(randFloat() * mapSize.x, randFloat() * mapSize.y);
	}

	type_particle* getType() const { return type; }

	sf::Vector2f getPosition() const { return position; }
	sf::Color getColor() const { return type->getColor(); }
	void interactWith(sf::Vector2f pos2, type_particle& t2, float distance);
	void update(sf::Vector2f mapSize, unsigned int type_env);
};

