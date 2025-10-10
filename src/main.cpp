#include "Cube.hpp"

#include <iostream>
#include <vector>

using namespace cube;

int main()
{
   Cube cube;
   std::vector<eCubeMove> moves;
   Cube::ParseMoveNotation("R U2 R' r rW L x U2 D",
      moves);

   cube.ExecuteMoves(moves.data(), moves.size());
   cube.Print(std::cout);

   Cube::SerializeMoveList(std::cout, moves.data(), moves.size(), true);
   return 0;
}