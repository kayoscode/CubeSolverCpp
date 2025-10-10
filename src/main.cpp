#include "Cube.hpp"
#include "CubeSolver.hpp"

#include <iostream>
#include <vector>

using namespace cube;

int main()
{
   Cube cube;

   std::vector<eCubeMove> moves;
   Cube::ParseMoveNotation(
      "R' u R' u'", moves);
   cube.ExecuteMoves(moves.data(), moves.size());
   cube.Print(std::cout);

   CfopSolver solver(cube, true, true);
   solver.Solve(std::cout);

   return 0;
}