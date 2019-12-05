#pragma once

class point {
public:
	// Constructors & Destructor
	point();
	point(float x, float y);
	~point();
	// Member Functions
	void print();
	bool eq(point p);
	// Member Variables
	float x;
	float y;

private:

};
