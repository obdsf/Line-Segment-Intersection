#include <iostream>
#include "point.h"
#include "line_segment.h"

int main() {
	std::cout << "Hello World!\n";
	point p{ 0, -1 };
	point q{ 1, 4.5 };
	std::cout << p.definesLineSegment << '\n';
	line_segment l{ p, q };
	std::cout << l.p->definesLineSegment << '\n';
	l.print();
	std::cout << '\n';
	l.~line_segment();
	std::cout << p.definesLineSegment << '\n';

	return 0;
}
