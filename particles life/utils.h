#ifndef UTILS
#define UTILS

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <time.h>



sf::Vector2f add(const sf::Vector2f a, const sf::Vector2f b);
sf::Vector3f add(const sf::Vector3f a, const sf::Vector3f b);

sf::Vector2f sub(const sf::Vector2f a, const sf::Vector2f b);
sf::Vector3f sub(const sf::Vector3f a, const sf::Vector3f b);

sf::Vector2f mult(const sf::Vector2f a, const sf::Vector2f b);
sf::Vector3f mult(const sf::Vector3f a, const sf::Vector3f b);


sf::Vector2f mult(const sf::Vector2f a, const float le);
sf::Vector3f mult(const sf::Vector3f a, const float le);


float dist(const sf::Vector2f p1, const sf::Vector2f p2);
float dist2(const sf::Vector2f p1, const sf::Vector2f p2);

// random float between 0 and 1
float randFloat(); 
// random float between -1 and 1
float randFloatNP(); 


void collideCircles(sf::Vector2f& pos1, sf::Vector2f& pos2, float radius1, float radius2, float d2);


sf::Vector3f HSVtoRGB(float H, float S, float V); // h:0-360.0, s:0.0-1.0, v:0.0-1.0
sf::Vector3f RGBtoHSV(float R, float G, float B); // (0-255, 0-255, 0-255) -> (0-360.0, 0.0-1.0, 0.0-1.0)


void coloredStdCout(std::string str, sf::Vector3f color); // cout dans 7 couleurs possibles, on selectionne la plus proche de celle passee en parametre. Blanc par défaut



std::string rgbToHex(int r, int g, int b);



#endif