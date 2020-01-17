#pragma once
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <vector>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "line_segment.h"

// # ### #################################################################################### ### #

class minimum_bounding_rectangle {
// Constructors & Destructor
public:
	minimum_bounding_rectangle(unsigned int size, bool containsLineSegments = false);
	~minimum_bounding_rectangle();

// Member Functions
public:
	int add(line_segment* line); // adds line segment iff mbr contains line segments
	int add(minimum_bounding_rectangle* rectangle); // adds mbr iff this->mbr contains mbrs
	bool partially_contains(line_segment& line); // returns true iff mbr contains the center point of given line segment
	bool partially_contains(minimum_bounding_rectangle& rectangle); // returns true iff mbr contains the center point of given rectangle
	bool contains(line_segment& line); // returns true iff mbr contains the whole line segment
	bool contains(minimum_bounding_rectangle& rectangle); // returns true iff mbr contains the whole rectangle
	void clear(bool containsLineSegments = false); // clears the corresponding container (vector) and if bool is true it toggles the contained type
// Member Variables
public:
	bool containsLineSegments; // type of elements contained in this mbr
	unsigned int containerSize;
	point upperLeft; // upper left boundary point
	point upperRight; // upper right boundary point
	point lowerLeft; // lower left boundary point
	point lowerRight; // lower right boundary point
	point center; // center point
	std::vector<line_segment*> containedSegments; // contained segments, it's empty if containsLineSegments is false (mbr is leaf in R-tree)
	std::vector<minimum_bounding_rectangle*> containedRectangles; // contained rectangles, it's empty if containsLineSegments if true (mbr is inner node in R-tree)
};
