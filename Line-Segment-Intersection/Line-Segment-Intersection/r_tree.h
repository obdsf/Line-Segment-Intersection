#pragma once
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <vector>
#include <utility>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "minimum_bounding_rectangle.h"

// # ### #################################################################################### ### #

class r_tree {
// Constructors & Destructor
public:
	r_tree();
	~r_tree();

// Member Functions
public:
	unsigned int size();

// Member Variables
public:
	std::vector<minimum_bounding_rectangle> tree;
private:
	unsigned int m_size;
	unsigned int m_uniqueNodeID;
};
