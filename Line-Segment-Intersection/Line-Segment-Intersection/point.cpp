
// ### # #################################################################################### # ###

// STL : Standard Template Library
#include <iostream>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "point.h"

// ### # #################################################################################### # ###

// Constructors & Destructor
point::point(const float& x, const float& y)
	:x{ x }, y{ y }, lineSeg{ nullptr }
{}

point::~point() {}

// Member Functions
void point::update(const point& p) {
	x = p.x;
	y = p.y;
}

void point::update(const float& x, const float& y) {
	this->x = x;
	this->y = y;
}

void point::print() {
	std::cout << "(" << x << ", " << y << ")\n";
	return;
}

bool point::eq(const point& p) {
	if (x == p.x && y == p.y) return true;
	return false;
}

void point::setLineSeg(line_segment& newLineSeg) {
	lineSeg = &newLineSeg;
}
