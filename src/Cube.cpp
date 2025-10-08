#include "Cube.hpp"

#include <array>
#include <ostream>

namespace cube
{
enum class eFaceRow
{
   Upper,
   Middle,
   Lower
};

enum class eFaceCol
{
   Left,
   Middle,
   Right
};

/**
 * @brief      Returns the data in the row going left to right.
 *
 * @param      face    The face
 * @param[in]  row     The row
 * @param      first   The first
 * @param      second  The second
 * @param      third   The third
 */
template <eFaceRow TRow, bool TReverse>
inline static void GetRowData(
   SingleCubeFace& face, eCubeColor& first, eCubeColor& second, eCubeColor& third)
{
   constexpr int row = EnumToInt(TRow);
   if constexpr (!TReverse)
   {
      first = face[CubeDimsToIdx(0, row)];
      second = face[CubeDimsToIdx(1, row)];
      third = face[CubeDimsToIdx(2, row)];
   }
   else
   {
      first = face[CubeDimsToIdx(2, row)];
      second = face[CubeDimsToIdx(1, row)];
      third = face[CubeDimsToIdx(0, row)];
   }
}

/**
 * @brief      Sets the data in the row from left to right.
 *
 * @param      face    The face
 * @param[in]  row     The row
 * @param      first   The first
 * @param      second  The second
 * @param      third   The third
 */
template <eFaceRow TRow, bool TReverse>
inline static void SetRowData(
   SingleCubeFace& face, eCubeColor first, eCubeColor second, eCubeColor third)
{
   constexpr int row = EnumToInt(TRow);

   if constexpr (!TReverse)
   {
      face[CubeDimsToIdx(0, row)] = first;
      face[CubeDimsToIdx(1, row)] = second;
      face[CubeDimsToIdx(2, row)] = third;
   }
   else
   {
      face[CubeDimsToIdx(2, row)] = first;
      face[CubeDimsToIdx(1, row)] = second;
      face[CubeDimsToIdx(0, row)] = third;
   }
}

template <eFaceRow TSrcRow, eFaceRow TDestRow, bool TReverse>
   requires(TSrcRow != TDestRow)
inline static void CopyRowToRow(SingleCubeFace& face)
{
   constexpr int srcRow = EnumToInt(TSrcRow);
   constexpr int destRow = EnumToInt(TDestRow);

   if constexpr (!TReverse)
   {
      face[CubeDimsToIdx(0, destRow)] = face[CubeDimsToIdx(0, srcRow)];
      face[CubeDimsToIdx(1, destRow)] = face[CubeDimsToIdx(1, srcRow)];
      face[CubeDimsToIdx(2, destRow)] = face[CubeDimsToIdx(2, srcRow)];
   }
   else
   {
      face[CubeDimsToIdx(0, destRow)] = face[CubeDimsToIdx(2, srcRow)];
      face[CubeDimsToIdx(1, destRow)] = face[CubeDimsToIdx(1, srcRow)];
      face[CubeDimsToIdx(2, destRow)] = face[CubeDimsToIdx(0, srcRow)];
   }
}

template <eFaceRow TFirstRow, eFaceRow TSecondRow>
   requires(TFirstRow != TSecondRow)
inline static void SwapRows(SingleCubeFace& face)
{
   constexpr int firstRow = EnumToInt(TFirstRow);

   eCubeColor firstTemp = face[CubeDimsToIdx(0, firstRow)];
   eCubeColor secondTemp = face[CubeDimsToIdx(1, firstRow)];
   eCubeColor thirdTemp = face[CubeDimsToIdx(2, firstRow)];

   // First copy the second row to the first, then set the second to the temp
   CopyRowToRow<TSecondRow, TFirstRow, false>(face);
   SetRowData<TFirstRow, false>(face, firstTemp, secondTemp, thirdTemp);
}

template <eFaceRow TSrcRow, eFaceCol TDestCol, bool TReverse>
inline static void CopyRowToCol(SingleCubeFace& face)
{
   constexpr int srcRow = EnumToInt(TSrcRow);
   constexpr int destCol = EnumToInt(TDestCol);
   if constexpr (!TReverse)
   {
      face[CubeDimsToIdx(destCol, 0)] = face[CubeDimsToIdx(0, srcRow)];
      face[CubeDimsToIdx(destCol, 1)] = face[CubeDimsToIdx(1, srcRow)];
      face[CubeDimsToIdx(destCol, 2)] = face[CubeDimsToIdx(2, srcRow)];
   }
   else
   {
      face[CubeDimsToIdx(destCol, 0)] = face[CubeDimsToIdx(2, srcRow)];
      face[CubeDimsToIdx(destCol, 1)] = face[CubeDimsToIdx(1, srcRow)];
      face[CubeDimsToIdx(destCol, 2)] = face[CubeDimsToIdx(0, srcRow)];
   }
}

/**
 * @brief      Returns the data in the col going from top to bottom.
 *
 * @param      face    The face
 * @param      first   The first
 * @param      second  The second
 * @param      third   The third
 *
 * @tparam     TCol    { description }
 */
template <eFaceCol TCol, bool TReverse>
static void GetColData(
   SingleCubeFace& face, eCubeColor& first, eCubeColor& second, eCubeColor& third)
{
   constexpr int col = EnumToInt(TCol);

   if constexpr (!TReverse)
   {
      first = face[CubeDimsToIdx(col, 0)];
      second = face[CubeDimsToIdx(col, 1)];
      third = face[CubeDimsToIdx(col, 2)];
   }
   else
   {
      first = face[CubeDimsToIdx(col, 2)];
      second = face[CubeDimsToIdx(col, 1)];
      third = face[CubeDimsToIdx(col, 0)];
   }
}

/**
 * @brief      Returns the data in the col going from top to bottom.
 *
 * @param      face    The face
 * @param      first   The first
 * @param      second  The second
 * @param      third   The third
 *
 * @tparam     TCol    { description }
 */
template <eFaceCol TCol, bool TReverse>
static void SetColData(SingleCubeFace& face, eCubeColor first, eCubeColor second, eCubeColor third)
{
   constexpr int col = EnumToInt(TCol);

   if constexpr (!TReverse)
   {
      face[CubeDimsToIdx(col, 0)] = first;
      face[CubeDimsToIdx(col, 1)] = second;
      face[CubeDimsToIdx(col, 2)] = third;
   }
   else
   {
      face[CubeDimsToIdx(col, 2)] = first;
      face[CubeDimsToIdx(col, 1)] = second;
      face[CubeDimsToIdx(col, 0)] = third;
   }
}

template <eFaceCol TSrcCol, eFaceCol TDestCol, bool TReverse>
   requires(TSrcCol != TDestCol)
inline static void CopyColToCol(SingleCubeFace& face)
{
   constexpr int srcCol = EnumToInt(TSrcCol);
   constexpr int destCol = EnumToInt(TDestCol);

   if constexpr (!TReverse)
   {
      face[CubeDimsToIdx(destCol, 0)] = face[CubeDimsToIdx(srcCol, 0)];
      face[CubeDimsToIdx(destCol, 1)] = face[CubeDimsToIdx(srcCol, 1)];
      face[CubeDimsToIdx(destCol, 2)] = face[CubeDimsToIdx(srcCol, 2)];
   }
   else
   {
      face[CubeDimsToIdx(destCol, 0)] = face[CubeDimsToIdx(srcCol, 2)];
      face[CubeDimsToIdx(destCol, 1)] = face[CubeDimsToIdx(srcCol, 1)];
      face[CubeDimsToIdx(destCol, 2)] = face[CubeDimsToIdx(srcCol, 0)];
   }
}

template <eFaceCol TSrcCol, eFaceRow TDestRow, bool TReverse>
inline static void CopyColToRow(SingleCubeFace& face)
{
   constexpr int srcCol = EnumToInt(TSrcCol);
   constexpr int destRow = EnumToInt(TDestRow);

   if constexpr (!TReverse)
   {
      face[CubeDimsToIdx(0, destRow)] = face[CubeDimsToIdx(srcCol, 0)];
      face[CubeDimsToIdx(1, destRow)] = face[CubeDimsToIdx(srcCol, 1)];
      face[CubeDimsToIdx(2, destRow)] = face[CubeDimsToIdx(srcCol, 2)];
   }
   else
   {
      face[CubeDimsToIdx(0, destRow)] = face[CubeDimsToIdx(srcCol, 2)];
      face[CubeDimsToIdx(1, destRow)] = face[CubeDimsToIdx(srcCol, 1)];
      face[CubeDimsToIdx(2, destRow)] = face[CubeDimsToIdx(srcCol, 0)];
   }
}

template <eFaceCol TFirstCol, eFaceCol TSecondCol>
   requires(TFirstCol != TSecondCol)
inline static void SwapCols(SingleCubeFace& face)
{
   constexpr int firstCol = EnumToInt(TFirstCol);

   eCubeColor firstTemp = face[CubeDimsToIdx(0, firstCol)];
   eCubeColor secondTemp = face[CubeDimsToIdx(1, firstCol)];
   eCubeColor thirdTemp = face[CubeDimsToIdx(2, firstCol)];

   // First copy the second row to the first, then set the second to the temp
   CopyColToCol<TSecondCol, TFirstCol, false>(face);
   SetColData<TFirstCol, false>(face, firstTemp, secondTemp, thirdTemp);
}

static void RotateFaceClockwise(CubeFaceData& faceData, eCubeFace face)
{
   SingleCubeFace& faceToRotate = faceData[EnumToInt(face)];

   // Preserve the data from the top row since we need it later
   eCubeColor upperLeftCorner, upperMiddleEdge, upperRightCorner;
   GetRowData<eFaceRow::Upper, false>(
      faceToRotate, upperLeftCorner, upperMiddleEdge, upperRightCorner);

   CopyColToRow<eFaceCol::Left, eFaceRow::Upper, true>(faceToRotate);
   CopyRowToCol<eFaceRow::Lower, eFaceCol::Left, false>(faceToRotate);
   CopyColToRow<eFaceCol::Right, eFaceRow::Lower, true>(faceToRotate);
   SetColData<eFaceCol::Right, false>(
      faceToRotate, upperLeftCorner, upperMiddleEdge, upperRightCorner);
}

static void RotateFaceCounterClockwise(CubeFaceData& faceData, eCubeFace face)
{
   SingleCubeFace& faceToRotate = faceData[EnumToInt(face)];

   // Preserve the data from the top row since we need it later
   eCubeColor upperLeftCorner, upperMiddleEdge, upperRightCorner;
   GetRowData<eFaceRow::Upper, false>(
      faceToRotate, upperLeftCorner, upperMiddleEdge, upperRightCorner);

   CopyColToRow<eFaceCol::Right, eFaceRow::Upper, false>(faceToRotate);
   CopyRowToCol<eFaceRow::Lower, eFaceCol::Right, true>(faceToRotate);
   CopyColToRow<eFaceCol::Left, eFaceRow::Lower, false>(faceToRotate);
   SetColData<eFaceCol::Left, true>(faceToRotate, upperLeftCorner, upperMiddleEdge, upperRightCorner);
}

static void RotateFaceTwice(CubeFaceData& faceData, eCubeFace face)
{
   SingleCubeFace& faceToRotate = faceData[EnumToInt(face)];
   SwapCols<eFaceCol::Left, eFaceCol::Right>(faceToRotate);
   SwapRows<eFaceRow::Upper, eFaceRow::Lower>(faceToRotate);
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