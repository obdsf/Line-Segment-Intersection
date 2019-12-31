
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <iostream>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "global.h"
#include "point.h"
#include "line_segment.h"

// # ### #################################################################################### ### #

unsigned int line_segment::uniqueID{ 0 };

// Constructors & Destructor
line_segment::line_segment(const point& p, const point& q)
	: p{ p }, q{ q }, slope{ calcSlope() }, yIntercept{ calcYIntercept() }
	, name{ "ls" + std::to_string(uniqueID++) }
{
	calcEndPointsAndBoundaries();
	this->p.setLineSeg(*this);
	this->q.setLineSeg(*this);
}

line_segment::line_segment(const float& px, const float& py, const float& qx, const float& qy)
	: p{ px, py }, q{ qx, qy }, slope{ calcSlope() }, yIntercept{ calcYIntercept() }
	, name{ "ls" + std::to_string(uniqueID++) }
{
	calcEndPointsAndBoundaries();
	p.setLineSeg(*this);
	q.setLineSeg(*this);
}

line_segment::~line_segment() {}

// Member Functions
void line_segment::update(const point& p, const point& q) {
	this->p.update(p);
	this->q.update(q);
	this->p.setLineSeg(*this);
	this->q.setLineSeg(*this);
	slope = calcSlope();
	yIntercept = calcYIntercept();
	calcEndPointsAndBoundaries();
	return;
}

void line_segment::update(const float& px, const float& py, const float& qx, const float& qy) {
	p.update(px, py);
	q.update(qx, qy);
	p.setLineSeg(*this);
	q.setLineSeg(*this);
	slope = calcSlope();
	yIntercept = calcYIntercept();
	calcEndPointsAndBoundaries();
	return;
}

void line_segment::print() {
	std::cout
		<< "(" << p.x << ", " << p.y << ")"
		<< " <---> "
		<< "(" << q.x << ", " << q.y << ")"
		<< " || slope: " << slope
		<< " || y-intercept: " << yIntercept
		<< '\n';
	return;
}

bool line_segment::eq(const line_segment& l) {
	if (p.eq(l.p) && q.eq(l.q) || q.eq(l.p) && p.eq(l.q)) return true;
	return false;
}

bool line_segment::parallel(const line_segment& l) {
	if (slope == l.slope) return true;
	return false;
}

bool line_segment::collinear(const line_segment& l) {
	if (parallel(l) && yIntercept == l.yIntercept) return true;
	return false;
}

bool line_segment::partially_contains(const point& k) {
	if (abs(((double)k.y - (double)slope * (double)k.x - (double)yIntercept)) <= g_precision) return true;
	/* Without the cast there is a risk of arithmetic overflow (C26451)
		* If calculated value is too big and overflows the remainders
		* might be less than 0.5 and thus return true, falsely.
		*/
	return false;
}

bool line_segment::contains(const point& k) {
	if (partially_contains(k)) {
		if (k.x <= xMax && k.x >= xMin && k.y <= yMax && k.y >= yMin) return true;
	}
	return false;
}

bool line_segment::partially_intersects(const line_segment& l, point& k) {
	if (parallel(l)) return false;
	float x, y;
	if (slope == INFINITY) {
		x = yIntercept;
		y = x * l.slope + l.yIntercept;
	} else if (l.slope == INFINITY) {
		x = l.yIntercept;
		y = x * slope + yIntercept;
	} else {
		x = (l.yIntercept - yIntercept) / (slope - l.slope); // slope * x + yIntercept = l.slope * x + l.yIntercept => x * (slope - l.slope) = l.yIntercept - yIntercept => x = (l.yIntercept - yIntercept) / (slope - l.slope)
		y = x * slope + yIntercept;
	}
	k.update(x, y);
	return true;
}

bool line_segment::intersects(line_segment& l, point& k) {
	if (partially_intersects(l, k)) {
		if (contains(k) && l.contains(k)) return true;
	}
	return false;
}

float line_segment::solveForX(float y) {
	if (slope == INFINITY) return p.x;
	else return (y - yIntercept) / slope;
}

float line_segment::solveForY(float x) {
	return slope * x + yIntercept;
}

float line_segment::calcSlope() {
	return (q.y - p.y) / (q.x - p.x);
}

float line_segment::calcYIntercept() {
	if (slope == INFINITY) return p.x;
	return p.y - p.x * slope;
}

void line_segment::calcEndPointsAndBoundaries() {
	// calculate end points
	if (slope == 0) {
		if (p.x < q.x) {
			upperEndPoint = p;
			lowerEndPoint = q;
		} else {
			upperEndPoint = q;
			lowerEndPoint = p;
		}
	} else {
		if (p.y > q.y) {
			upperEndPoint = p;
			lowerEndPoint = q;
		} else {
			upperEndPoint = q;
			lowerEndPoint = p;
		}
	}
	// calculate boundaries
	if (p.x > q.x) {
		xMax = p.x;
		xMin = q.x;
	} else {
		xMax = q.x;
		xMin = p.x;
	}
	yMax = upperEndPoint.y;
	yMin = lowerEndPoint.y;

	return;
}

void line_segment::resetID() {
	uniqueID = 0;
	return;
}
