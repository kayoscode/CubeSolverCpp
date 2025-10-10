#pragma once

#include <array>
#include <cassert>
#include <cerrno>
#include <vector>

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
   // Middle moves
   Middle,
   MiddlePrime,
   Middle2,
   Equator,
   EquatorPrime,
   Equator2,
   Standing,
   StandingPrime,
   Standing2,
   // Cube rotations.
   X,
   XPrime,
   X2,
   Y,
   YPrime,
   Y2,
   Z,
   ZPrime,
   Z2,
   NumMoves
};

using SingleCubeFace = std::array<eCubeColor, CubeSize * CubeSize>;
using CubeFaceData = std::array<SingleCubeFace, static_cast<int>(eCubeFace::NumFaces)>;

template <typename T> [[nodiscard]] constexpr int EnumToInt(T token)
{
   return static_cast<int>(token);
}

[[nodiscard]] constexpr int CubeDimsToIdx(int x, int y)
{
   return (y * CubeSize) + x;
}

/**
 * @brief      Defines operations that can be done on a cube.
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
   void SetSolved();

   /**
    * @brief      Returns true if the cube is in a solved state.
    *
    * @return     True if solved, False otherwise.
    */
   bool IsSolved();

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
   void ExecuteMoves(eCubeMove* move, size_t numMoves);

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

   /**
    * @brief      Returns the default face associated with a given color.
    *
    * @param[in]  color  The color
    *
    * @return     The cube face.
    */
   [[nodiscard]] static constexpr eCubeFace DefaultFaceOfColor(eCubeColor color)
   {
      return static_cast<eCubeFace>(color);
   }

   /**
    * @brief      Returns the default color associated with the given face.
    * This is the state if no face rotations have occurred.
    *
    * @param[in]  face  The face
    *
    * @return     The e cube color.
    */
   [[nodiscard]] static constexpr eCubeColor DefaultColorOfFace(eCubeFace face)
   {
      return static_cast<eCubeColor>(face);
   }

   /**
    * @brief      Returns the current face associated with the given color.
    *
    * @param[in]  color  The color
    *
    * @return     The cube face.
    */
   eCubeFace FaceOfColor(eCubeColor color)
   {
      for (int i = 0; i < EnumToInt(eCubeFace::NumFaces); i++)
      {
         eCubeFace face = static_cast<eCubeFace>(i);
         if (ColorOfFace(face) == color)
         {
            return face;
         }
      }

      assert(false && "Color missing from cube.");
      return DefaultFaceOfColor(color);
   }

   /**
    * @brief      Returns the current color associated with the given face.
    *
    * @param[in]  face  The face
    *
    * @return     The cube color.
    */
   eCubeColor ColorOfFace(eCubeFace face)
   {
      // Return the color of the middle of the given face.
      return mCube[EnumToInt(face)][CubeDimsToIdx(CubeSize / 2, CubeSize / 2)];
   }

   /**
    * @brief      Prints the cube to the given stream in a user readable way.
    * @return     String representation of the object.
    */
   void Print(std::ostream& outputStream, bool useColor = true);

   /**
    * @brief      Prints a single face to the output stream.
    * @param      outputStream  The stream to write data to
    */
   void PrintFace(eCubeFace face, std::ostream& outputStream, bool useColor = true);

   /**
    * @brief      Converts the list of moves to a human readable string.
    *
    * @param      outputStream  The stream to write data to
    * @param      moves         The moves
    */
   static void SerializeMoveList(
      std::ostream& outputStream, eCubeMove* moves, size_t numMoves, bool includeSeparators = false);

   /**
    * @brief      Creates a list of moves given a string of move notation.
    *
    * @param[in]  moveNotation  The move notation
    * @param      moves         The moves
    */
   static void ParseMoveNotation(const std::string& moveNotation, std::vector<eCubeMove>& moves);

   /**
    * @brief      Takes in a list of moves and produces the reverse of those moves.
    *
    * @param[in]  moves         The moves
    * @param      reverseMoves  The reverse moves
    */
   static void ReverseMoves(
      const std::vector<eCubeMove>& moves, std::vector<eCubeMove>& reverseMoves);

private:
   CubeFaceData mCube;

   class CubeStaticConstructor
   {
   public:
      CubeStaticConstructor();
   };
   // We need this static constructor to init some memory in static space in a thread safe manner.
   CubeStaticConstructor mStaticConstructor;
};
}   // namespace cube