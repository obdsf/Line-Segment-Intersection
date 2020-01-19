#pragma once
// # ### #################################################################################### ### #

// STL : Standard Template Library
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers

// # ### #################################################################################### ### #

class line_segment;

class point {
public:
	// Constructors & Destructor
	point(const double& x = 0, const double& y = 0);
	~point();
	// Member Functions
	void update(const point& p);
	void update(const double& x, const double& y);
	void print();
	bool eq(const point& p);
	void setLineSeg(line_segment& newLineSeg);
	double distance(point& p);
	// Overloaded Operators
	bool operator==(point p);
	// Member Variables
	double x;
	double y;
	line_segment* lineSeg;
private:

};
