#include "engine.h"



sf::Vector2f SIZE_ENV = sf::Vector2f(2000, 2000); //Taille environnement 
std::vector <type_particle> typesOfParticles;
std::vector <particle> particles;
/*
Types of environments:
	0: rectangle with collisions
	#1: thorus
	#2: gravity
	# ...
*/
unsigned int type_env = 0; 


void initEngine(){
	int seed = time(nullptr);
	std::cout << "seed : " << seed << std::endl;
	srand(seed);

	typesOfParticles.clear();
	particles.clear();

	unsigned int number_of_types = 5;
	unsigned int number_of_particles = 500;

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
		for (unsigned int j = 0; j < particles.size(); j++) {
			if (i == j) continue;
			particle* p1 = &particles[i];
			particle* p2 = &particles[j];


			float dist2Interaction = 100000;
			if (type_env == 0) {
				dist2Interaction = dist2(p1->getPosition(), p2->getPosition());
			}
			else if (type_env == 1) {
				dist2Interaction = dist2OnThorus(SIZE_ENV, p1->getPosition(), p2->getPosition());
			}

			if (dist2Interaction < 40000) { //interaction
				if (dist2Interaction < 20 * 20) { //collision
					sf::Vector2f pos1 = p1->getPosition();
					sf::Vector2f pos2 = p2->getPosition();
					collideCircles(pos1, pos2, 10, 10, dist2Interaction);
					p1->setPosition(pos1);
					p2->setPosition(pos2);
				}
				else {
					p1->interactWith(p2->getPosition(), *(p2->getType()), std::sqrt(dist2Interaction));
				}
			}
		}

		particles[i].update(SIZE_ENV, type_env);
	}

	for (unsigned int n = 0; n < 8; n++) {
		for (unsigned int i = 0; i < particles.size(); i++) {
			for (unsigned int j = i; j < particles.size(); j++) {
				if (i == j) continue;
				particle* p1 = &particles[i];
				particle* p2 = &particles[j];


				float dist2Interaction = 100000;
				if (type_env == 0) {
					dist2Interaction = dist2(p1->getPosition(), p2->getPosition());
				}
				if (dist2Interaction < 20 * 20) { //collision
					sf::Vector2f pos1 = p1->getPosition();
					sf::Vector2f pos2 = p2->getPosition();
					collideCircles(pos1, pos2, 10, 10, dist2Interaction);
					p1->setPosition(pos1);
					p2->setPosition(pos2);
				}
			}
		}
	}
}

