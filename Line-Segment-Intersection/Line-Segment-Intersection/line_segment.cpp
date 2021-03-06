
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
	, name{ "ls_" + std::to_string(uniqueID++) }
{
	calcPointsAndBoundaries();
	this->p.setLineSeg(*this);
	this->q.setLineSeg(*this);
}

line_segment::line_segment(const double& px, const double& py, const double& qx, const double& qy)
	: p{ px, py }, q{ qx, qy }, slope{ calcSlope() }, yIntercept{ calcYIntercept() }
	, name{ "ls" + std::to_string(uniqueID++) }
{
	calcPointsAndBoundaries();
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
	calcPointsAndBoundaries();
	return;
}

void line_segment::update(const double& px, const double& py, const double& qx, const double& qy) {
	p.update(px, py);
	q.update(qx, qy);
	p.setLineSeg(*this);
	q.setLineSeg(*this);
	slope = calcSlope();
	yIntercept = calcYIntercept();
	calcPointsAndBoundaries();
	return;
}

void line_segment::print() {
	std::cout
		<< name << ": "
		<< "p(" << p.x << ", " << p.y << ")"
		<< " <---> "
		<< "q(" << q.x << ", " << q.y << ")"
		<< " || slope: " << slope
		<< " || y-intercept: " << yIntercept
		<< '\n';
	std::cout << "Upper End Point: ";
	upperEndPoint.print();
	std::cout << "Lower End Point: ";
	lowerEndPoint.print();
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
	if (abs(((double)k.y - (double)slope * (double)k.x - (double)yIntercept)) <= lineThickness) return true;
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
	double x, y;
	if (parallel(l)) {
		if (isinf(slope)) {
			if (l.p.x != p.x) return false;
			else {
				if (l.lowerEndPoint.y > upperEndPoint.y || lowerEndPoint.y > l.upperEndPoint.y) return false;
				else {
					x = p.x;
					if (upperEndPoint.y > l.upperEndPoint.y) {
						y = l.upperEndPoint.y;
					} else {
						y = upperEndPoint.y;
					}
				}
			}
		} else {
			if (l.yIntercept != yIntercept) return false;
			else {
				if (l.lowerEndPoint.y > upperEndPoint.y || lowerEndPoint.y > l.upperEndPoint.y) return false;
				else {
					x = p.x;
					if (upperEndPoint.y > l.upperEndPoint.y) {
						y = l.upperEndPoint.y;
					} else {
						y = upperEndPoint.y;
					}
				}
			}
		}
	}
	if (isinf(slope)) {
		x = yIntercept;
		y = x * l.slope + l.yIntercept;
	} else if (isinf(l.slope)) {
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

double line_segment::solveForX(double y) {
	if (isinf(slope)) return p.x;
	else if (slope == 0) return FLT_MAX;
	/* returning NULL is technically the right choice since mathematically
	 * it is impossible to determine the x of a horizontal line, given a y
	 * but returning any value above distMax saves us one check in the add
	 * function of the status_structure class since all horizontal line
	 * segments in a given y must be placed last
	 * (according to the sweep line algorithm)
	 */
	else return (y - yIntercept) / slope;
}

double line_segment::solveForY(double x) {
	if (isinf(slope)) return FLT_MIN;
	/* returning INT_MIN is not mathematically correct but it is done in
	 * order to be consistent with the solveForX function
	 * (makes no difference in our calculations)
	 */
	else if (slope == 0) return p.y;
	else return slope * x + yIntercept;
}

double line_segment::calcSlope() {
	return (q.y - p.y) / (q.x - p.x);
}

double line_segment::calcYIntercept() {
	if (isinf(slope)) return p.x;
	return p.y - p.x * slope;
}

void line_segment::calcPointsAndBoundaries() {
	// upper/lower end points
	if (slope == 0) {
		if (p.x <= q.x) {
			upperEndPoint = p;
			lowerEndPoint = q;
		} else {
			upperEndPoint = q;
			lowerEndPoint = p;
		}
	} else {
		if (p.y >= q.y) {
			upperEndPoint = p;
			lowerEndPoint = q;
		} else {
			upperEndPoint = q;
			lowerEndPoint = p;
		}
	}
	// leftmost/rightmost end points
	if (isinf(slope)) {
		if (p.y >= q.y) {
			leftmostEndPoint = p;
			rightmostEndPoint = q;
		} else {
			leftmostEndPoint = q;
			rightmostEndPoint = p;
		}
	} else {
		if (p.x <= q.x) {
			leftmostEndPoint = p;
			rightmostEndPoint = q;
		} else {
			leftmostEndPoint = q;
			rightmostEndPoint = p;
		}
	}
	// middle point
	midPoint.x = (p.x + q.x) / 2;
	midPoint.y = (p.y + q.y) / 2;
	// calculate boundaries
	xMax = rightmostEndPoint.x;
	xMin = leftmostEndPoint.x;
	/*if (p.x > q.x) {
		xMax = p.x;
		xMin = q.x;
	} else {
		xMax = q.x;
		xMin = p.x;
	}*/
	yMax = upperEndPoint.y;
	yMin = lowerEndPoint.y;

	return;
}

void line_segment::resetID() {
	uniqueID = 0;
	return;
}

bool line_segment::operator==(line_segment l) {
	if (p == l.p && q == l.q || q == l.p && p == l.q) return true;
	return false;
}
