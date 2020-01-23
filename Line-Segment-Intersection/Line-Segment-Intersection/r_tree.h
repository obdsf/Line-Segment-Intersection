#pragma once
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <vector>
#include <utility>
#include <memory>
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
	void insert(minimum_bounding_rectangle& u, line_segment& line);
	minimum_bounding_rectangle* choose_subtree(minimum_bounding_rectangle& u, line_segment& line);
	void handle_overflow(minimum_bounding_rectangle& u);
	minimum_bounding_rectangle* root();
	void clear();
	void print();

// Member Variables
public:
	std::vector<minimum_bounding_rectangle> tree;
private:
	minimum_bounding_rectangle* m_root;
};
