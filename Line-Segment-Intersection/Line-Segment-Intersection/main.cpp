#include <iostream>
#include "point.h"
#include "line_segment.h"

int main() {
	std::cout << "Hello World!\n";
	point p{ 0, -1 };
	point q{ 1, 4.5 };
	line_segment l{ p, q };
	l.print();
	std::cout << '\n';

	return 0;
}

int slopeOrientation() {

	return 0;
}
