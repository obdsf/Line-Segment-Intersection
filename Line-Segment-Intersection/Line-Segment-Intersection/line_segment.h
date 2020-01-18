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
	line_segment(const double& px = 0, const double& py = 0, const double& qx = 0, const double& qy = 0);
	~line_segment();

// Member Functions
public:
	void update(const point& p, const point& q);
	void update(const double& px, const double& py, const double& qx, const double& qy);
	void print();
	bool eq(const line_segment& l);
	bool parallel(const line_segment& l);
	bool collinear(const line_segment& l);
	bool partially_contains(const point& k);
	bool contains(const point& k);
	bool partially_intersects(const line_segment& l, point& k);
	bool intersects(line_segment& l, point& k);
	double solveForX(double y);
	double solveForY(double x);
	void resetID();
private:
	double calcSlope();
	double calcYIntercept();
	void calcPointsAndBoundaries();

// Member Variables
public:
	point p;
	point q;
	point upperEndPoint;
	point lowerEndPoint;
	point rightmostEndPoint;
	point leftmostEndPoint;
	point midPoint;
	double slope;
	double yIntercept;
	double xMin;
	double xMax;
	double yMin;
	double yMax;
	std::string name;
private:
	static unsigned int uniqueID;
};
