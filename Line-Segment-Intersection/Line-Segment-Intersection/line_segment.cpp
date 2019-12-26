
// ### # #################################################################################### # ###

// STL : Standard Template Library
#include <iostream>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "global.h"
#include "point.h"
#include "line_segment.h"

// ### # #################################################################################### # ###

// Constructors & Destructor
line_segment::line_segment(const point& p, const point& q)
	:p{ p }, q{ q }, slope{ calcSlope() }, yIntercept{ calcYIntercept() }
{
	calcLineBoundaries(p.x, p.y, q.x, q.y);
	this->p.setLineSeg(*this);
	this->q.setLineSeg(*this);
}

line_segment::line_segment(const float& px, const float& py, const float& qx, const float& qy)
	: p{ px, py }, q{ qx, qy }, slope{ calcSlope() }, yIntercept{ calcYIntercept() }
{
	calcLineBoundaries(px, py, qx, qy);
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
	calcLineBoundaries(p.x, p.y, q.x, q.y);
	return;
}

void line_segment::update(const float& px, const float& py, const float& qx, const float& qy) {
	p.update(px, py);
	q.update(qx, qy);
	p.setLineSeg(*this);
	q.setLineSeg(*this);
	slope = calcSlope();
	yIntercept = calcYIntercept();
	calcLineBoundaries(px, py, qx, qy);
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

float line_segment::calcSlope() {
	return (q.y - p.y) / (q.x - p.x);
}

float line_segment::calcYIntercept() {
	if (slope == INFINITY) return p.x;
	return p.y - p.x * slope;
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
	if (k.x <= xMax && k.x >= xMin && k.y <= yMax && k.y >= yMin) return true;
	return false;
}

bool line_segment::contains(const point& k) {
	if (partially_contains(k)) {
		if (1) {
			if (abs(((double)k.y - (double)slope * (double)k.x - (double)yIntercept)) <= g_precision) return true;
			/* Without the cast there is a risk of arithmetic overflow (C26451)
			 * If calculated value is too big and overflows the remainders
			  * might be less than 0.5 and thus return true, falsely.
			 */
		} else {
			if(k.y == slope * k.x + yIntercept) return true;
		}
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

void line_segment::calcLineBoundaries(const float& px, const float& py, const float& qx, const float& qy) {
	if (px > qx) {
		xMax = px;
		xMin = qx;
	} else {
		xMax = qx;
		xMin = px;
	}
	if (py > qy) {
		yMax = py;
		yMin = qy;
	} else {
		yMax = qy;
		yMin = py;
	}
	return;
}
