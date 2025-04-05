#pragma once
#include "utils.h"
#include "particle.h"


class particle;


class RectByCenter
{
public:
	sf::Vector2f center; sf::Vector2f radius;
	RectByCenter() {
		center = sf::Vector2f(0, 0);
		radius = sf::Vector2f(1, 1);
	}

	RectByCenter(sf::Vector2f c, sf::Vector2f r) {
		center = c;
		radius = r;
	}
};


class quadtree
{
public:
	// Enfants/*
	quadtree* northWest;
	quadtree* northEast;
	quadtree* southWest;
	quadtree* southEast;

	// Constante arbitraire indiquant combien d'elements peuvent �tre stock�s dans ce n�ud de quadtree
	const int QT_NODE_CAPACITY = 40;


	// Zone de d�limitation align�e sur l'axe (repr�sent�e par sa demi-dimension et son centre)
	// repr�sentant les limites de ce quadtree
	RectByCenter boundary;

	// Points de ce n�eud de quadtree
	std::vector <particle*> points;


	// M�thodes
	quadtree(RectByCenter bd) {
		this->boundary = bd; points.clear();
		northWest = nullptr; northEast = nullptr; southWest = nullptr; southEast = nullptr;
	};

	bool insert(particle* p);
	void getAllParticles(std::vector<particle*>& particles);
	void subdivide(); // creer quatre enfants permettant de diviser ce quadrant en quatre quadrants d'�gales dimensions
	void queryRangeRect(RectByCenter range, std::vector<particle*>& pointsInRange);
	void queryRangeCircle(RectByCenter range, std::vector<particle*>& pointsInRange);
	void queryRangeInThorusEnv(RectByCenter range, sf::Vector2f envSize, std::vector<particle*>& pointsInRange);
	void display(sf::RenderWindow& window, sf::Vector2f camPos, float zoom, sf::Vector2f offsetWindow);
	void del();  // delete the QuadTree from the actual pos. to the leafs

};


