#include "Cube.hpp"

#include <array>
#include <gtest/gtest.h>
#include <random>

using namespace cube;

TEST(DefaultsTest, StateTests)
{
   Cube cube;
   cube.SetState(eCubeFace::Front, 0, 0, eCubeColor::Red);
   cube.SetSolved();

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

static void SetCubeFace(
   Cube& cube, eCubeFace face, std::array<eCubeColor, CubeSize * CubeSize>& cubeFace)
{
   for (int j = 0; j < CubeSize; j++)
   {
      for (int i = 0; i < CubeSize; i++)
      {
         cube.SetState(face, i, j, cubeFace[CubeDimsToIdx(i, j)]);
      }
   }
}

static void CompareCubeFace(
   Cube& cube, eCubeFace face, std::array<eCubeColor, CubeSize * CubeSize>& cubeFace)
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

   std::array<eCubeColor, CubeSize * CubeSize> cw0 = {eCubeColor::Yellow, eCubeColor::Red,
      eCubeColor::Green, eCubeColor::Green, eCubeColor::Orange, eCubeColor::Blue,
      eCubeColor::Orange, eCubeColor::Blue, eCubeColor::White};

   std::array<eCubeColor, CubeSize * CubeSize> cw1 = {eCubeColor::Orange, eCubeColor::Green,
      eCubeColor::Yellow, eCubeColor::Blue, eCubeColor::Orange, eCubeColor::Red, eCubeColor::White,
      eCubeColor::Blue, eCubeColor::Green};

   std::array<eCubeColor, CubeSize * CubeSize> cw2 = {eCubeColor::White, eCubeColor::Blue,
      eCubeColor::Orange, eCubeColor::Blue, eCubeColor::Orange, eCubeColor::Green,
      eCubeColor::Green, eCubeColor::Red, eCubeColor::Yellow};

   std::array<eCubeColor, CubeSize * CubeSize> cw3 = {eCubeColor::Green, eCubeColor::Blue,
      eCubeColor::White, eCubeColor::Red, eCubeColor::Orange, eCubeColor::Blue, eCubeColor::Yellow,
      eCubeColor::Green, eCubeColor::Orange};

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

TEST(RotatePerftTest, StateTests)
{
   Cube cube;
   std::vector<eCubeMove> moves;

   // Execute a long series of moves and check to see if the result is correct.
   Cube::ParseMoveNotation(
      "F' B' D U R2 L2 U L' D L2 D2 B D' L R U2 F2 D2 B D' B' F2 U D2 L' F' D F2 B' R U R2 D2 R U2 "
      "D' B' F U' B' D2 B2 L' F2 D R F R' F2 D' F' D U2 L2 U R L D2 U L2 R' F2 L' D F' L F' D2 R L "
      "F2 D' R U' R2 U' F B L' U' D' L D' L B L2 U R U' F2 L2 B' R2 L' U R2 D R' D' U' B2 U' R' L2 "
      "D2 B' L D' F2 U L2 F D' L' U D' F' B' L' R' D U L B2 D' U L B R2 U2 B F2 U D' R2 D U2 B2 R2 "
      "L2 D2 L' B' L' R D2 R D R' U' L R2 B F2 U' B2 D2 B2 D' B' F' R2 F R D' U' F L' R' F U B2 D "
      "L2 B2 R D2 F' U D2 L R2 F2 D F' B2 U L D' B2 U' R2 F' R U R2 U F' U' B2 F' L2 U' D F' R L' "
      "U D2 F' B2 L R' F' L D2 U2 L2 R2 F2 D F' D B2 F U2 D' B L2 F D F' L R2 U B' U' R' L2 D' F' "
      "B' D2 F' B' R B2 U B' R L B U' F' L' B U' D' L B2 U' F2 D' U R B2 L2 F B2 L' U' R2 L' F2 R2 "
      "B2 D2 R2 U2 B' L2 B L2 R U' R B2 R' L2 D' L' B' L F' L2 B' L2 F D L' D2 R U' F' R2 F2 L U2 "
      "D2 L' F' B2 U D B2 U L2 U R' U2 D' R F2 B R' D2 B2 L U' R D2 U2 L R U2 R L' D R U L D2 F2 L "
      "R' B2 R2 D' L2 F' L2 B U R2 D2 U2 F' U' L2 D' F' R2 D L F2 R2 L' B2 U' R' U2 D R' B2 U2 R' "
      "D B U2 D B D2 U2 R F' D U2 B' D2 F' U D2 F B R2 B2 D' B' F' D2 L2 D' L U R2 D' F R' B D2 L "
      "U B2 L B' R L2 D2 L F U B F L' U' F' B D2 R' D2 F2 R2 F' B2 U' D F U' B R' B F' U B2 R2 D "
      "B2 L R' U2 L2 U B F D2 R2 F2 U' L' B2 D B D2 L F2 U' D2 F2 L' R B L2 U2 F R F' R F2 L' F2 B "
      "D' R2 U' D2 F' D U2 L' R B2 L R2 U2 B2 F U R' B2 L' F' U' F' R' B2 L2 U B' D' F' R2 U2 B2 "
      "R' U F2 B' U' R2 B R B D2 B2 R L2 B' F L' F' R L U' R F2 R' B' L B U' D B R2 L' D2 B' D R2 "
      "L' F B2 D' L D' B' U2 L2 D' B U2 D' R' D L' U' B2 R U L2 D' F2 R' F' U2 F B L2 R2 D2 R U' "
      "F2 R' L2 B' D' R' D2 U' R F U' B2 U D2 L2 B' R' F2 U' F B R U' L U R' D L' D' R' D R L B L "
      "U' B2 R' L2 D' U R D' B2 R' L B' U' R B F2 D2 U' B2 U L2 U2 D2 R2 U2 B2 D' R2 B2 D' B' D L' "
      "B2 R L2 U' D F U D F2 R2 D' L' F' L' R' U R",
      moves);

   cube.ExecuteMoves(moves.data(), moves.size());

   std::array<eCubeColor, CubeSize * CubeSize> expectedFront = {eCubeColor::White,
      eCubeColor::White, eCubeColor::White, eCubeColor::Orange, eCubeColor::Green,
      eCubeColor::Green, eCubeColor::Yellow, eCubeColor::Orange, eCubeColor::Blue};

   std::array<eCubeColor, CubeSize * CubeSize> expectedTop = {eCubeColor::White, eCubeColor::Red,
      eCubeColor::Green, eCubeColor::Green, eCubeColor::Yellow, eCubeColor::Blue,
      eCubeColor::Orange, eCubeColor::Orange, eCubeColor::Blue};

   std::array<eCubeColor, CubeSize * CubeSize> expectedLeft = {eCubeColor::Red, eCubeColor::White,
      eCubeColor::Green, eCubeColor::Red, eCubeColor::Red, eCubeColor::Blue, eCubeColor::Blue,
      eCubeColor::Yellow, eCubeColor::Red};

   std::array<eCubeColor, CubeSize * CubeSize> expectedRight = {eCubeColor::Red, eCubeColor::White,
      eCubeColor::Orange, eCubeColor::Yellow, eCubeColor::Orange, eCubeColor::Red,
      eCubeColor::Yellow, eCubeColor::Blue, eCubeColor::Yellow};

   std::array<eCubeColor, CubeSize * CubeSize> expectedBottom = {eCubeColor::Green,
      eCubeColor::Yellow, eCubeColor::Red, eCubeColor::Blue, eCubeColor::White, eCubeColor::Red,
      eCubeColor::White, eCubeColor::Green, eCubeColor::Orange};

   std::array<eCubeColor, CubeSize * CubeSize> expectedBack = {eCubeColor::Yellow,
      eCubeColor::Yellow, eCubeColor::Green, eCubeColor::Green, eCubeColor::Blue, eCubeColor::White,
      eCubeColor::Blue, eCubeColor::Orange, eCubeColor::Orange};

   CompareCubeFace(cube, eCubeFace::Front, expectedFront);
   CompareCubeFace(cube, eCubeFace::Top, expectedTop);
   CompareCubeFace(cube, eCubeFace::Left, expectedLeft);
   CompareCubeFace(cube, eCubeFace::Right, expectedRight);
   CompareCubeFace(cube, eCubeFace::Bottom, expectedBottom);
   CompareCubeFace(cube, eCubeFace::Back, expectedBack);
}

TEST(RandomMovesTest, StateTests)
{
   // Simple test: Generate a completely random sequence of moves then execute them.
   // Run them in reverse and see if we end up at the default state.
   Cube cube;
   std::vector<eCubeMove> moves;
   std::vector<eCubeMove> reverseMoves;
   constexpr int numMoves = 1000;
   constexpr int numTrials = 10;

   std::random_device rd;
   std::mt19937 engine(rd());
   engine.seed(100);

   // Uniform integer distribution between 1 and 6 (like a die roll)
   std::uniform_int_distribution<int> randomMovesGen(0, EnumToInt(eCubeMove::NumMoves) - 1);

   for (int i = 0; i < numTrials; i++)
   {
      engine.seed(i * 100);

      moves.clear();
      reverseMoves.clear();

      for (int j = 0; j < numMoves; j++)
      {
         moves.push_back(static_cast<eCubeMove>(randomMovesGen(engine)));
      }

      cube.ExecuteMoves(moves.data(), moves.size());
      Cube::ReverseMoves(moves, reverseMoves);
      ASSERT_EQ(moves.size(), reverseMoves.size());
      cube.ExecuteMoves(reverseMoves.data(), reverseMoves.size());

      ASSERT_TRUE(cube.IsSolved());

      // Now make sure each face is in the default position.
      for (int i = 0; i < EnumToInt(eCubeFace::NumFaces); i++)
      {
         eCubeFace face = static_cast<eCubeFace>(i);
         ASSERT_EQ(cube.ColorOfFace(face), cube.DefaultColorOfFace(face));
      }
   }
}

int main(int argc, char** argv)
{
   testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}