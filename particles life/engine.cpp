#include "engine.h"



sf::Vector2f SIZE_ENV = sf::Vector2f(3800, 3800); //Taille environnement 
std::vector <type_particle> typesOfParticles;

quadtree* mainQuadTree;

//std::vector <particle> particles;

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

	if (mainQuadTree != nullptr) mainQuadTree->del();
	// le rayon du quadtree est un peu plus grand que l'environnement afin de ne pas oublier les particules
	// qui sont exactement sur le bord
	RectByCenter rectQ(mult(SIZE_ENV, 0.5), mult(SIZE_ENV, 0.501));
	mainQuadTree = new quadtree(rectQ);

	unsigned int number_of_types = 5;
	unsigned int number_of_particles = 5000;

	for (unsigned int i = 0; i < number_of_types; i++) {
		typesOfParticles.push_back(type_particle());
	}

	for (unsigned int i = 0; i < number_of_particles; i++) {
		particle* p = new particle(SIZE_ENV, &typesOfParticles[rand() % number_of_types]);
		mainQuadTree->insert(p);
	}
}

void update() {
	RectByCenter range(sf::Vector2f(0, 0), sf::Vector2f(100.0f, 100.0f)); //query pos and radius
	std::vector<particle*> queryResult;
	std::vector<particle*> vectAllParticles;
	mainQuadTree->getAllParticles(vectAllParticles);

	for (unsigned int i = 0; i < vectAllParticles.size(); i++) {
		particle* p1 = vectAllParticles.at(i);

		//update query position
		range.center = vectAllParticles.at(i)->getPosition();
		//request
		queryResult.clear();
		mainQuadTree->queryRangeCircle(range, queryResult);

		//interact with every other particle
		for (unsigned int j = 0; j < queryResult.size(); j++) {
			particle* p2 = queryResult[j];

			if (p1->getPosition() == p2->getPosition()) continue;

			
			//calculate dist squared 
			float dist2Interaction = 100000;
			if (type_env == 0) {
				dist2Interaction = dist2(p1->getPosition(), p2->getPosition());
			}
			else if (type_env == 1) {
				dist2Interaction = dist2OnThorus(SIZE_ENV, p1->getPosition(), p2->getPosition());
			}

			//std::cout << dist2Interaction << " ";
			if (dist2Interaction < 40000) { // if close
				if (dist2Interaction < 20*20) { //collision
					sf::Vector2f pos1 = p1->getPosition();
					sf::Vector2f pos2 = p2->getPosition();
					collideCircles(pos1, pos2, 10, 10, dist2Interaction);
					p1->setPosition(pos1);
					p2->setPosition(pos2);

					p1->interactWith(p2->getPosition(), *(p2->getType()), 20);

				}
				else { // then interact
					p1->interactWith(p2->getPosition(), *(p2->getType()), std::sqrt(dist2Interaction));
				}
			}
		}
	}


	mainQuadTree->del();
	for (unsigned int i = 0; i < vectAllParticles.size(); i++) {
		particle* p1 = vectAllParticles.at(i);
		p1->update(SIZE_ENV, type_env);
		mainQuadTree->insert(p1);
	}
}

