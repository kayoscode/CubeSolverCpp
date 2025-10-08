#include "Cube.hpp"

namespace cube
{
static void ExecuteUp(CubeState& cube)
{
}

static void ExecuteUpPrime(CubeState& cube)
{
}

static void ExecuteUp2(CubeState& cube)
{
}

static void ExecuteDown(CubeState& cube)
{
}

static void ExecuteDownPrime(CubeState& cube)
{
}

static void ExecuteDown2(CubeState& cube)
{
}

static void ExecuteRight(CubeState& cube)
{
}

static void ExecuteRightPrime(CubeState& cube)
{
}

static void ExecuteRight2(CubeState& cube)
{
}

static void ExecuteLeft(CubeState& cube)
{
}

static void ExecuteLeftPrime(CubeState& cube)
{
}

static void ExecuteLeft2(CubeState& cube)
{
}

static void ExecuteFront(CubeState& cube)
{
}

static void ExecuteFrontPrime(CubeState& cube)
{
}

static void ExecuteFront2(CubeState& cube)
{
}

static void ExecuteBack(CubeState& cube)
{
}

static void ExecuteBackPrime(CubeState& cube)
{
}

static void ExecuteBack2(CubeState& cube)
{
}

static void ExecuteUpWide(CubeState& cube)
{
}

static void ExecuteUpWidePrime(CubeState& cube)
{
}

static void ExecuteUpWide2(CubeState& cube)
{
}

static void ExecuteDownWide(CubeState& cube)
{
}

static void ExecuteDownWidePrime(CubeState& cube)
{
}

static void ExecuteDownWide2(CubeState& cube)
{
}

static void ExecuteRightWide(CubeState& cube)
{
}

static void ExecuteRightWidePrime(CubeState& cube)
{
}

static void ExecuteRightWide2(CubeState& cube)
{
}

static void ExecuteLeftWide(CubeState& cube)
{
}

static void ExecuteLeftWidePrime(CubeState& cube)
{
}

static void ExecuteLeftWide2(CubeState& cube)
{
}

static void ExecuteFrontWide(CubeState& cube)
{
}

static void ExecuteFrontWidePrime(CubeState& cube)
{
}

static void ExecuteFrontWide2(CubeState& cube)
{
}

static void ExecuteBackWide(CubeState& cube)
{
}

static void ExecuteBackWidePrime(CubeState& cube)
{
}

static void ExecuteBackWide2(CubeState& cube)
{
}

static void ExecuteX(CubeState& cube)
{
}

static void ExecuteY(CubeState& cube)
{
}

static void ExecuteZ(CubeState& cube)
{
}

Cube::Cube()
{
   static_assert(
      EnumToInt(eCubeColor::Yellow) == EnumToInt(eCubeFace::Top), "Incorrect cube config.");
   static_assert(
      EnumToInt(eCubeColor::White) == EnumToInt(eCubeFace::Bottom), "Incorrect cube config.");
   static_assert(
      EnumToInt(eCubeColor::Red) == EnumToInt(eCubeFace::Left), "Incorrect cube config.");
   static_assert(
      EnumToInt(eCubeColor::Orange) == EnumToInt(eCubeFace::Right), "Incorrect cube config.");
   static_assert(
      EnumToInt(eCubeColor::Green) == EnumToInt(eCubeFace::Front), "Incorrect cube config.");
   static_assert(
      EnumToInt(eCubeColor::Blue) == EnumToInt(eCubeFace::Back), "Incorrect cube config.");

   SetDefaultState();
}

void Cube::SetDefaultState()
{
   for (int i = 0; i < EnumToInt(eCubeColor::NumColors); i++)
   {
      eCubeColor color = static_cast<eCubeColor>(i);
      int face = EnumToInt(ColorToFace(color));

      for (int i = 0; i < mCube[face].size(); i++)
      {
         mCube[face][i] = color;
      }
   }
}

void Cube::ExecuteMove(eCubeMove move)
{
}
}   // namespace cube