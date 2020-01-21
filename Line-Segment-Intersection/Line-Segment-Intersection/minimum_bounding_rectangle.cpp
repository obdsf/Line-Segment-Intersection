
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <iostream>
#include <vector>
#include <algorithm>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "minimum_bounding_rectangle.h"
#include "line_segment.h"

// # ### #################################################################################### ### #

unsigned int minimum_bounding_rectangle::uniqueID{ 0 };

// Constructors & Destructor
minimum_bounding_rectangle::minimum_bounding_rectangle(line_segment& line, minimum_bounding_rectangle* parent, unsigned int size)
  : upperLeft{ line.xMin, line.yMax }, upperRight{ line.xMax, line.yMax }
  , lowerLeft{ line.xMin, line.yMin }, lowerRight{ line.xMax, line.yMin }
  , center{ line.midPoint.x, line.midPoint.y }, perimeter{ calcPerimeter() }
  , containedRectangles{}, containedSegments{}, m_parent{ parent }
  , isLeaf{ true }, isRoot{ false }, m_containerSize{ size }
  , nodeID{ uniqueID++ }
{
  containedSegments.push_back(&line);
  if (m_parent == nullptr) isRoot = true;
}

minimum_bounding_rectangle::minimum_bounding_rectangle(minimum_bounding_rectangle& rectangle, minimum_bounding_rectangle* parent, unsigned int size)
  : upperLeft{ rectangle.upperLeft }, upperRight{ rectangle.upperRight }
  , lowerLeft{ rectangle.lowerLeft }, lowerRight{ rectangle.lowerRight }
  , center{ rectangle.center }, perimeter{ calcPerimeter() }
  , containedRectangles{}, containedSegments{}, m_parent{ parent }
  , isLeaf{ false }, isRoot{ false }, m_containerSize{ size }
  , nodeID{ uniqueID++ }
{
  containedRectangles.push_back(&rectangle);
  if (m_parent == nullptr) isRoot = true;
}

minimum_bounding_rectangle::minimum_bounding_rectangle(const minimum_bounding_rectangle& mbr)
  : upperLeft{ mbr.upperLeft }, upperRight{ mbr.upperRight }
  , lowerLeft{ mbr.lowerLeft }, lowerRight{ mbr.lowerRight }
  , center{ mbr.center }, perimeter{ mbr.perimeter }
  , containedRectangles{ mbr.containedRectangles }, containedSegments{ mbr.containedSegments }
  , m_parent{ mbr.m_parent }
  , isLeaf{ mbr.isLeaf }, isRoot{ mbr.isRoot }, m_containerSize{ mbr.m_containerSize }
  , nodeID{ uniqueID++ }
{
  std::cout << "copy constructor called!\ncopying node " << mbr.nodeID << " to node " << nodeID << '\n'; // TEMP TEST
}

minimum_bounding_rectangle::minimum_bounding_rectangle::~minimum_bounding_rectangle() {}

// Member Functions
bool minimum_bounding_rectangle::add(line_segment& line) {
  containedSegments.push_back(&line); // add new element to coresponding container
  //if (line.yMax > upperLeft.y) { // expand upper boundary if needed
  //  upperLeft.y = line.yMax;
  //  upperRight.y = upperLeft.y;
  //}
  //if (line.yMin < lowerLeft.y) { // expand lower boundary if needed
  //  lowerLeft.y = line.yMin;
  //  lowerRight.y = lowerLeft.y;
  //}
  //if (line.xMin < upperLeft.x) { // expand left boundary if needed
  //  upperLeft.x = line.xMin;
  //  lowerLeft.x = upperLeft.x;
  //}
  //if (line.xMax > upperRight.x) { // expand right boundary if needed
  //  upperRight.x = line.xMax;
  //  lowerRight.x = upperRight.x;
  //}
  adjustSubTreeBoundaries(); // adjust the subtree's boundary points
  perimeter = calcPerimeter(); // calculate new perimeter
  if (size() >= maxSize()) return 0; // false, new element added but overflow occured
  else return 1; // true, new element added without overflow
}

bool minimum_bounding_rectangle::add(minimum_bounding_rectangle& rectangle) {
  containedRectangles.push_back(&rectangle); // add new element to coresponding container
  //if (rectangle.upperLeft.y > upperLeft.y) { // expand upper boundary if needed
  //  upperLeft.y = rectangle.upperLeft.y;
  //  upperRight.y = upperLeft.y;
  //}
  //if (rectangle.lowerLeft.y < lowerLeft.y) { // expand lower boundary if needed
  //  lowerLeft.y = rectangle.lowerLeft.y;
  //  lowerRight.y = lowerLeft.y;
  //}
  //if (rectangle.upperLeft.x < upperLeft.x) { // expand left boundary if needed
  //  upperLeft.x = rectangle.upperLeft.x;
  //  lowerLeft.x = upperLeft.x;
  //}
  //if (rectangle.upperRight.x > upperRight.x) { // expand right boundary if needed
  //  upperRight.x = rectangle.upperRight.x;
  //  lowerRight.x = upperRight.x;
  //}
  adjustSubTreeBoundaries(); // adjust the subtree's boundary points
  perimeter = calcPerimeter(); // calculate new perimeter
  rectangle.m_parent = this; // set rectangles parent to be the current mbr
  if (size() >= maxSize()) return 0; // false, new element added but overflow occured
  else return 1; // true, new element added without overflow
}

int minimum_bounding_rectangle::remove(line_segment& line) {
  if (empty()) return 0; // false, container is already empty
  if (!isLeaf) return -1; // false, container does not contain line segments
  auto iter{ std::find(containedSegments.begin(), containedSegments.end(), &line) };
  if (iter == containedSegments.end()) { // container does not contain given line segment
    if (contains(line)) return -3; // false, line segment is not contained in this mbr but it is within it's boundaries (fatal error)
    else return -2; // false, line segment is not contained in this mbr and is not within it's boundaries
  } else { // line segment found within the container;
    containedSegments.erase(iter); // remove specified line segment from the container
    if (empty()) { // if container is now empty resize mbr accordingly
      upperLeft.update(0, 0);
      upperRight.update(0, 0);
      lowerLeft.update(0, 0);
      lowerRight.update(0, 0);
      perimeter = calcPerimeter(); // calculate new perimeter
      // no point in adjasting boundaries here (after the return of 2 remove this mbr and then adjust boundaries)
      return 2; // false, container is empty and needs to be deleted (unless it's the root)
    } else {
      adjustSubTreeBoundaries(); // adjust the subtree's boundary points
      return 1; // true, container updated and mbr resized
    }
  }
}

int minimum_bounding_rectangle::remove(minimum_bounding_rectangle& rectangle) {
  if (empty()) return 0; // false, container is already empty
  if (isLeaf) return -1; // false, container does not contain rectangles
  auto iter{ std::find(containedRectangles.begin(), containedRectangles.end(), &rectangle) };
  if (iter == containedRectangles.end()) { // container does not contain given rectangle
    if (contains(rectangle)) return -3; // false, rectangle is not contained in this mbr but it is within it's boundaries (fatal error)
    else return -2; // false, rectangle is not contained in this mbr and is not within it's boundaries
  } else { // rectangle found within the container;
    containedRectangles.erase(iter); // remove specified rectangle from the container
    if (empty()) { // if container is now empty resize mbr accordingly
      upperLeft.update(0, 0);
      upperRight.update(0, 0);
      lowerLeft.update(0, 0);
      lowerRight.update(0, 0);
      perimeter = calcPerimeter(); // calculate new perimeter
      // no point in adjasting boundaries here (after the return of 2 remove this mbr and then adjust boundaries)
      return 2; // false, container is empty and needs to be deleted (unless it's the root)
    } else {
      adjustSubTreeBoundaries(); // adjust the subtree's boundary points
      return 1; // true, container updated and mbr resized
    }
  }
}

void minimum_bounding_rectangle::refreshBoundaries() {
  if (isLeaf) {
    double topBound{ -369 };
    double bottomBound{ 369 };
    double leftBound{ 369 };
    double rightBound{ -369 };
    for (line_segment* lineSeg : containedSegments) {
      if (lineSeg->upperEndPoint.y > topBound) topBound = lineSeg->upperEndPoint.y;
      if (lineSeg->lowerEndPoint.y < bottomBound) bottomBound = lineSeg->lowerEndPoint.y;
      if (lineSeg->leftmostEndPoint.x < leftBound) leftBound = lineSeg->leftmostEndPoint.x;
      if (lineSeg->rightmostEndPoint.x > rightBound) rightBound = lineSeg->rightmostEndPoint.x;
    }
    upperLeft.update(leftBound, topBound);
    upperRight.update(rightBound, topBound);
    lowerLeft.update(leftBound, bottomBound);
    lowerRight.update(rightBound, bottomBound);
    perimeter = calcPerimeter(); // calculate new perimeter
  } else {
    double topBound{ -369 };
    double bottomBound{ 369 };
    double leftBound{ 369 };
    double rightBound{ -369 };
    for (minimum_bounding_rectangle* rect : containedRectangles) {
      if (rect->upperLeft.y > topBound) topBound = rect->upperLeft.y;
      if (rect->lowerLeft.y < bottomBound) bottomBound = rect->lowerLeft.y;
      if (rect->upperLeft.x < leftBound) leftBound = rect->upperLeft.x;
      if (rect->upperRight.x > rightBound) rightBound = rect->upperRight.x;
    }
    upperLeft.update(leftBound, topBound);
    upperRight.update(rightBound, topBound);
    lowerLeft.update(leftBound, bottomBound);
    lowerRight.update(rightBound, bottomBound);
    perimeter = calcPerimeter(); // calculate new perimeter
  }
  return;
}

void minimum_bounding_rectangle::adjustSubTreeBoundaries() {
  // refresh current mbr's boundaries, if it's not the root continue up until you find it
  refreshBoundaries();
  if(!isRoot) m_parent->adjustSubTreeBoundaries();
  return;
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

bool minimum_bounding_rectangle::intersects(line_segment& line) {
  bool pIsContained{ line.p.x >= upperLeft.x && line.p.x <= upperRight.x && line.p.y >= lowerLeft.y && line.p.y <= upperLeft.y }; // true iff p is within the mbr
  bool qIsContained{ line.q.x >= upperLeft.x && line.q.x <= upperRight.x && line.q.y >= lowerLeft.y && line.q.y <= upperLeft.y }; // true iff q is within the mbr
  if (pIsContained || qIsContained) return true;
  else return false;
}

bool minimum_bounding_rectangle::intersects(minimum_bounding_rectangle& rectangle) {
  // rectangle upper left point
  double rulx{ rectangle.upperLeft.x };
  double ruly{ rectangle.upperLeft.y };
  // rectangle upper right point
  double rurx{ rectangle.upperRight.x };
  double rury{ rectangle.upperRight.y };
  // rectangle lower left point
  double rllx{ rectangle.lowerLeft.x };
  double rlly{ rectangle.lowerLeft.y };
  // rectangle lower right point
  double rlrx{ rectangle.lowerRight.x };
  double rlry{ rectangle.lowerRight.y };
  // true iff upper left is within the mbr
  bool upperLeftIsContained{ rulx >= upperLeft.x && rulx <= upperRight.x && ruly >= lowerLeft.y && ruly <= upperLeft.y };
  // true iff upper right is within the mbr
  bool upperRightIsContained{ rurx >= upperLeft.x && rurx <= upperRight.x && rury >= lowerLeft.y && rury <= upperLeft.y };
  // true iff lower left is within the mbr
  bool lowerLeftIsContained{ rllx >= upperLeft.x && rllx <= upperRight.x && rlly >= lowerLeft.y && rlly <= upperLeft.y };
  // true iff lower right is within the mbr
  bool lowerRightIsContained{ rlrx >= upperLeft.x && rlrx <= upperRight.x && rlry >= lowerLeft.y && rlry <= upperLeft.y };
  if (upperLeftIsContained || upperRightIsContained || lowerLeftIsContained || lowerRightIsContained) return true;
  else return false;
}

minimum_bounding_rectangle& minimum_bounding_rectangle::split() {
  if (isLeaf) {
    std::vector<line_segment*>::iterator iStart{ containedSegments.begin() + containedSegments.size() / 2 };
    std::vector<line_segment*>::iterator iEnd{ containedSegments.end() };
    minimum_bounding_rectangle umbr{ **iStart++ }; // u' (needs to be initiallized with an mbr)
    for (auto iter = iStart--; iter != iEnd; iter++) umbr.add(**iter); // split u into u and u' (copy the rest mbrs into u')
    containedSegments.erase(iStart, iEnd); // erase mbrs copied from u into u'
    // no need to adjust all boundaries up to the root since every add function call above does that regardless, we only need to adjust the current mbr's boundaries
    refreshBoundaries();
    return umbr;
  } else {
    std::vector<minimum_bounding_rectangle*>::iterator iStart{ containedRectangles.begin() + containedRectangles.size() / 2 };
    std::vector<minimum_bounding_rectangle*>::iterator iEnd{ containedRectangles.end() };
    minimum_bounding_rectangle umbr{ **iStart++ }; // u' (needs to be initiallized with an mbr
    for (auto iter = iStart--; iter != iEnd; iter++) umbr.add(**iter); // split u into u and u' (copy the rest mbrs into u')
    containedRectangles.erase(iStart, iEnd); // erase mbrs copied from u into u'
    // no need to adjust all boundaries up to the root since every add function call above does that regardless, we only need to adjust the current mbr's boundaries
    refreshBoundaries();
    return umbr;
  }
}

double minimum_bounding_rectangle::hypotheticalNewPerimeter(line_segment& line) {
  double lineTop{ line.upperEndPoint.y };
  double lineBottom{ line.lowerEndPoint.y };
  double lineLeft{ line.leftmostEndPoint.x };
  double lineRight{ line.rightmostEndPoint.x };
  point newUpperLeft{ upperLeft };
  point newUpperRight{ upperRight };
  point newLowerLeft{ lowerLeft };
  point newLowerRight{ lowerRight };
  bool boundariesAreTheSame{ true };
  if (lineTop > upperLeft.y) {
    newUpperLeft.y = lineTop;
    newUpperRight.y = lineTop;
    boundariesAreTheSame = false;
  }
  if (lineBottom < lowerLeft.y) {
    newLowerLeft.y = lineBottom;
    newLowerRight.y = lineBottom;
    boundariesAreTheSame = false;
  }
  if (lineLeft < upperLeft.x) {
    newUpperLeft.x = lineLeft;
    newLowerLeft.x = lineLeft;
    boundariesAreTheSame = false;
  }
  if (lineRight > upperRight.x) {
    newUpperRight.x = lineRight;
    newLowerRight.x = lineRight;
    boundariesAreTheSame = false;
  }
  if (boundariesAreTheSame) return perimeter;
  else return newUpperLeft.distance(newUpperRight) + newUpperRight.distance(newLowerRight) + newLowerRight.distance(newLowerLeft) + newLowerLeft.distance(newUpperLeft);
}

 // unused function
int minimum_bounding_rectangle::positionRelativeTo(line_segment& line) {
  int verticalPosition{ 0 }; // same horizontal space as mbr (possible intersection)
  if (line.lowerEndPoint.y > upperLeft.y) verticalPosition = 1; // above the mbr
  else if (line.upperEndPoint.y < lowerLeft.y) verticalPosition = -1; // below the mbr
  int horizontalPosition{ 0 }; // same vertical space as mbr (possible intersection)
  if (line.leftmostEndPoint.x > upperRight.x) horizontalPosition = 1; // right of mbr
  else if (line.rightmostEndPoint.x < upperLeft.x) horizontalPosition = -1; // left of mbr
  if (verticalPosition == 1 && horizontalPosition == -1) return 1; // top left
  else if (verticalPosition == 1 && horizontalPosition == 0) return 2; // top middle
  else if (verticalPosition == 1 && horizontalPosition == 1) return 3; // top right
  else if (verticalPosition == 0 && horizontalPosition == -1) return 4; // mid left
  else if (verticalPosition == 0 && horizontalPosition == 0) return 5; // center (use center & middle points to calculate distance)
  else if (verticalPosition == 0 && horizontalPosition == 1) return 6; // mid right
  else if (verticalPosition == -1 && horizontalPosition == -1) return 7; // bottom left
  else if (verticalPosition == -1 && horizontalPosition == 0) return 8; // bottom middle
  else if (verticalPosition == -1 && horizontalPosition == 1) return 9; // bottom right
}

 // unused function
int minimum_bounding_rectangle::positionRelativeTo(minimum_bounding_rectangle& rectangle) {
  int verticalPosition{ 0 }; // same horizontal space as mbr (possible intersection)
  if (rectangle.lowerLeft.y > upperLeft.y) verticalPosition = 1; // above the mbr
  else if (rectangle.upperLeft.y < lowerLeft.y) verticalPosition = -1; // below the mbr
  int horizontalPosition{ 0 }; // same vertical space as mbr (possible intersection)
  if (rectangle.upperLeft.x > upperRight.x) horizontalPosition = 1; // right of mbr
  else if (rectangle.upperRight.x < upperLeft.x) horizontalPosition = -1; // left of mbr
  if (verticalPosition == 1 && horizontalPosition == -1) return 1; // top left
  else if (verticalPosition == 1 && horizontalPosition == 0) return 2; // top middle
  else if (verticalPosition == 1 && horizontalPosition == 1) return 3; // top right
  else if (verticalPosition == 0 && horizontalPosition == -1) return 4; // mid left
  else if (verticalPosition == 0 && horizontalPosition == 0) return 5; // center (use center points to calculate distance)
  else if (verticalPosition == 0 && horizontalPosition == 1) return 6; // mid right
  else if (verticalPosition == -1 && horizontalPosition == -1) return 7; // bottom left
  else if (verticalPosition == -1 && horizontalPosition == 0) return 8; // bottom middle
  else if (verticalPosition == -1 && horizontalPosition == 1) return 9; // bottom right
}

unsigned int minimum_bounding_rectangle::size() {
  if (isLeaf) return containedSegments.size();
  else return containedRectangles.size();
}

bool minimum_bounding_rectangle::empty() {
  if (isLeaf) return containedSegments.empty();
  else return containedRectangles.empty();
}

unsigned int minimum_bounding_rectangle::maxSize() {
  return m_containerSize; // max container size for any type of node
}

 // unused function
unsigned int minimum_bounding_rectangle::minSize() {
  if (isRoot) { // if mbr is root
    if (isLeaf) return 1; // as a leaf it can have atleast 1 child
    else return 2; // and as an inner node it can have atleast 2 children
  } else return 0.4 * m_containerSize; // otherwise it must have atleast 0.4 times the maximum container size
}

void minimum_bounding_rectangle::clear(bool isLeaf) {
  if (this->isLeaf) containedSegments.clear(); // clear the correct sub-container
  else containedRectangles.clear();
  if (isLeaf) this->isLeaf ? this->isLeaf = false : this->isLeaf = true; // toggle container type if needed
  return;
}

void minimum_bounding_rectangle::makeRoot() {
  isRoot = true;
  m_parent = nullptr;
  return;
}

void minimum_bounding_rectangle::makeRegularNode(minimum_bounding_rectangle* parent) {
  isRoot = false;
  m_parent = parent;
  return;
}

minimum_bounding_rectangle& minimum_bounding_rectangle::parent() {
  return *m_parent;
}

void minimum_bounding_rectangle::resetID() {
  uniqueID = 0;
  return;
}

double minimum_bounding_rectangle::calcPerimeter() {
  return upperLeft.distance(upperRight) + upperRight.distance(lowerRight) + lowerRight.distance(lowerLeft) + lowerLeft.distance(upperLeft);
}

void minimum_bounding_rectangle::print() {
  std::string type1{ "inner " };
  std::string type2{ "inner " };
  if (isLeaf) type1 = "leaf ";
  if (isRoot) type2 = "root ";
  std::cout << type1 << type2 << "node[" << nodeID << "]:\n";
  upperLeft.print();
  upperRight.print();
  lowerLeft.print();
  lowerRight.print();
  return;
}
