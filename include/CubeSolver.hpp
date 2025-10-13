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
   NumEdges,
};

constexpr int GetNumEdgeTypes() 
{
   return static_cast<int>(eFaceEdgePos::NumEdges);
}

class CubeSolveUtils
{
public:
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