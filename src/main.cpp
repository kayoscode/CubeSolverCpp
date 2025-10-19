#include "Cube.hpp"
#include "CubeSolver.hpp"

#include <iostream>
#include <vector>

using namespace cube;

int main()
{
   std::vector<eCubeMove> moves;

   int seed = time(0);

   Cube::GenerateScramble(moves, 20, seed);
   std::cout << "Seed: " << seed << "\n" << "Scramble: ";
   Cube::SerializeMoveList(std::cout, moves.data(), moves.size());
   std::cout << "\n";

   Cube cube;
   cube.ExecuteMoves(moves.data(), moves.size());
   cube.Print(std::cout);

   CfopSolver solver(cube, true, true);
   solver.Solve(std::cout);

   return 0;
}