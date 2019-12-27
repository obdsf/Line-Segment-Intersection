
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <iostream>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "global.h"
#include "sweep_line.h"

// # ### #################################################################################### ### #

// Constructors & Destructor
sweep_line::sweep_line()
  : sweep{ epbTopLeft, epbTopRight }
  , step{ 2 * g_precision }
  , start{ (int)epbTopLeft.y }
  , end{ (int)epbBotLeft.y }
{}

sweep_line::~sweep_line() {}

// Member Functions
void sweep_line::reset() {
  sweep.update(epbTopLeft, epbTopRight);
}

void sweep_line::advance() {
  sweep.p.y = truncOneDigit(sweep.p.y + step);
  sweep.q.y = truncOneDigit(sweep.q.y + step);
  /* truncOneDigit function truncs every digit after the first decimal digit
   * just like trunc, there are cases of error due to how floating point
   * numbers operate (number 512.1 causes truncOneDigit to produce the same
   * result, when we start counting from 300 with step equal to 0.1, in our
   * case with step equal to 0.2 and the same starting point we are bound to
   * hit the same wall somewhere above 1000, but that never happens since we
   * stop at 900).
   * floating point numbers are impossible to trunc because they're impossible
   * to even represent accurately
   * (https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html)
   */
  return;
}

void sweep_line::advance(const float& position) {
  if (position >= start && position <= end) {
    sweep.p.y = position;
    sweep.q.y = position;
  }
  return;
}

bool sweep_line::reachedEnd() {
  if (sweep.p.y > epbBotLeft.y)return true;
  return false;
}
