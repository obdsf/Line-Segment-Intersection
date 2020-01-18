
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <vector>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "r_tree.h"

// # ### #################################################################################### ### #

// Constructors & Destructor
r_tree::r_tree()
  : tree{}, m_size{}
{}

r_tree::~r_tree() {}

// Member Functions
unsigned int r_tree::size() {
  return m_size;
}