#include "quadtree.h"



bool collideRectAndRect(RectByCenter rect1, RectByCenter rect2) {
    float tlx1 = rect1.center.x - rect1.radius.x;
    float tly1 = rect1.center.y - rect1.radius.y;
    float brx1 = rect1.center.x + rect1.radius.x;
    float bry1 = rect1.center.y + rect1.radius.y;

    float tlx2 = rect2.center.x - rect2.radius.x;
    float tly2 = rect2.center.y - rect2.radius.y;
    float brx2 = rect2.center.x + rect2.radius.x;
    float bry2 = rect2.center.y + rect2.radius.y;

    if ((tlx1 <= brx2) && (brx1 >= tlx2) && (tly1 <= bry2) && (bry1 >= tly2)) {
        return true;
    }
    return false;
}


bool collideRectAndRectOnThorus(RectByCenter rect1, RectByCenter rect2, sf::Vector2f envSize) {
    float dx = std::abs(rect2.center.x - rect1.center.x);
    float dy = std::abs(rect2.center.y - rect1.center.y);

    //looping
    if (dx > envSize.x / 2.0f)
        dx = envSize.x - dx;

    if (dy > envSize.y / 2.0f)
        dy = envSize.y - dy;

    //check if dx < total-radius in x and same in y
    return (dx <= rect1.radius.x + rect2.radius.x  &&  dy <= rect1.radius.y + rect2.radius.y);
}





void quadtree::del() {
    points.clear();
    if (northWest != nullptr) {
        northWest->del();
        northEast->del();
        southEast->del();
        southWest->del();
    }
    delete northWest;
    delete northEast;
    delete southEast;
    delete southWest;

    northWest = nullptr;
    northEast = nullptr;
    southEast = nullptr;
    southWest = nullptr;

    return;
}

void quadtree::getAllParticles(std::vector<particle*>& particles){
    if (northWest != nullptr) {
        northWest->getAllParticles(particles);
        northEast->getAllParticles(particles);
        southWest->getAllParticles(particles);
        southEast->getAllParticles(particles);
    }
    else {
        particles.insert(particles.end(), points.begin(), points.end());
    }
}


void quadtree::display(sf::RenderWindow& window, sf::Vector2f camPos, float zoom, sf::Vector2f offsetWindow) {
    sf::RectangleShape rect;
    sf::Vector3f color = HSVtoRGB(float (sf::Uint8 (boundary.center.x/10 + boundary.center.y/10)) * 1.4, 1.0, 1.0);
    rect.setOutlineColor(sf::Color(color.x, color.y, color.z));
    rect.setOutlineThickness(1);
    rect.setFillColor(sf::Color(0, 255, 0, 0));
    sf::Vector2f pos;

    pos = sub(add(boundary.center, camPos), boundary.radius);

    rect.setPosition(add(mult(pos, zoom), offsetWindow));
    rect.setSize(mult(boundary.radius, 2*zoom));
    window.draw(rect);
    if (northWest == nullptr) {
        return;
    }
    else {
        northWest->display(window, camPos, zoom, offsetWindow);
        northEast->display(window, camPos, zoom, offsetWindow);
        southEast->display(window, camPos, zoom, offsetWindow);
        southWest->display(window, camPos, zoom, offsetWindow);
        return;
    }
}

void quadtree::subdivide() {
    //subdivide the previous rect in 4 equal parts
    RectByCenter r;
    r.radius = mult(boundary.radius, 0.5);
    r.center.x = boundary.center.x - boundary.radius.x / 2;
    r.center.y = boundary.center.y - boundary.radius.y / 2;
    northWest = new quadtree(r);

    r.center.x = boundary.center.x + boundary.radius.x / 2;
    r.center.y = boundary.center.y - boundary.radius.y / 2;
    northEast = new quadtree(r);

    r.center.x = boundary.center.x + boundary.radius.x / 2;
    r.center.y = boundary.center.y + boundary.radius.y / 2;
    southEast = new quadtree(r);

    r.center.x = boundary.center.x - boundary.radius.x / 2;
    r.center.y = boundary.center.y + boundary.radius.y / 2;
    southWest = new quadtree(r);
};

bool quadtree::insert(particle* p) {
    if (isnan(p->getPosition().x)) return false;

    // Insérer un point dans le QuadTree
    // Ignorer les objets qui n'appartiennent pas a ce quadtree
    if ((abs(boundary.center.x - p->getPosition().x) >= boundary.radius.x) || (abs(boundary.center.y - p->getPosition().y) >= boundary.radius.y)) {
        return false; // l'objet ne doit pas être ajoute
    }

    // S'il reste de la place dans ce quadtree, y ajouter l'objet
    if (points.size() < QT_NODE_CAPACITY && northWest == nullptr) {
        points.push_back(p);
        return true;
    }

    // Sinon, subdiviser le quadtree, puis ajouter le point au nœud qui l'acceptera
    if (northWest == nullptr) {
        subdivide();

        //distribute the points to the childs
        for (int pointMoved = 0; pointMoved < points.size(); pointMoved++) {
            if (northWest->insert(points[pointMoved])) continue;
            else if (northEast->insert(points[pointMoved])) continue;
            else if (southWest->insert(points[pointMoved])) continue;
            else if (southEast->insert(points[pointMoved])) continue;
        }

        points.clear();
    }

    if (northWest->insert(p)) { return true; }
    if (northEast->insert(p)) { return true; }
    if (southWest->insert(p)) { return true; }
    if (southEast->insert(p)) { return true; }

    // Sinon, le point ne peut etre insere, pour une raison inconnue (cela ne devrait jamais arriver)
    return false;
}

void quadtree::queryRangeRect(RectByCenter range, std::vector<particle*>& pointsInRange) {
    // skip if the quadtree isn't concerned
    if (!collideRectAndRect(range, boundary)) {
        return;
    }

    //if no child, return the node points
    if (northWest == nullptr) {
        for (int p = 0; p < points.size(); p++) {
            if ((abs(points.at(p)->getPosition().x - range.center.x) < range.radius.x) &&
                (abs(points.at(p)->getPosition().y - range.center.y) < range.radius.y)) {
                pointsInRange.push_back(points.at(p));
            }
        }
        return;
    }

    // if it have childs, ask them
    northWest->queryRangeRect(range, pointsInRange);
    northEast->queryRangeRect(range, pointsInRange);
    southEast->queryRangeRect(range, pointsInRange);
    southWest->queryRangeRect(range, pointsInRange);
    return;
}

void quadtree::queryRangeCircle(RectByCenter range, std::vector<particle*>& pointsInRange) {
    // skip if the quadtree isn't concerned
    if (!collideRectAndRect(range, boundary)) {
        return;
    }

    //if no child, return the node points
    if (northWest == nullptr) {
        for (int p = 0; p < points.size(); p++) {
            if ((abs(points.at(p)->getPosition().x - range.center.x) < range.radius.x) &&
                (abs(points.at(p)->getPosition().y - range.center.y) < range.radius.y)) {

                //in rect, check if in circle
                if (dist2(points[p]->getPosition(), range.center) < range.radius.x * range.radius.x + range.radius.y * range.radius.y) {
                    pointsInRange.push_back(points.at(p));
                }
            }
        }
        return;
    }

    // if it have childs, ask them
    northWest->queryRangeCircle(range, pointsInRange);
    northEast->queryRangeCircle(range, pointsInRange);
    southEast->queryRangeCircle(range, pointsInRange);
    southWest->queryRangeCircle(range, pointsInRange);
    return;
}



void quadtree::queryRangeInThorusEnv(RectByCenter range, sf::Vector2f envSize, std::vector<particle*>& pointsInRange) {
    // skip if the quadtree isn't concerned
    if (!collideRectAndRectOnThorus(range, boundary, envSize)) {
        return;
    }

    //if no child, return the node points
    if (northWest == nullptr) {
        float px, py; 
        sf::Vector2f p_pos;
        for (int p = 0; p < points.size(); p++) {
            p_pos = points.at(p)->getPosition();
            px = p_pos.x; py = p_pos.y;
            
            if (((abs(px - range.center.x) < range.radius.x) || (abs(px + envSize.x - range.center.x) < range.radius.x) || (abs(px - envSize.x - range.center.x) < range.radius.x))
                &&
                ((abs(py - range.center.y) < range.radius.y) || (abs(py + envSize.y - range.center.y) < range.radius.y) || (abs(py - envSize.y - range.center.y) < range.radius.y))) {

                pointsInRange.push_back(points.at(p));
            }
        }
        return;
    }

    // if it have childs, ask them
    northWest->queryRangeInThorusEnv(range, envSize, pointsInRange);
    northEast->queryRangeInThorusEnv(range, envSize, pointsInRange);
    southEast->queryRangeInThorusEnv(range, envSize, pointsInRange);
    southWest->queryRangeInThorusEnv(range, envSize, pointsInRange);
    return;
}