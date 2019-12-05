#include <iostream>
#include "point.h"
#include "line_segment.h"

int main() {
	std::cout << "Hello World!\n";
	point p;
	p.print();
	std::cout << '\n';
	point q{ 1, 1 };
	q.print();
	std::cout << '\n';
	if (p.eq(q)) {
		std::cout << "p and q are equal!\n";
	} else {
		std::cout << "p and q are not equal :/\n";
	}
	if (q.eq(q)) {
		std::cout << "q and q are equal!\n";
	} else {
		std::cout << "q and q are not equal :/\n";
	}
	line_segment l{ p, q };
	l.print();
	std::cout << '\n';
	point p2{ 1, 0 }, q2{ 0, 1 };
	line_segment l2{ q2, p2 };
	l2.print();
	std::cout << '\n';

	std::string kappa{ "kappa" };
	std::string keepo{ "keepo" };
	std::cout << kappa + keepo;

	return 0;
}

int slopeOrientation() {

	return 0;
}
