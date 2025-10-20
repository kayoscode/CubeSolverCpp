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

   Cube::GenerateScramble(moves, (int)2e9, seed);
   std::cout << "Seed: " << seed << "\n" << "Scramble: ";
   //Cube::SerializeMoveList(std::cout, moves.data(), moves.size());
   std::cout << "\n";

   Cube cube;
   Timer t;
   cube.ExecuteMoves(moves.data(), moves.size());
   std::cout << "Executing moves took: " << t.Milliseconds() << " ms\n";
   cube.Print(std::cout);

   t.Reset();
   CfopSolver solver(cube, true, true);
   solver.Solve(std::cout);
   std::cout << "Solved in: " << t.Microseconds() << " us\n";

   return 0;
}