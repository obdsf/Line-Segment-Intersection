
// ### # #################################################################################### # ###

// STL : Standard Template Library
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "global.h"
#include "sweep_line.h"

// ### # #################################################################################### # ###

// Constructors & Destructor
sweep_line::sweep_line()
  : sweep{ epbTopLeft, epbBotLeft }
{}

sweep_line::~sweep_line() {}

// Member Functions
void sweep_line::reset() {
  //sweep.update()
}