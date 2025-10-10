#include "Cube.hpp"

#include <iostream>

using namespace cube;

int main()
{
   Cube cube;
   cube.ExecuteMove(eCubeMove::Middle);
   cube.Print(std::cout);
   std::cout << "\n";
   return 0;
}