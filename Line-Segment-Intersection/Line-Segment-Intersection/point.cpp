
// ### # #################################################################################### # ###

// STL : Standard Template Library
#include <iostream>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "point.h"

// ### # #################################################################################### # ###

// Constructors & Destructor
point::point(float x, float y)
	:x{ x }, y{ y }
{}

point::~point() {}

// Member Functions
void point::print() {
	std::cout << "(" << x << ", " << y << ")";
	return;
}

bool point::eq(point p) {
	if (x == p.x && y == p.y) return true;
	return false;
}
