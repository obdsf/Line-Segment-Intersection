
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

unsigned int minimum_bounding_rectangle::uniqueID{ 0 };

// Constructors & Destructor
minimum_bounding_rectangle::minimum_bounding_rectangle(line_segment* line, minimum_bounding_rectangle* parent, unsigned int size)
  : upperLeft{ line->xMin, line->yMax }, upperRight{ line->xMax, line->yMax }
  , lowerLeft{ line->xMin, line->yMin }, lowerRight{ line->xMax, line->yMin }
  , center{ line->midPoint.x, line->midPoint.y }, containedRectangles{}, m_parent{ parent }
  , m_isLeaf{ true }, m_isRoot{ false }, m_containerSize{ size }
  , m_nodeID{ ++uniqueID }
{
  containedSegments.push_back(line);
  if (m_parent == nullptr) m_isRoot = true;
}

minimum_bounding_rectangle::minimum_bounding_rectangle(minimum_bounding_rectangle* rectangle, minimum_bounding_rectangle* parent, unsigned int size)
  : upperLeft{ rectangle->upperLeft }, upperRight{ rectangle->upperRight }
  , lowerLeft{ rectangle->lowerLeft }, lowerRight{ rectangle->lowerRight }
  , center{ rectangle->center }, containedRectangles{}, m_parent{ parent }
  , m_isLeaf{ false }, m_isRoot{ false }, m_containerSize{ size }
  , m_nodeID{ ++uniqueID }
{
  containedRectangles.push_back(rectangle);
  if (m_parent == nullptr) m_isRoot = true;
}

minimum_bounding_rectangle::minimum_bounding_rectangle::~minimum_bounding_rectangle() {}

// Member Functions
int minimum_bounding_rectangle::add(line_segment* line) {
  if(!m_isRoot) if (containedSegments.size() >= m_containerSize) return 0; // false, container is already at max size iff it is not the root
  if (!m_isLeaf) {
    std::cout << "error (mbr does not contain line segments)\n";
    return -1; // false, container does not contain line segments
  }
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
  containedSegments.push_back(line); // add new element to coresponding container
  return 1; // true, new element added
}

int minimum_bounding_rectangle::add(minimum_bounding_rectangle* rectangle) {
  if (!m_isRoot) if (containedRectangles.size() >= m_containerSize) return 0; // false, container is already at max size iff it is not the root
  if (m_isLeaf) {
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

unsigned int minimum_bounding_rectangle::size() {
  if (m_isLeaf) return containedSegments.size();
  else return containedRectangles.size();
}

unsigned int minimum_bounding_rectangle::maxSize() {
  if (m_isRoot) return INFINITY;
  else return m_containerSize;
}

unsigned int minimum_bounding_rectangle::minSize() {
  if (m_isRoot) return 0;
  else return m_containerSize / 2;
}

void minimum_bounding_rectangle::clear(bool m_isLeaf) {
  if (this->m_isLeaf) containedSegments.clear(); // clear the correct sub-container
  else containedRectangles.clear();
  if (m_isLeaf) this->m_isLeaf ? this->m_isLeaf = false : this->m_isLeaf = true; // toggle container type if needed
  return;
}

void minimum_bounding_rectangle::makeRoot() {
  m_isRoot = true;
  m_parent = nullptr;
  return;
}

void minimum_bounding_rectangle::makeRegularNode(minimum_bounding_rectangle* parent) {
  m_isRoot = false;
  m_parent = parent;
  return;
}

unsigned int minimum_bounding_rectangle::type() {
  if (m_isRoot) return 0;
  else if (m_isLeaf) return 2;
  else return 1;
}

void minimum_bounding_rectangle::resetID() {
  uniqueID = 0;
  return;
}
