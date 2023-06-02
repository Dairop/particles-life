#include "engine.h"



sf::Vector2f SIZE_ENV = sf::Vector2f(1000, 650); //Taille environnement 
std::vector <type_particle> typesOfParticles;
std::vector <particle> particles;


void initEngine(){
	typesOfParticles.clear();
	particles.clear();

	unsigned int number_of_types = 5;
	unsigned int number_of_particles = 100;

	for (unsigned int i = 0; i < number_of_types; i++) {
		typesOfParticles.push_back(type_particle());
	}

	for (unsigned int i = 0; i < number_of_particles; i++) {
		particles.push_back(particle(SIZE_ENV, &typesOfParticles[rand()%number_of_types]));
	}
}

void update() {
	//non-optimized interactions
	for (unsigned int i = 0; i < particles.size(); i++) {
		for (unsigned int j = i + 1; j < particles.size(); j++) {
			float d2 = dist2(particles[i].getPosition(), particles[j].getPosition());
			if (d2 < 10000) { //interaction
				if (d2 < 900) { //collision
					//
				}
				else {
					particles[i].interactWith(*(particles[j].getType()), 0);
				}
			}
		}

		particles[i].update(SIZE_ENV);
	}
}