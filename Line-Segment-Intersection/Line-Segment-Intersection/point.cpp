
// ### # #################################################################################### # ###

// STL : Standard Template Library
#include <iostream>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "point.h"

// ### # #################################################################################### # ###

// Constructors & Destructor
point::point(int x, int y)
	:x{ x }, y{ y }
{}

point::~point() {}

// Member Functions
void point::update(int xVal, int yVal) {
	x = xVal;
	y = yVal;
}

void point::print() {
	std::cout << "(" << x << ", " << y << ")\n";
	return;
}

bool point::eq(point p) {
	if (x == p.x && y == p.y) return true;
	return false;
}
