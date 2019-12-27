
// # ### #################################################################################### ### #

// STL : Standard Template Library
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "event_point.h"
#include "event_queue.h"
#include "line_segment.h"

// # ### #################################################################################### ### #

// Constructors & Destructor
event_queue::event_queue(std::vector<line_segment>& lineSet)
  : queue{}, lineSet{ &lineSet }
{}

event_queue::~event_queue() {}

// Member Functions
void event_queue::initialize() {
  for (int i = 0; i < lineSet->size(); i++) {
    event_point upperEndPoint{ lineSet->at(i).upperEndPoint, lineSet->at(i) };
    event_point lowerEndPoint{ lineSet->at(i).lowerEndPoint };
    queue.push_back(upperEndPoint);
    queue.push_back(lowerEndPoint);
  }
}
