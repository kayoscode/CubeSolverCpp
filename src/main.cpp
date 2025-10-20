#include "Cube.hpp"
#include "CubeSolver.hpp"
#include "Timer.hpp"

#include <iostream>
#include <vector>

using namespace cube;

int main()
{
   std::vector<eCubeMove> moves;

   int seed = time(0);

   Cube::GenerateScramble(moves, 30, seed);
   std::cout << "Seed: " << seed << "\n" << "Scramble: ";
   Cube::SerializeMoveList(std::cout, moves.data(), moves.size());
   std::cout << "\n";

   Cube cube;
   cube.ExecuteMoves(moves.data(), moves.size());
   cube.Print(std::cout);

   Timer t;
   CfopSolver solver(cube, true, true);
   solver.Solve(std::cout);
   std::cout << "Solved in: " << t.Microseconds() << " us\n";

   return 0;
}