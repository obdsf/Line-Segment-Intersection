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
	point(float x = 0, float y = 0);
	~point();
	// Member Functions
	void update(float xVal, float yVal);
	void print();
	bool eq(point p);
	// Member Variables
	float x;
	float y;

private:

};
