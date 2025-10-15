#pragma once

#include "Cube.hpp"
#include <ostream>

namespace cube
{
enum class eFaceEdgePos
{
   TopEdge,
   LeftEdge,
   RightEdge,
   BottomEdge,
};

enum class eFaceCornerPos
{
   TopLeft,
   TopRight,
   BottomLeft,
   BottomRight
};

constexpr int GetNumEdgeTypes() 
{
   return static_cast<int>(eFaceEdgePos::BottomEdge) + 1;
}

constexpr int GetNumCornerTypes() 
{
   return static_cast<int>(eFaceCornerPos::BottomRight) + 1;
}

struct tCornerDescriptor
{
   constexpr tCornerDescriptor(eCubeFace faceY, eFaceCornerPos cornerY, eCubeFace faceX, eFaceCornerPos cornerX,
      eCubeFace faceZ, eFaceCornerPos cornerZ)
   : FaceY(faceY), CornerY(cornerY), FaceX(faceX), CornerX(cornerX), FaceZ(faceZ), CornerZ(cornerZ)
   {
   }

   eCubeFace FaceY;
   eFaceCornerPos CornerY;

   eCubeFace FaceX;
   eFaceCornerPos CornerX;

   eCubeFace FaceZ;
   eFaceCornerPos CornerZ;
};

/**
 * @brief      Stores a list of moves. Abstracts away logic allowing the moves to be 
 * simulated but not executed depending on the state of the cube. This simplifies the logic
 * in certain places during solve;
 */
class CubeMoveList
{
public:
   CubeMoveList(Cube& cube) : mCube(cube)
   {
   }

   /**
    * @brief      Simulates the moves, but doesn't append to the move list yet.
    *
    * @param      cube   The cube
    * @param[in]  moves  The moves
    */
   void PushMoves(const std::vector<eCubeMove>& moves, bool acceptMoves = false)
   {
      for (const auto& move : moves)
      {
         mPendingMoves.push_back(move);
         mCube.ExecuteMove(move);
      }

      if (acceptMoves)
      {
         AcceptPendingMoves();
      }
   }

   /**
    * @brief      Simulates the move, but doesn't push it to the move list yet.
    *
    * @param      cube  The cube
    * @param[in]  move  The move
    */
   void PushMove(eCubeMove move, bool acceptMoves = false)
   {
      mPendingMoves.push_back(move);
      mCube.ExecuteMove(move);

      if (acceptMoves)
      {
         AcceptPendingMoves();
      }
   }

   /**
    * @brief      Pushes all pending moves to the moves list.
    */
   void AcceptPendingMoves()
   {
      mMoves.insert(mMoves.end(), mPendingMoves.begin(), mPendingMoves.end());
      mPendingMoves.clear();
   }

   /**
    * @brief      Reverses the pending moves on the cube and doesn't add them to the move list. 
    */
   void RejectPendingMoves()
   {
      std::vector<eCubeMove> reverseMoves;
      Cube::ReverseMoves(mPendingMoves, reverseMoves);
      mCube.ExecuteMoves(reverseMoves.data(), reverseMoves.size());
   }

   /**
    * @brief      Returns the list of moves applied to the cube.
    * Failing to accept or reject the pending moves before calling this is seen as a bug.
    * Although this reference is still valid after calling since we don't copy, it is recommended
    * to discard it immediately after use and call it again when it's needed later.
    * 
    * @return     The moves.
    */
   const std::vector<eCubeMove>& GetMoves()
   {
      // Assert false to catch issues where we didn't accept or reject moves leaving the cube
      // in an incorrect state.
      assert(mPendingMoves.size() == 0 && "There are pending moves.");
      return mMoves;
   }

   /**
    * @brief      Prints the list of moves to the output stream.
    * @param      outputStream  The stream to write data to
    */
   void SerializeMoves(std::ostream& outputStream)
   {
      assert(mPendingMoves.size() == 0 && "There shuold be no pending moves.");
      mCube.SerializeMoveList(outputStream, mMoves.data(), mMoves.size());
   }

   int GetNumMoves()
   {
      return mMoves.size();
   }

   int GetNumPendingMoves()
   {
      return mPendingMoves.size();
   }

private:
   Cube& mCube;
   std::vector<eCubeMove> mMoves;
   std::vector<eCubeMove> mPendingMoves;
};

class CubeSolveUtils
{
public:
   /**
    * @brief      Returns the details about the given corner. Fills all information
    *
    * @param[in]  face              The face
    * @param[in]  corner            The corner
    * @param      resultDescriptor  The result descriptor
    */
   static tCornerDescriptor GetCornerDescriptor(eCubeFace face, eFaceCornerPos corner);

   /**
    * @brief      Finds the postion on the cube where the adjacent edge can be found.
    *
    * @param[in]  face        The face
    * @param[in]  edge        The edge
    * @param      resultFace  The result face
    * @param      resultEdge  The result edge
    */
   static void GetAdjacentEdge(eCubeFace face, eFaceEdgePos edge, 
      eCubeFace& resultFace, eFaceEdgePos& resultEdge);

   /**
    * @brief      Returns the color of the adjacent edge.
    *
    * @param      cube     The cube
    * @param[in]  face     The face
    * @param[in]  edgePos  The edge position
    *
    * @return     The adjacent edge color.
    */
   static eCubeColor GetAdjacentEdgeColor(Cube& cube, eCubeFace face, eFaceEdgePos edgePos);

   /**
    * @brief      Finds the coords on the face correlating with the given edge position.
    *
    * @param[in]  edge     The edge
    * @param      resultX  The result x
    * @param      resultY  The result y
    */
   static void GetEdgePosition(eFaceEdgePos edge, int& resultX, int& resultY);

   /**
    * @brief      Returns the position on the face given the corner position enum.
    *
    * @param[in]  corner   The corner
    * @param      resultX  The result x
    * @param      resultY  The result y
    */
   static void GetCornerPosition(eFaceCornerPos corner, int& resultX, int& resultY);

   /**
    * @brief      Determines if the edge is in the given position on the given face.
    * isInverted is set to false if it found the edge, but color 1 is on the given face and color 2 is
    * on the ajacent face. It's set to true if color1 is on the ajacent face and color2 is on the face.
    *
    * @param[in]  cube        The cube
    * @param[in]  face        The face
    * @param[in]  edgePos     The edgePosition
    * @param[in]  color1      The color 1
    * @param[in]  color2      The color 2
    * @param      isInverted  Indicates if inverted
    *
    * @return     True if the edge is in the given position, false otherwise.
    */
   static bool IsEdgeInPosition(Cube& cube, eCubeFace face, eFaceEdgePos edgePos, 
      eCubeColor color1, eCubeColor color2, bool& isInverted);
};

class CubeSolver
{
public:
   /**
    * @brief      Standard constructor
    *
    * @param      cube  The cube
    */
   CubeSolver(Cube& cube) : mCube(cube)
   {
   }

   virtual ~CubeSolver()
   {
   }

   /**
    * @brief      Implement this method to solve the cube. The series of moves to solve should be
    * written to the output stream.
    */
   virtual void Solve(std::ostream& outputStream) = 0;

protected:
   Cube& mCube;
};

class CfopSolver : public CubeSolver
{
public:
   CfopSolver(Cube& cube, bool showCubeAfterEachStep = false, bool addSeparators = false)
      : CubeSolver(cube), mShowCubeAfterEachStep(showCubeAfterEachStep), mAddSeparators(addSeparators)
   {
   }

   /**
    * @brief      Solves the cube using the cfop method.
    *
    * @param      outputStream  The stream to write data to
    */
   void Solve(std::ostream& outputStream);

private:
   bool mShowCubeAfterEachStep;
   bool mAddSeparators;
};
}   // namespace cube