#pragma once

// ### # #################################################################################### # ###

// STL : Standard Template Library
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers

// ### # #################################################################################### # ###

class point {
public:
	// Constructors & Destructor
	point(int x = 0, int y = 0);
	~point();
	// Member Functions
	void update(int xVal, int yVal);
	void print();
	bool eq(point p);
	// Member Variables
	int x;
	int y;

private:

};
