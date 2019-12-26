#pragma once

// ### # #################################################################################### # ###

// STL : Standard Template Library
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers

// ### # #################################################################################### # ###

class line_segment;

class point {
public:
	// Constructors & Destructor
	point(const float& x = 0, const float& y = 0);
	~point();
	// Member Functions
	void update(const point& p);
	void update(const float& x, const float& y);
	void print();
	bool eq(const point& p);
	void setLineSeg(line_segment& newLineSeg);
	// Member Variables
	float x;
	float y;
	line_segment* lineSeg;

private:

};
