#include "Cube.hpp"

#include <iostream>

using namespace cube;

int main()
{
   Cube cube;
   cube.SetState(eCubeFace::Top, 0, 0, eCubeColor::Green);
   cube.PrintFace(eCubeFace::Top, std::cout);

   cube.ExecuteMove(eCubeMove::UpPrime);
   std::cout << "\n";
   cube.PrintFace(eCubeFace::Top, std::cout);

   cube.ExecuteMove(eCubeMove::UpPrime);
   std::cout << "\n";
   cube.PrintFace(eCubeFace::Top, std::cout);

   cube.ExecuteMove(eCubeMove::UpPrime);
   std::cout << "\n";
   cube.PrintFace(eCubeFace::Top, std::cout);

   cube.ExecuteMove(eCubeMove::UpPrime);
   std::cout << "\n";
   cube.PrintFace(eCubeFace::Top, std::cout);
   return 0;
}