#include "Cube.hpp"

#include <array>
#include <ostream>

namespace cube
{
enum Coord
{
   TopLeftCorner,
   TopEdge,
   TopRightCorner,
   LeftEdge,
   Middle,
   RightEdge,
   BottomLeftCorner,
   BottomEdge,
   BottomRightCorner,
   NumCoords
};

static void RotateFaceClockwise(CubeFaceData& faceData, eCubeFace face)
{
   SingleCubeFace& faceToRotate = faceData[EnumToInt(face)];

   eCubeColor topLeftCorner = faceToRotate[TopLeftCorner];
   eCubeColor topEdge = faceToRotate[TopEdge];
   eCubeColor topRightCorner = faceToRotate[TopRightCorner];
   eCubeColor leftEdge = faceToRotate[LeftEdge];
   eCubeColor rightEdge = faceToRotate[RightEdge];
   eCubeColor bottomLeftCorner = faceToRotate[BottomLeftCorner];
   eCubeColor bottomEdge = faceToRotate[BottomEdge];
   eCubeColor bottomRightCorner = faceToRotate[BottomRightCorner];

   faceToRotate[TopLeftCorner] = bottomLeftCorner;
   faceToRotate[TopEdge] = leftEdge;
   faceToRotate[TopRightCorner] = topLeftCorner;
   faceToRotate[LeftEdge] = bottomEdge;
   faceToRotate[RightEdge] = topEdge;
   faceToRotate[BottomLeftCorner] = bottomRightCorner;
   faceToRotate[BottomEdge] = rightEdge;
   faceToRotate[BottomRightCorner] = topRightCorner;
}

static void RotateFaceCounterClockwise(CubeFaceData& faceData, eCubeFace face)
{

   SingleCubeFace& faceToRotate = faceData[EnumToInt(face)];

   eCubeColor topLeftCorner = faceToRotate[TopLeftCorner];
   eCubeColor topEdge = faceToRotate[TopEdge];
   eCubeColor topRightCorner = faceToRotate[TopRightCorner];
   eCubeColor leftEdge = faceToRotate[LeftEdge];
   eCubeColor rightEdge = faceToRotate[RightEdge];
   eCubeColor bottomLeftCorner = faceToRotate[BottomLeftCorner];
   eCubeColor bottomEdge = faceToRotate[BottomEdge];
   eCubeColor bottomRightCorner = faceToRotate[BottomRightCorner];

   faceToRotate[TopLeftCorner] = topRightCorner;
   faceToRotate[TopEdge] = rightEdge;
   faceToRotate[TopRightCorner] = bottomRightCorner;
   faceToRotate[LeftEdge] = topEdge;
   faceToRotate[RightEdge] = bottomEdge;
   faceToRotate[BottomLeftCorner] = topLeftCorner;
   faceToRotate[BottomEdge] = leftEdge;
   faceToRotate[BottomRightCorner] = bottomLeftCorner;
}

static void RotateFaceTwice(CubeFaceData& faceData, eCubeFace face)
{
   SingleCubeFace& faceToRotate = faceData[EnumToInt(face)];

   eCubeColor topLeftCorner = faceToRotate[TopLeftCorner];
   eCubeColor topEdge = faceToRotate[TopEdge];
   eCubeColor topRightCorner = faceToRotate[TopRightCorner];
   eCubeColor leftEdge = faceToRotate[LeftEdge];
   eCubeColor rightEdge = faceToRotate[RightEdge];
   eCubeColor bottomLeftCorner = faceToRotate[BottomLeftCorner];
   eCubeColor bottomEdge = faceToRotate[BottomEdge];
   eCubeColor bottomRightCorner = faceToRotate[BottomRightCorner];

   faceToRotate[TopLeftCorner] = bottomRightCorner;
   faceToRotate[TopEdge] = bottomEdge;
   faceToRotate[TopRightCorner] = bottomLeftCorner;
   faceToRotate[LeftEdge] = rightEdge;
   faceToRotate[RightEdge] = leftEdge;
   faceToRotate[BottomLeftCorner] = topRightCorner;
   faceToRotate[BottomEdge] = topEdge;
   faceToRotate[BottomRightCorner] = topLeftCorner;
}

static void ExecuteUp(CubeFaceData& cube)
{
   RotateFaceClockwise(cube, eCubeFace::Top);
}

static void ExecuteUpPrime(CubeFaceData& cube)
{
   RotateFaceCounterClockwise(cube, eCubeFace::Top);
}

static void ExecuteUp2(CubeFaceData& cube)
{
   RotateFaceTwice(cube, eCubeFace::Top);
}

static void ExecuteDown(CubeFaceData& cube)
{
}

static void ExecuteDownPrime(CubeFaceData& cube)
{
}

static void ExecuteDown2(CubeFaceData& cube)
{
}

static void ExecuteRight(CubeFaceData& cube)
{
}

static void ExecuteRightPrime(CubeFaceData& cube)
{
}

static void ExecuteRight2(CubeFaceData& cube)
{
}

static void ExecuteLeft(CubeFaceData& cube)
{
}

static void ExecuteLeftPrime(CubeFaceData& cube)
{
}

static void ExecuteLeft2(CubeFaceData& cube)
{
}

static void ExecuteFront(CubeFaceData& cube)
{
}

static void ExecuteFrontPrime(CubeFaceData& cube)
{
}

static void ExecuteFront2(CubeFaceData& cube)
{
}

static void ExecuteBack(CubeFaceData& cube)
{
}

static void ExecuteBackPrime(CubeFaceData& cube)
{
}

static void ExecuteBack2(CubeFaceData& cube)
{
}

static void ExecuteUpWide(CubeFaceData& cube)
{
}

static void ExecuteUpWidePrime(CubeFaceData& cube)
{
}

static void ExecuteUpWide2(CubeFaceData& cube)
{
}

static void ExecuteDownWide(CubeFaceData& cube)
{
}

static void ExecuteDownWidePrime(CubeFaceData& cube)
{
}

static void ExecuteDownWide2(CubeFaceData& cube)
{
}

static void ExecuteRightWide(CubeFaceData& cube)
{
}

static void ExecuteRightWidePrime(CubeFaceData& cube)
{
}

static void ExecuteRightWide2(CubeFaceData& cube)
{
}

static void ExecuteLeftWide(CubeFaceData& cube)
{
}

static void ExecuteLeftWidePrime(CubeFaceData& cube)
{
}

static void ExecuteLeftWide2(CubeFaceData& cube)
{
}

static void ExecuteFrontWide(CubeFaceData& cube)
{
}

static void ExecuteFrontWidePrime(CubeFaceData& cube)
{
}

static void ExecuteFrontWide2(CubeFaceData& cube)
{
}

static void ExecuteBackWide(CubeFaceData& cube)
{
}

static void ExecuteBackWidePrime(CubeFaceData& cube)
{
}

static void ExecuteBackWide2(CubeFaceData& cube)
{
}

static void ExecuteX(CubeFaceData& cube)
{
}

static void ExecuteY(CubeFaceData& cube)
{
}

static void ExecuteZ(CubeFaceData& cube)
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
      int face = EnumToInt(DefaultFaceOfColor(color));

      for (int i = 0; i < mCube[face].size(); i++)
      {
         mCube[face][i] = color;
      }
   }
}

void Cube::ExecuteMove(eCubeMove move)
{
   switch (move)
   {
   case eCubeMove::Up:
      ExecuteUp(mCube);
      break;
   case eCubeMove::UpPrime:
      ExecuteUpPrime(mCube);
      break;
   case eCubeMove::Up2:
      ExecuteUp2(mCube);
      break;
   case eCubeMove::Down:
      ExecuteDown(mCube);
      break;
   case eCubeMove::DownPrime:
      ExecuteDownPrime(mCube);
      break;
   case eCubeMove::Down2:
      ExecuteDown2(mCube);
      break;
   case eCubeMove::Right:
      ExecuteRight(mCube);
      break;
   case eCubeMove::RightPrime:
      ExecuteRightPrime(mCube);
      break;
   case eCubeMove::Right2:
      ExecuteRight2(mCube);
      break;
   case eCubeMove::Left:
      ExecuteLeft(mCube);
      break;
   case eCubeMove::LeftPrime:
      ExecuteLeftPrime(mCube);
      break;
   case eCubeMove::Left2:
      ExecuteLeft2(mCube);
      break;
   case eCubeMove::Front:
      ExecuteFront(mCube);
      break;
   case eCubeMove::FrontPrime:
      ExecuteFrontPrime(mCube);
      break;
   case eCubeMove::Front2:
      ExecuteFront2(mCube);
      break;
   case eCubeMove::Back:
      ExecuteBack(mCube);
      break;
   case eCubeMove::BackPrime:
      ExecuteBackPrime(mCube);
      break;
   case eCubeMove::Back2:
      ExecuteBack2(mCube);
      break;
   case eCubeMove::UpWide:
      ExecuteUpWide(mCube);
      break;
   case eCubeMove::UpWidePrime:
      ExecuteUpWidePrime(mCube);
      break;
   case eCubeMove::UpWide2:
      ExecuteUpWide2(mCube);
      break;
   case eCubeMove::DownWide:
      ExecuteDownWide(mCube);
      break;
   case eCubeMove::DownWidePrime:
      ExecuteDownWidePrime(mCube);
      break;
   case eCubeMove::DownWide2:
      ExecuteDownWide2(mCube);
      break;
   case eCubeMove::RightWide:
      ExecuteRightWide(mCube);
      break;
   case eCubeMove::RightWidePrime:
      ExecuteRightWidePrime(mCube);
      break;
   case eCubeMove::RightWide2:
      ExecuteRightWide2(mCube);
      break;
   case eCubeMove::LeftWide:
      ExecuteLeftWide(mCube);
      break;
   case eCubeMove::LeftWidePrime:
      ExecuteLeftWidePrime(mCube);
      break;
   case eCubeMove::LeftWide2:
      ExecuteLeftWide2(mCube);
      break;
   case eCubeMove::FrontWide:
      ExecuteFrontWide(mCube);
      break;
   case eCubeMove::FrontWidePrime:
      ExecuteFrontWidePrime(mCube);
      break;
   case eCubeMove::FrontWide2:
      ExecuteFrontWide2(mCube);
      break;
   case eCubeMove::BackWide:
      ExecuteBackWide(mCube);
      break;
   case eCubeMove::BackWidePrime:
      ExecuteBackWidePrime(mCube);
      break;
   case eCubeMove::BackWide2:
      ExecuteBackWide2(mCube);
      break;
   case eCubeMove::X:
      ExecuteX(mCube);
      break;
   case eCubeMove::Y:
      ExecuteY(mCube);
      break;
   case eCubeMove::Z:
      ExecuteZ(mCube);
      break;
   }
}

void Cube::PrintFace(eCubeFace faceToken, std::ostream& outputStream)
{
   static std::array<char, EnumToInt(eCubeColor::NumColors)> colorsCharMap{
      'Y',
      'W',
      'R',
      'O',
      'G',
      'B',
   };

   SingleCubeFace& face = mCube[EnumToInt(faceToken)];
   for (int j = 0; j < CubeSize; j++)
   {
      for (int i = 0; i < CubeSize; i++)
      {
         eCubeColor color = face[CubeDimsToIdx(i, j)];
         outputStream << colorsCharMap[EnumToInt(color)];
      }

      outputStream << "\n";
   }
}

void Cube::Print(std::ostream& outputStream)
{
   outputStream << "Front Face:\n";
   PrintFace(eCubeFace::Front, outputStream);
   outputStream << "\nTop Face:\n";
   PrintFace(eCubeFace::Top, outputStream);
   outputStream << "\nLeft Face:\n";
   PrintFace(eCubeFace::Left, outputStream);
   outputStream << "\nRight Face:\n";
   PrintFace(eCubeFace::Right, outputStream);
   outputStream << "\nBottom Face:\n";
   PrintFace(eCubeFace::Bottom, outputStream);
   outputStream << "\nBack Face:\n";
   PrintFace(eCubeFace::Back, outputStream);
}
}   // namespace cube