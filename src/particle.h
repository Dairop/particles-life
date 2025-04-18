#pragma once
#include "utils.h"
#include "expression.h"
#include <fstream>

class type_particle;

class type_particle {
protected:
	unsigned int id;
	sf::Color color;
	std::array<float, 5> globalValues = { randFloatNP(), randFloatNP(), randFloatNP(), randFloatNP(), randFloatNP() }; // values between -1 and 1
	std::shared_ptr<expression> _expression;
public:
	type_particle(float colorAngle) {
		id = rand() % RAND_MAX;
		globalValues = {randFloatNP(), randFloatNP(), randFloatNP(), randFloatNP(), randFloatNP()};
		sf::Vector3f _c = HSVtoRGB(colorAngle * 360.0f, 1, 1);
		color = sf::Color(_c.x, _c.y, _c.z);

		bool loading_from_file = true;

		if (loading_from_file) {
			std::ifstream file("../expression.txt");
			std::string line;
			std::getline(file, line);
			_expression = std::make_shared<expression>(line);
		} else {
			//random expression
			//std::string expr_str = "tanh(" + generate_rand_str_expression(5) + ")";
			//"tanh(sin(&*&/10000 +" + generate_rand_str_expression(2) + ")+" + generate_rand_str_expression(4) + "+cos(" + generate_rand_str_expression(3) + "))";
			//std::string expr_str = generate_rand_str_expression(5, false);
			std::string expr_str = "tanh($1+&)";
			std::cout << expr_str << "\n";
			_expression = std::make_shared<expression>( expr_str );
		}
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
		position = sf::Vector2f(randFloat(), randFloat());
	}
	particle(sf::Vector2f mapSize, type_particle* _type) : type(_type) {
		speed = sf::Vector2f(randFloatNP(), randFloatNP());
		position = sf::Vector2f(randFloat() * mapSize.x, randFloat() * mapSize.y);
	}

	type_particle* getType() const { return type; }

	sf::Vector2f getPosition() const { return position; }
	void setPosition(sf::Vector2f& newPos) { position = newPos; }
	sf::Color getColor() const { return type->getColor(); }
	void interactWith(sf::Vector2f pos2, type_particle& t2, float distance);
	void update(sf::Vector2f mapSize, unsigned int type_env);
};

