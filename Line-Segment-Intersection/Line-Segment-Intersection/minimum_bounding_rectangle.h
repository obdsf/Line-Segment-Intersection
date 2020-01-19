#pragma once
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <vector>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "global.h"
#include "line_segment.h"

// # ### #################################################################################### ### #

class minimum_bounding_rectangle {
// Constructors & Destructor
public:
	minimum_bounding_rectangle(line_segment& line, minimum_bounding_rectangle* parent = nullptr, unsigned int size = defaultRTreeNodeSize);
	minimum_bounding_rectangle(minimum_bounding_rectangle& rectangle, minimum_bounding_rectangle* parent = nullptr, unsigned int size = defaultRTreeNodeSize);
	~minimum_bounding_rectangle();

// Member Functions
public:
	int add(line_segment& line); // adds line segment iff mbr contains line segments
	int add(minimum_bounding_rectangle& rectangle); // adds rectangle iff mbr contains rectangles
	int remove(line_segment& line); // removes line segment iff mbr contains line segments
	int remove(minimum_bounding_rectangle& rectangle); // removes rectangle iff mbr contains rectangles
	void refreshBoundaries(); // re-calculates boundary points and perimeter (used when a child node has added a new line or rectangle)
	bool partially_contains(line_segment& line); // returns true iff mbr contains the center point of given line segment
	bool partially_contains(minimum_bounding_rectangle& rectangle); // returns true iff mbr contains the center point of given rectangle
	bool contains(line_segment& line); // returns true iff mbr contains the whole line segment
	bool contains(minimum_bounding_rectangle& rectangle); // returns true iff mbr contains the whole rectangle
	bool intersects(line_segment& line); // returns true iff mbr intersects any part of the line segment
	bool intersects(minimum_bounding_rectangle& rectangle); // returns true iff mbr intersects any part of the rectangle
	double hypotheticalNewPerimeter(line_segment& line); // calculates the new perimeter if given line segment was added to this mbr's sub-tree
	double hypotheticalNewPerimeter(minimum_bounding_rectangle& rectangle);
	int positionRelativeTo(line_segment& line); // returns [1-9] depending on the relative position of the line to the mbr
	int positionRelativeTo(minimum_bounding_rectangle& rectangle); // returns [1-9] depending on the relative position of the rectangle to the mbr
	unsigned int size(); // returns the current size of the container
	unsigned int maxSize(); // returns the maximum size of the container
	unsigned int minSize(); // returns the minimum size of the container
	void clear(bool isLeaf = false); // clears the corresponding container (vector) and if bool is true it toggles the contained type
	void makeRoot(); // makes this mbr the root of the R-tree it belongs to
	void makeRegularNode(minimum_bounding_rectangle* parent); // makes this mbr a regular node of the R-tree
	unsigned int type(); // 0 if mbr is root, 1 if it is inner node and 2 if it's a leaf node
	void resetID(); // resets the unique IDs assigned to mbrs
	double calcPerimeter(); // calculates the perimeter of the mbr with the current boundary points

// Member Variables
public:
	point upperLeft; // upper left boundary point
	point upperRight; // upper right boundary point
	point lowerLeft; // lower left boundary point
	point lowerRight; // lower right boundary point
	point center; // center point
	double perimeter; // the perimeter of the mbr
	std::vector<line_segment*> containedSegments; // contained segments, it's empty if containsLineSegments is false (mbr is leaf in R-tree)
	std::vector<minimum_bounding_rectangle*> containedRectangles; // contained rectangles, it's empty if containsLineSegments if true (mbr is inner node in R-tree)
private:
	minimum_bounding_rectangle* m_parent; // the parent element of this mbr
	bool m_isLeaf; // type of elements contained in this mbr
	bool m_isRoot; // true iff this mbr is the root of the R-tree it belongs to
	unsigned int m_containerSize; // the maximum number of elements this mbr can hold
	unsigned int m_nodeID;
	static unsigned int uniqueID;
};
