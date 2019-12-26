#pragma once

// ### # #################################################################################### # ###

// STL : Standard Template Library
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "global.h"
#include "line_segment.h"

// ### # #################################################################################### # ###

class sweep_line {
public:
	// Constructors & Destructor
	sweep_line();
	~sweep_line();
	// Member Functions
	void reset();
	void advance();
	void advance(const float& position);
	bool reachedEnd();
	// Member Variables
	line_segment sweep;
private:
	float step;
	int start;
	int end;
};
