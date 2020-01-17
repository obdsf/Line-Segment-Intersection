
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <iostream>
#include <vector>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "minimum_bounding_rectangle.h"
#include "line_segment.h"

// # ### #################################################################################### ### #

// Constructors & Destructor
minimum_bounding_rectangle::minimum_bounding_rectangle(unsigned int size, bool containsLineSegments) // false by default
  : containsLineSegments{ containsLineSegments }, containerSize{ size }
  , upperLeft{}, upperRight{}, lowerLeft{}, lowerRight{}, center{}
  , containedSegments{}, containedRectangles{}
{}

minimum_bounding_rectangle::minimum_bounding_rectangle::~minimum_bounding_rectangle() {}

// Member Functions
int minimum_bounding_rectangle::add(line_segment* line) {
  if (containedSegments.size() >= containerSize) return 0; // false, container is already at max size
  if (!containsLineSegments) {
    std::cout << "error (mbr does not contain line segments)\n";
    return -1; // false, container does not contain line segments
  }
  containedSegments.push_back(line); // add new element to coresponding container
  if (line->yMax > upperLeft.y) { // expand upper boundary if needed
    upperLeft.y = line->yMax;
    upperRight.y = upperLeft.y;
  }
  if (line->yMin < lowerLeft.y) { // expand lower boundary if needed
    lowerLeft.y = line->yMin;
    lowerRight.y = lowerLeft.y;
  }
  if (line->xMin < upperLeft.x) { // expand left boundary if needed
    upperLeft.x = line->xMin;
    lowerLeft.x = upperLeft.x;
  }
  if (line->xMax > upperRight.x) { // expand right boundary if needed
    upperRight.x = line->xMax;
    lowerRight.x = upperRight.x;
  }
  return 1; // true, new element added
}

int minimum_bounding_rectangle::add(minimum_bounding_rectangle* rectangle) {
  if (containedRectangles.size() >= containerSize) return 0; // false, container is already at max size
  if (containsLineSegments) {
    std::cout << "error (mbr does not contain rectangles)\n";
    return -1; // false, container does not contain rectangles
  }
  containedRectangles.push_back(rectangle); // add new element to coresponding container
  if (rectangle->upperLeft.y > upperLeft.y) { // expand upper boundary if needed
    upperLeft.y = rectangle->upperLeft.y;
    upperRight.y = upperLeft.y;
  }
  if (rectangle->lowerLeft.y < lowerLeft.y) { // expand lower boundary if needed
    lowerLeft.y = rectangle->lowerLeft.y;
    lowerRight.y = lowerLeft.y;
  }
  if (rectangle->upperLeft.x < upperLeft.x) { // expand left boundary if needed
    upperLeft.x = rectangle->upperLeft.x;
    lowerLeft.x = upperLeft.x;
  }
  if (rectangle->upperRight.x > upperRight.x) { // expand right boundary if needed
    upperRight.x = rectangle->upperRight.x;
    lowerRight.x = upperRight.x;
  }
  return 1; // true, new element added
}

bool minimum_bounding_rectangle::partially_contains(line_segment& line) { // true iff line's mid point is inside the mbr
  if (line.midPoint.x <= upperRight.x && line.midPoint.x >= upperLeft.x && line.midPoint.y <= upperLeft.y && line.midPoint.y >= lowerLeft.y) return true;
  return false;
}

bool minimum_bounding_rectangle::partially_contains(minimum_bounding_rectangle& rectangle) { // true iff rectangle's mid point is inside the mbr
  if (rectangle.center.x <= upperRight.x && rectangle.center.x >= upperLeft.x && rectangle.center.y <= upperLeft.y && rectangle.center.y >= lowerLeft.y) return true;
  return false;
}

bool minimum_bounding_rectangle::contains(line_segment& line) { // true iff line is completely inside the mbr
  if (line.lowerEndPoint.y >= lowerLeft.y && line.upperEndPoint.y <= upperLeft.y && line.leftmostEndPoint.x >= upperLeft.x && line.rightmostEndPoint.x <= upperRight.x)
    return true;
  return false;
}

bool minimum_bounding_rectangle::contains(minimum_bounding_rectangle& rectangle) { // true iff rectangle is completely inside the mbr
  if (rectangle.lowerLeft.y >= lowerLeft.y && rectangle.upperLeft.y <= upperLeft.y && rectangle.upperLeft.x >= upperLeft.x && rectangle.upperRight.x <= upperRight.x)
    return true;
  return false;
}

void minimum_bounding_rectangle::clear(bool containsLineSegments) {
  if (this->containsLineSegments) containedSegments.clear(); // clear the correct sub-container
  else containedRectangles.clear();
  if (containsLineSegments) this->containsLineSegments ? this->containsLineSegments = false : this->containsLineSegments = true; // toggle container type if needed
  return;
}
