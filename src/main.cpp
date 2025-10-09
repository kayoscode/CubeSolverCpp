#include "Cube.hpp"

#include <iostream>

using namespace cube;

int main()
{
   Cube cube;
   cube.ExecuteMove(eCubeMove::Left);
   cube.Print(std::cout);
   std::cout << "\n";

   cube.ExecuteMove(eCubeMove::Equator);
   cube.Print(std::cout);
   std::cout << "\n";

   cube.ExecuteMove(eCubeMove::LeftPrime);
   cube.Print(std::cout);
   std::cout << "\n";

   cube.ExecuteMove(eCubeMove::EquatorPrime);
   cube.Print(std::cout);
   std::cout << "\n";
   return 0;
}