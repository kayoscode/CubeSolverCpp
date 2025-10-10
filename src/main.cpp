#include "Cube.hpp"

#include <iostream>

using namespace cube;

int main()
{
   Cube cube;
   cube.ExecuteMove(eCubeMove::BackWide);
   cube.Print(std::cout);
   return 0;
}