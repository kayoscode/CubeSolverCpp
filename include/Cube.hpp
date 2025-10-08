#pragma once

#include <array>
#include <type_traits>

namespace cube
{
constexpr int CubeSize = 3;

/**
 * @brief      Defines the valid colors on a cube.
 * The colors are specified on the cube such that color==face
 */
enum class eCubeColor
{
   Yellow,
   White,
   Red,
   Orange,
   Green,
   Blue,
   NumColors,
};

/**
 * @brief      All valid faces on the cube.
 * The colors are specified on the cube such that color==face
 */
enum class eCubeFace
{
   Top,
   Bottom,
   Left,
   Right,
   Front,
   Back,
   NumFaces
};

/**
 * @brief      Defines the valid moves that can be done on the cube.
 */
enum class eCubeMove
{
   Up,
   UpPrime,
   Up2,
   Down,
   DownPrime,
   Down2,
   Right,
   RightPrime,
   Right2,
   Left,
   LeftPrime,
   Left2,
   Front,
   FrontPrime,
   Front2,
   Back,
   BackPrime,
   Back2,
   UpWide,
   UpWidePrime,
   UpWide2,
   DownWide,
   DownWidePrime,
   DownWide2,
   RightWide,
   RightWidePrime,
   RightWide2,
   LeftWide,
   LeftWidePrime,
   LeftWide2,
   FrontWide,
   FrontWidePrime,
   FrontWide2,
   BackWide,
   BackWidePrime,
   BackWide2,
   X,
   Y,
   Z
};

using CubeState = 
   std::array<std::array<eCubeColor, CubeSize * CubeSize>, static_cast<int>(eCubeFace::NumFaces)>;

template <typename T>
   requires std::is_same_v<eCubeColor, T> || std::is_same_v<eCubeFace, T>
[[nodiscard]] constexpr int EnumToInt(T token)
{
   return static_cast<int>(token);
}

[[nodiscard]] constexpr int CubeDimsToIdx(int x, int y)
{
   return (y * CubeSize) + x;
}

/**
 * @brief      Defines the state of of the cube with manipulation logic.
 */
class Cube
{
public:
   /**
    * @brief      Standard constructor.
    */
   Cube();

   /**
    * @brief      Sets the cube to the default solved state.
    */
   void SetDefaultState();

   /**
    * @brief      Executes a single move.
    *
    * @param[in]  move  The move
    */
   void ExecuteMove(eCubeMove move);

   /**
    * @brief      Executes a series of moves
    *
    * @param      move      The move
    * @param[in]  numMoves  The number moves
    */
   void ExecuteMoves(eCubeMove* move, int numMoves);

   /**
    * @brief      Solves the cube using the CFOP method.
    */
   void Solve();

   /**
    * @return     True if the cube is solvable as it's currently configured.
    */
   bool Validate();

   /**
    * @brief      Returns the color at a given index of the cube.
    * Note: The orientation of the cube matters.
    * For each face, imagine you hold the cube with the top color facing up and the
    * front color facing you. Now rotate the cube directly to your target color.
    * What you see on the face of the cube is the same as what you are going to index.
    * Indexing starts from the top left and goes right on the x axis and down on the Y.

    * @return     The color state at the given position.
    */
   inline eCubeColor GetState(eCubeFace face, int x, int y)
   {
      return mCube[EnumToInt(face)][CubeDimsToIdx(x, y)];
   }

   /**
    * @brief      Sets the color to the given color at the face 
    * and the X Y coordinates given. For the indexing scheme, see GetState docs
    *
    * @param[in]  face   The face
    * @param[in]  x      The new value
    * @param[in]  y      The new value
    * @param[in]  color  The color
    */
   inline void SetState(eCubeFace face, int x, int y, eCubeColor color)
   {
      mCube[EnumToInt(face)][CubeDimsToIdx(x, y)] = color;
   }

   [[nodiscard]] constexpr eCubeFace ColorToFace(eCubeColor color)
   {
      return static_cast<eCubeFace>(color);
   }

   [[nodiscard]] constexpr eCubeColor FaceToColor(eCubeFace face)
   {
      return static_cast<eCubeColor>(face);
   }

private:
   CubeState mCube;
};
}   // namespace cube