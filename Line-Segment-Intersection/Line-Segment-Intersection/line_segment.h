#pragma once
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <string>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "point.h"

// # ### #################################################################################### ### #

class line_segment {
// Constructors & Destructor
public:
	line_segment(const point &p, const point &q);
	line_segment(const float& px, const float& py, const float& qx, const float& qy);
	~line_segment();

// Member Functions
public:
	void update(const point& p, const point& q);
	void update(const float& px, const float& py, const float& qx, const float& qy);
	void print();
	bool eq(const line_segment& l);
	bool parallel(const line_segment& l);
	bool collinear(const line_segment& l);
	bool partially_contains(const point& k);
	bool contains(const point& k);
	bool partially_intersects(const line_segment& l, point& k);
	bool intersects(line_segment& l, point& k);
	float solveForX(float y);
	float solveForY(float x);
	void resetID();
private:
	float calcSlope();
	float calcYIntercept();
	void calcEndPointsAndBoundaries();

// Member Variables
public:
	point p;
	point q;
	point upperEndPoint;
	point lowerEndPoint;
	float slope;
	float yIntercept;
	float xMin;
	float xMax;
	float yMin;
	float yMax;
	std::string name;
	static unsigned int uniqueID;
};
