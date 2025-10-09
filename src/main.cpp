#include "Cube.hpp"

#include <iostream>

using namespace cube;

int main()
{
   Cube cube;
   cube.Print(std::cout);
   std::cout << "\n";

   cube.ExecuteMove(eCubeMove::Right);
   cube.ExecuteMove(eCubeMove::Equator);
   cube.ExecuteMove(eCubeMove::RightPrime);
   cube.ExecuteMove(eCubeMove::EquatorPrime);
   cube.Print(std::cout);
   std::cout << "\n";
   return 0;
}