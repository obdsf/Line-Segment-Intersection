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
	point(const float& x = 0, const float& y = 0);
	~point();
	// Member Functions
	void update(const point& p);
	void update(const float& xVal, const float& yVal);
	void print();
	bool eq(const point& p);
	// Member Variables
	float x;
	float y;

private:

};
