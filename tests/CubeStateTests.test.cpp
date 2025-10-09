#include "Cube.hpp"

#include <array>
#include <gtest/gtest.h>

using namespace cube;

TEST(DefaultsTest, StateTests)
{
   Cube cube;
   cube.SetState(eCubeFace::Front, 0, 0, eCubeColor::Red);
   cube.SetDefaultState();

   for (int i = 0; i < EnumToInt(eCubeColor::NumColors); i++)
   {
      eCubeColor faceColor = static_cast<eCubeColor>(i);
      eCubeFace face = cube.FaceOfColor(faceColor);
      ASSERT_EQ(faceColor, cube.DefaultColorOfFace(face));

      eCubeColor color = cube.GetState(face, 1, 1);
      ASSERT_EQ(color, faceColor);
      ASSERT_EQ(color, cube.ColorOfFace(face));
      ASSERT_EQ(color, cube.DefaultColorOfFace(face));

      for (int i = 0; i < CubeSize; i++)
      {
         for (int j = 0; j < CubeSize; j++)
         {
            ASSERT_EQ(cube.GetState(face, i, j), color);
         }
      }
   }
}

static void SetCubeFace(Cube& cube, eCubeFace face, std::array<eCubeColor, CubeSize * CubeSize>& cubeFace)
{
   for (int j = 0; j < CubeSize; j++)
   {
      for (int i = 0; i < CubeSize; i++)
      {
         cube.SetState(face, i, j, cubeFace[CubeDimsToIdx(i, j)]);
      }
   }
}

static void CompareCubeFace(Cube& cube, eCubeFace face, std::array<eCubeColor, CubeSize * CubeSize>& cubeFace)
{
   for (int j = 0; j < CubeSize; j++)
   {
      for (int i = 0; i < CubeSize; i++)
      {
         eCubeColor color = cube.GetState(face, i, j);
         ASSERT_EQ(color, cubeFace[CubeDimsToIdx(i, j)]);
      }
   }
}

TEST(FaceRotationTests, StateTests)
{
   Cube cube;

   std::array<eCubeColor, CubeSize * CubeSize> cw0 = { eCubeColor::Yellow, eCubeColor::Red, eCubeColor::Green,
                                                       eCubeColor::Green, eCubeColor::Orange, eCubeColor::Blue,
                                                       eCubeColor::Orange, eCubeColor::Blue, eCubeColor::White};

   std::array<eCubeColor, CubeSize * CubeSize> cw1 = { eCubeColor::Orange, eCubeColor::Green, eCubeColor::Yellow,
                                                       eCubeColor::Blue, eCubeColor::Orange, eCubeColor::Red,
                                                       eCubeColor::White, eCubeColor::Blue, eCubeColor::Green};

   std::array<eCubeColor, CubeSize * CubeSize> cw2 = { eCubeColor::White, eCubeColor::Blue, eCubeColor::Orange,
                                                       eCubeColor::Blue, eCubeColor::Orange, eCubeColor::Green,
                                                       eCubeColor::Green, eCubeColor::Red, eCubeColor::Yellow};

   std::array<eCubeColor, CubeSize * CubeSize> cw3 = { eCubeColor::Green, eCubeColor::Blue, eCubeColor::White,
                                                       eCubeColor::Red, eCubeColor::Orange, eCubeColor::Blue,
                                                       eCubeColor::Yellow, eCubeColor::Green, eCubeColor::Orange};

   // Clockwise tests
   SetCubeFace(cube, eCubeFace::Top, cw0);
   cube.ExecuteMove(eCubeMove::Up);
   CompareCubeFace(cube, eCubeFace::Top, cw1);
   cube.ExecuteMove(eCubeMove::Up);
   CompareCubeFace(cube, eCubeFace::Top, cw2);
   cube.ExecuteMove(eCubeMove::Up);
   CompareCubeFace(cube, eCubeFace::Top, cw3);
   cube.ExecuteMove(eCubeMove::Up);
   CompareCubeFace(cube, eCubeFace::Top, cw0);

   // Counter clockwise tests
   SetCubeFace(cube, eCubeFace::Top, cw0);
   cube.ExecuteMove(eCubeMove::UpPrime);
   CompareCubeFace(cube, eCubeFace::Top, cw3);
   cube.ExecuteMove(eCubeMove::UpPrime);
   CompareCubeFace(cube, eCubeFace::Top, cw2);
   cube.ExecuteMove(eCubeMove::UpPrime);
   CompareCubeFace(cube, eCubeFace::Top, cw1);
   cube.ExecuteMove(eCubeMove::UpPrime);
   CompareCubeFace(cube, eCubeFace::Top, cw0);

   // Rotate twice tests
   SetCubeFace(cube, eCubeFace::Top, cw0);
   cube.ExecuteMove(eCubeMove::Up2);
   CompareCubeFace(cube, eCubeFace::Top, cw2);
   cube.ExecuteMove(eCubeMove::Up2);
   CompareCubeFace(cube, eCubeFace::Top, cw0);
}

int main(int argc, char** argv)
{
   testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}