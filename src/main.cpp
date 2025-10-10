#include "Cube.hpp"

#include <iostream>
#include <vector>

using namespace cube;

int main()
{
   Cube cube;
   std::vector<eCubeMove> moves;
   Cube::ParseMoveNotation("",
      moves);

   cube.ExecuteMoves(moves.data(), moves.size());
   cube.Print(std::cout);
   return 0;
}