#include "Cube.hpp"

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

TEST(FaceRotationTests, StateTests)
{
   Cube cube;

   for (int faceIdx = 0; faceIdx < EnumToInt(eCubeFace::NumFaces); faceIdx++)
   {
      for (int i = 0; i < CubeSize; i++)
      {
         for (int j = 0; j < CubeSize; j++)
         {
            cube.SetDefaultState();
            // TODO
         }
      }
   }
}

int main(int argc, char** argv)
{
   testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}