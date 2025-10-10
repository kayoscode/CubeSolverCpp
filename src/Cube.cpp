#include "Cube.hpp"

#include <array>
#include <ostream>

namespace cube
{
[[nodiscard]] constexpr int InvertLayer(int layer)
{
   return CubeSize - layer - 1;
}

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

template <int TLayer>
static void GetRowData(SingleCubeFace& face, eCubeColor& first, eCubeColor& second, eCubeColor& third)
{
   first = face[CubeDimsToIdx(0, TLayer)];
   second = face[CubeDimsToIdx(1, TLayer)];
   third = face[CubeDimsToIdx(2, TLayer)];
}

template <int TLayer, bool TReverse>
static void SetRowData(SingleCubeFace& face, eCubeColor first, eCubeColor second, eCubeColor third)
{
   if constexpr (TReverse)
   {
      face[CubeDimsToIdx(0, TLayer)] = third;
      face[CubeDimsToIdx(1, TLayer)] = second;
      face[CubeDimsToIdx(2, TLayer)] = first; 
   }
   else
   {
      face[CubeDimsToIdx(0, TLayer)] = first;
      face[CubeDimsToIdx(1, TLayer)] = second;
      face[CubeDimsToIdx(2, TLayer)] = third; 
   }
}

template <int TLayer>
struct tLayerRotateYData
{
   // Load the necessary data for our operations.
   tLayerRotateYData(CubeFaceData& faceData)
   : FrontFace(faceData[EnumToInt(eCubeFace::Front)]),
     LeftFace(faceData[EnumToInt(eCubeFace::Left)]),
     BackFace(faceData[EnumToInt(eCubeFace::Back)]),
     RightFace(faceData[EnumToInt(eCubeFace::Right)])
   {
      GetRowData<TLayer>(FrontFace, FrontLeft, FrontMiddle, FrontRight);
      GetRowData<TLayer>(LeftFace, LeftLeft, LeftMiddle, LeftRight);
      GetRowData<TLayer>(BackFace, BackLeft, BackMiddle, BackRight);
      GetRowData<TLayer>(RightFace, RightLeft, RightMiddle, RightRight);
   }

   SingleCubeFace &FrontFace, &LeftFace, &BackFace, &RightFace;

   eCubeColor FrontLeft, FrontMiddle, FrontRight;
   eCubeColor LeftLeft, LeftMiddle, LeftRight;
   eCubeColor BackLeft, BackMiddle, BackRight;
   eCubeColor RightLeft, RightMiddle, RightRight;
};

template <int TLayer>
static void LayerRotateYClockwise(CubeFaceData& faceData)
{
   tLayerRotateYData<TLayer> layerData(faceData);

   SetRowData<TLayer, false>(layerData.FrontFace, layerData.RightLeft, layerData.RightMiddle, layerData.RightRight);
   SetRowData<TLayer, false>(layerData.LeftFace, layerData.FrontLeft, layerData.FrontMiddle, layerData.FrontRight);
   SetRowData<TLayer, false>(layerData.BackFace, layerData.LeftLeft, layerData.LeftMiddle, layerData.LeftRight);
   SetRowData<TLayer, false>(layerData.RightFace, layerData.BackLeft, layerData.BackMiddle, layerData.BackRight);
}

template <int TLayer>
static void LayerRotateYCounterClockwise(CubeFaceData& faceData)
{
   tLayerRotateYData<TLayer> layerData(faceData);

   SetRowData<TLayer, false>(layerData.FrontFace, layerData.LeftLeft, layerData.LeftMiddle, layerData.LeftRight);
   SetRowData<TLayer, false>(layerData.LeftFace, layerData.BackLeft, layerData.BackMiddle, layerData.BackRight);
   SetRowData<TLayer, false>(layerData.BackFace, layerData.RightLeft, layerData.RightMiddle, layerData.RightRight);
   SetRowData<TLayer, false>(layerData.RightFace, layerData.FrontLeft, layerData.FrontMiddle, layerData.FrontRight);
}

template <int TLayer>
static void LayerRotateYTwice(CubeFaceData& faceData)
{
   tLayerRotateYData<TLayer> layerData(faceData);

   SetRowData<TLayer, false>(layerData.FrontFace, layerData.BackLeft, layerData.BackMiddle, layerData.BackRight);
   SetRowData<TLayer, false>(layerData.LeftFace, layerData.RightLeft, layerData.RightMiddle, layerData.RightRight);
   SetRowData<TLayer, false>(layerData.BackFace, layerData.FrontLeft, layerData.FrontMiddle, layerData.FrontRight);
   SetRowData<TLayer, false>(layerData.RightFace, layerData.LeftLeft, layerData.LeftMiddle, layerData.LeftRight);
}

template <int TLayer>
static void GetColData(SingleCubeFace& face, eCubeColor& first, eCubeColor& second, eCubeColor& third)
{
   first = face[CubeDimsToIdx(TLayer, 0)];
   second = face[CubeDimsToIdx(TLayer, 1)];
   third = face[CubeDimsToIdx(TLayer, 2)];
}

template <int TLayer, bool TReverse>
static void SetColData(SingleCubeFace& face, eCubeColor first, eCubeColor second, eCubeColor third)
{
   if constexpr (TReverse)
   {
      face[CubeDimsToIdx(TLayer, 0)] = third;
      face[CubeDimsToIdx(TLayer, 1)] = second;
      face[CubeDimsToIdx(TLayer, 2)] = first; 
   }
   else
   {
      face[CubeDimsToIdx(TLayer, 0)] = first;
      face[CubeDimsToIdx(TLayer, 1)] = second;
      face[CubeDimsToIdx(TLayer, 2)] = third; 
   }
}

template <int TLayer>
struct tLayerRotateXData
{
   // Load the necessary data for our operations.
   tLayerRotateXData(CubeFaceData& faceData)
   : FrontFace(faceData[EnumToInt(eCubeFace::Front)]),
     TopFace(faceData[EnumToInt(eCubeFace::Top)]),
     BackFace(faceData[EnumToInt(eCubeFace::Back)]),
     BottomFace(faceData[EnumToInt(eCubeFace::Bottom)])
   {
      GetColData<TLayer>(FrontFace, FrontTop, FrontMiddle, FrontBottom);
      GetColData<TLayer>(TopFace, TopTop, TopMiddle, TopBottom);
      GetColData<InvertLayer(TLayer)>(BackFace, BackTop, BackMiddle, BackBottom);
      GetColData<TLayer>(BottomFace, BottomTop, BottomMiddle, BottomBottom);
   }

   SingleCubeFace &FrontFace, &TopFace, &BackFace, &BottomFace;

   eCubeColor FrontTop, FrontMiddle, FrontBottom;
   eCubeColor TopTop, TopMiddle, TopBottom;
   eCubeColor BackTop, BackMiddle, BackBottom;
   eCubeColor BottomTop, BottomMiddle, BottomBottom;
};

template <int TLayer>
static void LayerRotateXClockwise(CubeFaceData& faceData)
{
   tLayerRotateXData<TLayer> layerData(faceData);
   SetColData<TLayer, false>(layerData.FrontFace, layerData.BottomTop, layerData.BottomMiddle, layerData.BottomBottom);
   SetColData<TLayer, false>(layerData.TopFace, layerData.FrontTop, layerData.FrontMiddle, layerData.FrontBottom);
   SetColData<InvertLayer(TLayer), true>(layerData.BackFace, layerData.TopTop, layerData.TopMiddle, layerData.TopBottom);
   SetColData<TLayer, true>(layerData.BottomFace, layerData.BackTop, layerData.BackMiddle, layerData.BackBottom);
}

template <int TLayer>
static void LayerRotateXCounterClockwise(CubeFaceData& faceData)
{
   tLayerRotateXData<TLayer> layerData(faceData);
   SetColData<TLayer, false>(layerData.FrontFace, layerData.TopTop, layerData.TopMiddle, layerData.TopBottom);
   SetColData<TLayer, false>(layerData.TopFace, layerData.BackTop, layerData.BackMiddle, layerData.BackBottom);
   SetColData<InvertLayer(TLayer), true>(layerData.BackFace, layerData.BottomTop, layerData.BottomMiddle, layerData.BottomBottom);
   SetColData<TLayer, true>(layerData.BottomFace, layerData.FrontTop, layerData.FrontMiddle, layerData.FrontBottom);
}

template <int TLayer>
static void LayerRotateXTwice(CubeFaceData& faceData)
{
   tLayerRotateXData<TLayer> layerData(faceData);
   SetColData<TLayer, true>(layerData.FrontFace, layerData.BackTop, layerData.BackMiddle, layerData.BackBottom);
   SetColData<TLayer, false>(layerData.TopFace, layerData.BottomTop, layerData.BottomMiddle, layerData.BottomBottom);
   SetColData<InvertLayer(TLayer), true>(layerData.BackFace, layerData.FrontTop, layerData.FrontMiddle, layerData.FrontBottom);
   SetColData<TLayer, false>(layerData.BottomFace, layerData.TopTop, layerData.TopMiddle, layerData.TopBottom);
}

template <int TLayer>
struct tLayerRotateZData
{
   // Load the necessary data for our operations.
   tLayerRotateZData(CubeFaceData& faceData)
   : TopFace(faceData[EnumToInt(eCubeFace::Top)]),
     LeftFace(faceData[EnumToInt(eCubeFace::Left)]),
     RightFace(faceData[EnumToInt(eCubeFace::Right)]),
     BottomFace(faceData[EnumToInt(eCubeFace::Bottom)])
   {
      GetRowData<InvertLayer(TLayer)>(TopFace, TopLeft, TopMiddle, TopRight);
      GetColData<InvertLayer(TLayer)>(LeftFace, LeftTop, LeftMiddle, LeftBottom);
      GetColData<TLayer>(RightFace, RightTop, RightMiddle, RightBottom);
      GetRowData<TLayer>(BottomFace, BottomLeft, BottomMiddle, BottomRight);
   }

   SingleCubeFace &TopFace, &LeftFace, &RightFace, &BottomFace;

   eCubeColor TopLeft, TopMiddle, TopRight;
   eCubeColor LeftTop, LeftMiddle, LeftBottom;
   eCubeColor RightTop, RightMiddle, RightBottom;
   eCubeColor BottomLeft, BottomMiddle, BottomRight;
};

template <int TLayer>
static void LayerRotateZClockwise(CubeFaceData& faceData)
{
   tLayerRotateZData<TLayer> layerData(faceData);
   SetRowData<InvertLayer(TLayer), true>(layerData.TopFace, layerData.LeftTop, layerData.LeftMiddle, layerData.LeftBottom);
   SetColData<TLayer, false>(layerData.RightFace, layerData.TopLeft, layerData.TopMiddle, layerData.TopRight);
   SetRowData<TLayer, true>(layerData.BottomFace, layerData.RightTop, layerData.RightMiddle, layerData.RightBottom);
   SetColData<InvertLayer(TLayer), false>(layerData.LeftFace, layerData.BottomLeft, layerData.BottomMiddle, layerData.BottomRight);
}

template <int TLayer>
static void LayerRotateZCounterClockwise(CubeFaceData& faceData)
{
   tLayerRotateZData<TLayer> layerData(faceData);
   SetRowData<InvertLayer(TLayer), false>(layerData.TopFace, layerData.RightTop, layerData.RightMiddle, layerData.RightBottom);
   SetColData<TLayer, true>(layerData.RightFace, layerData.BottomLeft, layerData.BottomMiddle, layerData.BottomRight);
   SetRowData<TLayer, false>(layerData.BottomFace, layerData.LeftTop, layerData.LeftMiddle, layerData.LeftBottom);
   SetColData<InvertLayer(TLayer), true>(layerData.LeftFace, layerData.TopLeft, layerData.TopMiddle, layerData.TopRight);
}

template <int TLayer>
static void LayerRotateZTwice(CubeFaceData& faceData)
{
   tLayerRotateZData<TLayer> layerData(faceData);
   SetRowData<InvertLayer(TLayer), true>(layerData.TopFace, layerData.BottomLeft, layerData.BottomMiddle, layerData.BottomRight);
   SetColData<TLayer, true>(layerData.RightFace, layerData.LeftTop, layerData.LeftMiddle, layerData.LeftBottom);
   SetRowData<TLayer, true>(layerData.BottomFace, layerData.TopLeft, layerData.TopMiddle, layerData.TopRight);
   SetColData<InvertLayer(TLayer), true>(layerData.LeftFace, layerData.RightTop, layerData.RightMiddle, layerData.RightBottom);
}

static void ExecuteUp(CubeFaceData& cube)
{
   RotateFaceClockwise(cube, eCubeFace::Top);
   LayerRotateYClockwise<0>(cube);
}

static void ExecuteUpPrime(CubeFaceData& cube)
{
   RotateFaceCounterClockwise(cube, eCubeFace::Top);
   LayerRotateYCounterClockwise<0>(cube);
}

static void ExecuteUp2(CubeFaceData& cube)
{
   RotateFaceTwice(cube, eCubeFace::Top);
   LayerRotateYTwice<0>(cube);
}

static void ExecuteDown(CubeFaceData& cube)
{
   RotateFaceClockwise(cube, eCubeFace::Bottom);
   LayerRotateYCounterClockwise<2>(cube);
}

static void ExecuteDownPrime(CubeFaceData& cube)
{
   RotateFaceCounterClockwise(cube, eCubeFace::Bottom);
   LayerRotateYClockwise<2>(cube);
}

static void ExecuteDown2(CubeFaceData& cube)
{
   RotateFaceTwice(cube, eCubeFace::Bottom);
   LayerRotateYTwice<2>(cube);
}

static void ExecuteRight(CubeFaceData& cube)
{
   RotateFaceClockwise(cube, eCubeFace::Right);
   LayerRotateXClockwise<2>(cube);
}

static void ExecuteRightPrime(CubeFaceData& cube)
{
   RotateFaceCounterClockwise(cube, eCubeFace::Right);
   LayerRotateXCounterClockwise<2>(cube);
}

static void ExecuteRight2(CubeFaceData& cube)
{
   RotateFaceTwice(cube, eCubeFace::Right);
   LayerRotateXTwice<2>(cube);
}

static void ExecuteLeft(CubeFaceData& cube)
{
   RotateFaceClockwise(cube, eCubeFace::Left);
   LayerRotateXCounterClockwise<0>(cube);
}

static void ExecuteLeftPrime(CubeFaceData& cube)
{
   RotateFaceCounterClockwise(cube, eCubeFace::Left);
   LayerRotateXClockwise<0>(cube);
}

static void ExecuteLeft2(CubeFaceData& cube)
{
   RotateFaceTwice(cube, eCubeFace::Left);
   LayerRotateXTwice<0>(cube);
}

static void ExecuteFront(CubeFaceData& cube)
{
   RotateFaceClockwise(cube, eCubeFace::Front);
   LayerRotateZClockwise<0>(cube);
}

static void ExecuteFrontPrime(CubeFaceData& cube)
{
   RotateFaceCounterClockwise(cube, eCubeFace::Front);
   LayerRotateZCounterClockwise<0>(cube);
}

static void ExecuteFront2(CubeFaceData& cube)
{
   RotateFaceTwice(cube, eCubeFace::Front);
   LayerRotateZTwice<0>(cube);
}

static void ExecuteBack(CubeFaceData& cube)
{
   RotateFaceClockwise(cube, eCubeFace::Back);
   LayerRotateZCounterClockwise<2>(cube);
}

static void ExecuteBackPrime(CubeFaceData& cube)
{
   RotateFaceCounterClockwise(cube, eCubeFace::Back);
   LayerRotateZClockwise<2>(cube);
}

static void ExecuteBack2(CubeFaceData& cube)
{
   RotateFaceTwice(cube, eCubeFace::Back);
   LayerRotateZTwice<2>(cube);
}
static void ExecuteMiddle(CubeFaceData& cube)
{
   LayerRotateXCounterClockwise<1>(cube);
}

static void ExecuteMiddlePrime(CubeFaceData& cube)
{
   LayerRotateXClockwise<1>(cube);
}

static void ExecuteMiddle2(CubeFaceData& cube)
{
   LayerRotateXTwice<1>(cube);
}

static void ExecuteEquator(CubeFaceData& cube)
{
   LayerRotateYCounterClockwise<1>(cube);
}

static void ExecuteEquatorPrime(CubeFaceData& cube)
{
   LayerRotateYClockwise<1>(cube);
}

static void ExecuteEquator2(CubeFaceData& cube)
{
   LayerRotateYTwice<1>(cube);
}

static void ExecuteStanding(CubeFaceData& cube)
{
   LayerRotateZClockwise<1>(cube);
}

static void ExecuteStandingPrime(CubeFaceData& cube)
{
   LayerRotateZCounterClockwise<1>(cube);
}

static void ExecuteStanding2(CubeFaceData& cube)
{
   LayerRotateZTwice<1>(cube);
}

static void ExecuteX(CubeFaceData& cube)
{
   // Rotate the whole cube on the X axis. It's not inefficient to call the moves since each
   // move is independent.
   ExecuteRight(cube);
   ExecuteLeftPrime(cube);
   ExecuteMiddlePrime(cube);
}

static void ExecuteXPrime(CubeFaceData& cube)
{
   // Rotate the whole cube on the X axis. It's not inefficient to call the moves since each
   // move is independent.
   ExecuteRightPrime(cube);
   ExecuteLeft(cube);
   ExecuteMiddle(cube);
}

static void ExecuteX2(CubeFaceData& cube)
{
   // Rotate the whole cube on the X axis. It's not inefficient to call the moves since each
   // move is independent.
   ExecuteRight2(cube);
   ExecuteLeft2(cube);
   ExecuteMiddle2(cube);
}

static void ExecuteUpWide(CubeFaceData& cube)
{
   ExecuteUp(cube);
   ExecuteEquatorPrime(cube);
}

static void ExecuteUpWidePrime(CubeFaceData& cube)
{
   ExecuteUpPrime(cube);
   ExecuteEquator(cube);
}

static void ExecuteUpWide2(CubeFaceData& cube)
{
   ExecuteUp2(cube);
   ExecuteEquator2(cube);
}

static void ExecuteDownWide(CubeFaceData& cube)
{
   ExecuteDown(cube);
   ExecuteEquator(cube);
}

static void ExecuteDownWidePrime(CubeFaceData& cube)
{
   ExecuteDownPrime(cube);
   ExecuteEquatorPrime(cube);
}

static void ExecuteDownWide2(CubeFaceData& cube)
{
   ExecuteDown2(cube);
   ExecuteEquator2(cube);
}

static void ExecuteRightWide(CubeFaceData& cube)
{
   ExecuteRight(cube);
   ExecuteMiddlePrime(cube);
}

static void ExecuteRightWidePrime(CubeFaceData& cube)
{
   ExecuteRightPrime(cube);
   ExecuteMiddle(cube);
}

static void ExecuteRightWide2(CubeFaceData& cube)
{
   ExecuteRight2(cube);
   ExecuteMiddle2(cube);
}

static void ExecuteLeftWide(CubeFaceData& cube)
{
   ExecuteLeft(cube);
   ExecuteMiddle(cube);
}

static void ExecuteLeftWidePrime(CubeFaceData& cube)
{
   ExecuteLeftPrime(cube);
   ExecuteMiddlePrime(cube);
}

static void ExecuteLeftWide2(CubeFaceData& cube)
{
   ExecuteLeft2(cube);
   ExecuteMiddle2(cube);
}

static void ExecuteFrontWide(CubeFaceData& cube)
{
   ExecuteFront(cube);
   ExecuteStanding(cube);
}

static void ExecuteFrontWidePrime(CubeFaceData& cube)
{
   ExecuteFrontPrime(cube);
   ExecuteStandingPrime(cube);
}

static void ExecuteFrontWide2(CubeFaceData& cube)
{
   ExecuteFront2(cube);
   ExecuteStanding2(cube);
}

static void ExecuteBackWide(CubeFaceData& cube)
{
   ExecuteBack(cube);
   ExecuteStandingPrime(cube);
}

static void ExecuteBackWidePrime(CubeFaceData& cube)
{
   ExecuteBackPrime(cube);
   ExecuteStanding(cube);
}

static void ExecuteBackWide2(CubeFaceData& cube)
{
   ExecuteBack2(cube);
   ExecuteStanding2(cube);
}

static void ExecuteY(CubeFaceData& cube)
{
   ExecuteUp(cube);
   ExecuteDownPrime(cube);
   ExecuteEquatorPrime(cube);
}

static void ExecuteYPrime(CubeFaceData& cube)
{
   ExecuteUpPrime(cube);
   ExecuteDown(cube);
   ExecuteEquator(cube);
}

static void ExecuteY2(CubeFaceData& cube)
{
   ExecuteUp2(cube);
   ExecuteDown2(cube);
   ExecuteEquator2(cube);
}

static void ExecuteZ(CubeFaceData& cube)
{
   ExecuteFront(cube);
   ExecuteStanding(cube);
   ExecuteBackPrime(cube);
}

static void ExecuteZPrime(CubeFaceData& cube)
{
   ExecuteFrontPrime(cube);
   ExecuteStandingPrime(cube);
   ExecuteBack(cube);
}

static void ExecuteZ2(CubeFaceData& cube)
{
   ExecuteFront2(cube);
   ExecuteStanding2(cube);
   ExecuteBack2(cube);
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
   case eCubeMove::Middle:
      ExecuteMiddle(mCube);
      break;
   case eCubeMove::MiddlePrime:
      ExecuteMiddlePrime(mCube);
      break;
   case eCubeMove::Middle2:
      ExecuteMiddle2(mCube);
      break;
   case eCubeMove::Equator:
      ExecuteEquator(mCube);
      break;
   case eCubeMove::EquatorPrime:
      ExecuteEquatorPrime(mCube);
      break;
   case eCubeMove::Equator2:
      ExecuteEquator2(mCube);
      break;
   case eCubeMove::Standing:
      ExecuteStanding(mCube);
      break;
   case eCubeMove::StandingPrime:
      ExecuteStandingPrime(mCube);
      break;
   case eCubeMove::Standing2:
      ExecuteStanding2(mCube);
      break;
   case eCubeMove::X:
      ExecuteX(mCube);
      break;
   case eCubeMove::XPrime:
      ExecuteXPrime(mCube);
      break;
   case eCubeMove::X2:
      ExecuteX2(mCube);
      break;
   case eCubeMove::Y:
      ExecuteY(mCube);
      break;
   case eCubeMove::YPrime:
      ExecuteYPrime(mCube);
      break;
   case eCubeMove::Y2:
      ExecuteY2(mCube);
      break;
   case eCubeMove::Z:
      ExecuteZ(mCube);
      break;
   case eCubeMove::ZPrime:
      ExecuteZPrime(mCube);
      break;
   case eCubeMove::Z2:
      ExecuteZ2(mCube);
      break;
   }
}

void PrintRow(SingleCubeFace& face, int row, std::ostream& outputStream, bool useColor)
{
   std::array<std::string, EnumToInt(eCubeColor::NumColors)> colorMap = 
   {
      // Yellow
      "\033[48;2;255;255;51m  \033[0m",
      // White
      "\033\[48;2;230;230;230m  \033[0m",
      // Red
      "\033[48;2;230;15;0m  \033[0m",
      // Orange
      "\033[48;2;255;127;0m  \033[0m",
      // Green
      "\033[48;2;60;200;60m  \033[0m",
      // Blue
      "\033[48;2;51;51;255m  \033[0m",
   };

   std::array<std::string, EnumToInt(eCubeColor::NumColors)> textColorMap = 
   {
      // Yellow
      "Y",
      // White
      "W",
      // Red
      "R",
      // Orange
      "O",
      // Green
      "G",
      // Blue
      "B",
   };

   for (int i = 0; i < CubeSize; i++)
   {
      eCubeColor color = face[CubeDimsToIdx(i, row)];
      if (EnumToInt(color) >= EnumToInt(eCubeColor::NumColors))
      {
         outputStream << "??";
         continue;
      }

      if (useColor)
      {
         outputStream << colorMap[EnumToInt(color)];
      }
      else
      {
         outputStream << textColorMap[EnumToInt(color)] << " ";
      }
   }
}

void Cube::PrintFace(eCubeFace faceToken, std::ostream& outputStream, bool useColor)
{
   SingleCubeFace& face = mCube[EnumToInt(faceToken)];
   for (int j = 0; j < CubeSize; j++)
   {
      PrintRow(face, j, outputStream, useColor);
      outputStream << "\n";
   }
}

void Cube::Print(std::ostream& outputStream, bool useColor)
{
   for (int i = 0; i < CubeSize; i++)
   {
      PrintRow(mCube[EnumToInt(eCubeFace::Front)], i, outputStream, useColor);
      outputStream << "    ";
      PrintRow(mCube[EnumToInt(eCubeFace::Top)], i, outputStream, useColor);
      outputStream << "    ";
      PrintRow(mCube[EnumToInt(eCubeFace::Left)], i, outputStream, useColor);
      outputStream << "    ";
      PrintRow(mCube[EnumToInt(eCubeFace::Right)], i, outputStream, useColor);
      outputStream << "    ";
      PrintRow(mCube[EnumToInt(eCubeFace::Bottom)], i, outputStream, useColor);
      outputStream << "    ";
      PrintRow(mCube[EnumToInt(eCubeFace::Back)], i, outputStream, useColor);
      outputStream << "\n";
   }
   outputStream << "Front     Top       Left      Right     Bottom    Back      \n";
}
}   // namespace cube