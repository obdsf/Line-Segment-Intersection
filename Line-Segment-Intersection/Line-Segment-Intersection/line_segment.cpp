#include <iostream>
#include "point.h"
#include "line_segment.h"

// Constructors & Destructor
line_segment::line_segment(point &p, point &q)
	:p{ &p }, q{ &q }, slope{ calcSlope() }
{
	p.definesLineSegment = true;
	q.definesLineSegment = true;
}

line_segment::~line_segment() {
	p->definesLineSegment = false;
	q->definesLineSegment = false;
}

// Member Functions
void line_segment::print() {
	p->print();
	std::cout << "<--->";
	q->print();
	std::cout << " || slope: " << slope;
	return;
}

bool line_segment::eq(line_segment &l) {
	if (p->eq(*l.p) && q->eq(*l.q) || q->eq(*l.p) && p->eq(*l.q)) return true;

	return false;
}

float line_segment::calcSlope() {
	return (q->y - p->y) / (q->x - p->x);
}

int line_segment::slopeOrientation(line_segment &l) {
	return slope-l.slope;
}
