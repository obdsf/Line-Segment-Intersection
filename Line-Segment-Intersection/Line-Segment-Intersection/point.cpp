
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <iostream>
#include <cmath>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "global.h"
#include "point.h"

// # ### #################################################################################### ### #

// Constructors & Destructor
point::point(const double& x, const double& y)
	:x{ x }, y{ y }, lineSeg{ nullptr }
{}

point::~point() {}

// Member Functions
void point::update(const point& p) {
	x = p.x;
	y = p.y;
}

void point::update(const double& x, const double& y) {
	this->x = x;
	this->y = y;
}

void point::print() {
	std::cout << "(" << x << ", " << y << ")\n";
	return;
}

bool point::eq(const point& p) {
	//if (x == p.x && y == p.y) return true;
	double xDif{ abs(x - p.x) };
	double yDif{ abs(y - p.y) };
	if (xDif < lineThickness && yDif < lineThickness) return true; // floating point arithmetic requires sacrifices
	return false;
}

void point::setLineSeg(line_segment& newLineSeg) {
	lineSeg = &newLineSeg;
}

double point::distance(point& p) {
	return std::sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
}

bool point::operator==(point p) {
	double xDif{ abs(x - p.x) };
	double yDif{ abs(y - p.y) };
	if (xDif < lineThickness && yDif < lineThickness) return true; // floating point arithmetic requires sacrifices
	return false;
}
