#include "engine.h"



sf::Vector2f SIZE_ENV = sf::Vector2f(2000, 2000); //Taille environnement 
std::vector <type_particle> typesOfParticles;
std::vector<particle*> vectAllParticles;

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

unsigned int ITERATIONS_COLLISIONS = 10;


void initEngine(){
	int seed = time(nullptr);
	std::cout << "\n\nseed : " << seed << std::endl;
	srand(seed);

	typesOfParticles.clear();

	if (mainQuadTree != nullptr) {
		for (unsigned int i = 0; i < vectAllParticles.size(); i++) {
			delete vectAllParticles.at(i);
		}

		mainQuadTree->del();
		vectAllParticles.clear();
		delete mainQuadTree;
	}

	// le rayon du quadtree est un peu plus grand que l'environnement afin de ne pas oublier les particules
	// qui sont exactement sur le bord
	RectByCenter rectQ(mult(SIZE_ENV, 0.5), mult(SIZE_ENV, 0.9));
	mainQuadTree = new quadtree(rectQ);

	unsigned int number_of_types = 5;
	unsigned int number_of_particles = 640;

	for (unsigned int i = 0; i < number_of_types; i++) {
		typesOfParticles.push_back(type_particle());
	}


	for (unsigned int i = 0; i < number_of_particles; i++) {
		particle* p = new particle(SIZE_ENV, &typesOfParticles[rand() % number_of_types]);

		/* positionner les particules en forme de grille dans un env de taille 2000 avec 6400 particules: 
		sf::Vector2f _p = sf::Vector2f((i*25)%((int) SIZE_ENV.x), (i/80 * 25));
		p->setPosition(_p); */

		mainQuadTree->insert(p);
		vectAllParticles.push_back(p);
	}
}

void update() {
	RectByCenter range(sf::Vector2f(0, 0), sf::Vector2f(200.0f, 200.0f)); //query pos and radius
	std::vector<particle*> queryResult;

	// Update forces
	for (unsigned int i = 0; i < vectAllParticles.size(); i++) {
		particle* p1 = vectAllParticles.at(i);

		// Update query position
		range.center = vectAllParticles.at(i)->getPosition();

		// Request nearby particles from the Quadtree
		queryResult.clear();
		if (type_env == 0 || type_env == 2) {
			mainQuadTree->queryRangeCircle(range, queryResult);
		}
		else if (type_env == 1) {
			mainQuadTree->queryRangeInThorusEnv(range, SIZE_ENV, queryResult);
		}

		// Interact with every other nearby particle
		for (unsigned int j = 0; j < queryResult.size(); j++) {
			particle* p2 = queryResult[j];

			if (p1->getPosition() == p2->getPosition()) continue; // Skip interaction with self

			// Calculate distance squared 
			float dist2Interaction = 40000.0f;
			if (type_env == 0 || type_env == 2) {
				dist2Interaction = dist2(p1->getPosition(), p2->getPosition());
			}
			else if (type_env == 1) {
				dist2Interaction = dist2OnThorus(SIZE_ENV, p1->getPosition(), p2->getPosition());
			}

			if (dist2Interaction < 40000.0f) { // if close without any collision
				p1->interactWith(p2->getPosition(), *(p2->getType()), std::sqrt(dist2Interaction));
			}
		}
	}


	//update position relative to forces
	mainQuadTree->del();
	for (unsigned int i = 0; i < vectAllParticles.size(); i++) {
		particle* p1 = vectAllParticles.at(i);
		p1->update(SIZE_ENV, type_env);
		mainQuadTree->insert(p1);
	}


	//resolve superpositions
	range.radius = sf::Vector2f(25, 25);
	for (unsigned int it = 0; it < ITERATIONS_COLLISIONS; it++) { //number of iterations (precision)
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
				if (type_env == 0 ||type_env == 2) {
					dist2Interaction = dist2(p1->getPosition(), p2->getPosition());
				}
				else if (type_env == 1) {
					dist2Interaction = dist2OnThorus(SIZE_ENV, p1->getPosition(), p2->getPosition());
				}

				if (dist2Interaction < 20 * 20) { //collision
					sf::Vector2f pos1 = p1->getPosition();
					sf::Vector2f pos2 = p2->getPosition();
					collideCircles(pos1, pos2, 10, 10, dist2Interaction);
					p1->setPosition(pos1);
					p2->setPosition(pos2);

					if (type_env == 0 ||type_env == 2) {
						pos1 = p1->getPosition();
						pos2 = p2->getPosition();
						sf::Vector2f np1 = sf::Vector2f(std::fmax(0, std::fmin(SIZE_ENV.x, pos1.x)), std::fmax(0, std::fmin(SIZE_ENV.y, pos1.y)));
						sf::Vector2f np2 = sf::Vector2f(std::fmax(0, std::fmin(SIZE_ENV.x, pos2.x)), std::fmax(0, std::fmin(SIZE_ENV.y, pos2.y)));

						p1->setPosition(np1);
						p2->setPosition(np2);
					}
				}
			}
		}
	}

	//update position
	mainQuadTree->del();
	for (unsigned int i = 0; i < vectAllParticles.size(); i++) {
		particle* p1 = vectAllParticles.at(i);
		mainQuadTree->insert(p1);
	}
}

