
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <vector>
#include <iterator>
#include <memory>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "r_tree.h"

// # ### #################################################################################### ### #

// Constructors & Destructor
r_tree::r_tree()
  : tree{}, m_root{ nullptr }
{}

r_tree::~r_tree() {}

// Member Functions
unsigned int r_tree::size() {
  return tree.size();
}

void r_tree::insert(minimum_bounding_rectangle& u, line_segment& line) {
  if (tree.empty()) {
    std::cout << "INSERT: TREE IS EMPTY\n";
    //std::shared_ptr<minimum_bounding_rectangle> root{ new minimum_bounding_rectangle{ line } };
    minimum_bounding_rectangle root{ line };
    //tree.push_back(std::move(root)); std::cout << "tree.push_back(root);\n";
    tree.push_back(root);
    //m_root = &*tree.back();
    m_root = &tree.back();
  } else {
    if (u.isLeaf) { // u is leaf node
      std::cout << "INSERT: NODE IS LEAF\n";
      bool needToHandleOverflow{ !u.add(line) };
      u.adjustSubTreeBoundaries(); // after the addition it's time to adjust the boundaries of all parent nodes up to the root
      if (needToHandleOverflow) handle_overflow(u); // add line to u, if it overflows (namely, u has B + 1 points) handle it
    } else { // u is inner node or root
      std::cout << "INSERT: NODE IS INNER OR ROOT\n";
      minimum_bounding_rectangle* newSubTreeRoot{ choose_subtree(u, line) }; // choose the sub tree best fit to host new line
      insert(*newSubTreeRoot, line); // insert line into said sub tree
    }
  }
  return;
}

minimum_bounding_rectangle* r_tree::choose_subtree(minimum_bounding_rectangle& u, line_segment& line) {
  minimum_bounding_rectangle* minU{ u.containedRectangles.front() }; // consider first child to have the smallest perimeter increase
  double minPerimeterIncrease{ minU->hypotheticalNewPerimeter(line) - minU->perimeter };
  for (auto iter = ++u.containedRectangles.begin(); iter != u.containedRectangles.end(); iter++) { // for each child of u (after the first)
    minimum_bounding_rectangle* mbr{ *iter }; // get the pointer to current mbr
    double mbrPerimeterIncrease{ mbr->hypotheticalNewPerimeter(line) - mbr->perimeter }; // calculate it's hypothetical perimeter increase
    if (mbrPerimeterIncrease < minPerimeterIncrease) { // if it's increase is smaller than the current min
      minPerimeterIncrease = mbrPerimeterIncrease; // update min perimeter increase
      minU = mbr; // and mbr that has it
    } else if (mbrPerimeterIncrease == minPerimeterIncrease) { // if perimeter increases match
      if (mbr->perimeter < minU->perimeter) { // choose the mbr with the smallest current perimeter (break ties by favoring the smallest MBR)
        minPerimeterIncrease = mbrPerimeterIncrease; // update min perimeter increase
        minU = mbr; // and mbr that has it
      }
    }
  }
  return minU; // return the mbr with the smallest perimeter increase between all children of u
}

void r_tree::handle_overflow(minimum_bounding_rectangle& u) {
  std::cout << "HANDLE OVERFLOW\n";
  line_segment tempLine{};
  minimum_bounding_rectangle u_split{ tempLine }; // temporary holder of the split
  u.split(u_split); // split u into u and u'
  //std::shared_ptr<minimum_bounding_rectangle> u_split{ &overflowKeeper }; // u'
  //tree.push_back(u_split);
  tree.push_back(u_split);
  if (u.isRoot) { // u is root
    //std::shared_ptr<minimum_bounding_rectangle> newRoot{ new minimum_bounding_rectangle{ u } }; // the new root of the tree
    minimum_bounding_rectangle newRoot{ u };
    std::cout << "TREE.PUSH_BACK(NEWROOT) || SIZE: " << tree.size() << '\n';
    //newRoot->add(*u_split); // add u' to new root
    //u.makeRegularNode(&*newRoot); // make old root u a child of new root
    u.makeRegularNode(&newRoot);
    //u_split->makeRegularNode(&*newRoot); // make the split node of u a child of new root
    u_split.makeRegularNode(&newRoot);
    //tree.push_back(std::move(newRoot)); // add new root to the tree
    tree.push_back(newRoot);
  } else { // u is regular inner node or leaf
    minimum_bounding_rectangle* w{ u.parent() }; // parent of u
    //bool needToHandleOverflow{ !w->add(*u_split) };
    bool needToHandleOverflow{ !w->add(u_split) };
    w->adjustSubTreeBoundaries(); // after all the additions it's time to adjust the boundaries of all parent nodes up to the root
    if (needToHandleOverflow) handle_overflow(*w); // add u' as a child of w, if w overflows handle it
  }
  return;
}

minimum_bounding_rectangle* r_tree::root() {
  return m_root;
}

void r_tree::clear() {
  m_root->resetID();
  m_root = nullptr;
  tree.clear();
  return;
}

void r_tree::print() {
  std::cout << "\nR-Tree:\n";
  for (minimum_bounding_rectangle mbr : tree) {
    mbr.print();
  }
  return;
}
