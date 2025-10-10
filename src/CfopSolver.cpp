#include "Cube.hpp"
#include "CubeSolver.hpp"
#include <map>

namespace cube
{
   constexpr eCubeColor BottomColor = Cube::DefaultColorOfFace(eCubeFace::Bottom);
   constexpr eCubeColor FrontColor = Cube::DefaultColorOfFace(eCubeFace::Front);

   void AppendMoves(Cube& cube, std::vector<eCubeMove>& moveList, const std::vector<eCubeMove>& newMoves)
   {
      for (int i = 0; i < newMoves.size(); i++)
      {
         cube.ExecuteMove(newMoves[i]);
         moveList.push_back(newMoves[i]);
      }
   }

   /**
    * @brief      Returns the information about the adjacent edge to the given edge on the given face.
    *
    * @param[in]  face        The face
    * @param[in]  edge        The edge
    * @param      resultFace  The result face
    * @param      resultEdge  The result edge
    */
   void CubeSolveUtils::GetAdjacentEdge(eCubeFace face, eFaceEdgePos edge, 
      eCubeFace& resultFace, eFaceEdgePos& resultEdge)
   {
      static std::map<eCubeFace, std::map<eFaceEdgePos, std::tuple<eCubeFace, eFaceEdgePos>>> adjacentEdgeMappings
      {
         { 
            eCubeFace::Front, 
            {
               { eFaceEdgePos::TopEdge, { eCubeFace::Top, eFaceEdgePos::BottomEdge } },
               { eFaceEdgePos::LeftEdge, { eCubeFace::Left, eFaceEdgePos::RightEdge } },
               { eFaceEdgePos::RightEdge, { eCubeFace::Right, eFaceEdgePos::LeftEdge } },
               { eFaceEdgePos::BottomEdge, { eCubeFace::Bottom, eFaceEdgePos::TopEdge } },
            },
         },
         {
            eCubeFace::Left, 
            {
               { eFaceEdgePos::TopEdge, { eCubeFace::Top, eFaceEdgePos::LeftEdge } },
               { eFaceEdgePos::LeftEdge, { eCubeFace::Back, eFaceEdgePos::RightEdge } },
               { eFaceEdgePos::RightEdge, { eCubeFace::Front, eFaceEdgePos::LeftEdge } },
               { eFaceEdgePos::BottomEdge, { eCubeFace::Bottom, eFaceEdgePos::LeftEdge } },
            }
         },
         {
            eCubeFace::Back, 
            {
               { eFaceEdgePos::TopEdge, { eCubeFace::Top, eFaceEdgePos::TopEdge } },
               { eFaceEdgePos::LeftEdge, { eCubeFace::Right, eFaceEdgePos::RightEdge } },
               { eFaceEdgePos::RightEdge, { eCubeFace::Left, eFaceEdgePos::LeftEdge } },
               { eFaceEdgePos::BottomEdge, { eCubeFace::Bottom, eFaceEdgePos::BottomEdge } },
            }
         },
         {
            eCubeFace::Right, 
            {
               { eFaceEdgePos::TopEdge, { eCubeFace::Top, eFaceEdgePos::RightEdge } },
               { eFaceEdgePos::LeftEdge, { eCubeFace::Front, eFaceEdgePos::RightEdge } },
               { eFaceEdgePos::RightEdge, { eCubeFace::Back, eFaceEdgePos::LeftEdge } },
               { eFaceEdgePos::BottomEdge, { eCubeFace::Bottom, eFaceEdgePos::RightEdge } },
            }
         },
         {
            eCubeFace::Top, 
            {
               { eFaceEdgePos::TopEdge, { eCubeFace::Back, eFaceEdgePos::TopEdge } },
               { eFaceEdgePos::LeftEdge, { eCubeFace::Left, eFaceEdgePos::TopEdge } },
               { eFaceEdgePos::RightEdge, { eCubeFace::Right, eFaceEdgePos::TopEdge } },
               { eFaceEdgePos::BottomEdge, { eCubeFace::Front, eFaceEdgePos::TopEdge } },
            }
         },
         {
            eCubeFace::Bottom, 
            {
               { eFaceEdgePos::TopEdge, { eCubeFace::Front, eFaceEdgePos::BottomEdge } },
               { eFaceEdgePos::LeftEdge, { eCubeFace::Left, eFaceEdgePos::BottomEdge } },
               { eFaceEdgePos::RightEdge, { eCubeFace::Right, eFaceEdgePos::BottomEdge } },
               { eFaceEdgePos::BottomEdge, { eCubeFace::Back, eFaceEdgePos::BottomEdge } },
            }
         },
      };

      std::tuple<eCubeFace, eFaceEdgePos> result = adjacentEdgeMappings[face][edge];
      resultFace = std::get<0>(result);
      resultEdge = std::get<1>(result);
   }

   /**
    * @brief      Returns the position of the edge on a face
    *
    * @param[in]  edge     The edge
    * @param      resultX  The result x
    * @param      resultY  The result y
    */
   void CubeSolveUtils::GetEdgePosition(eFaceEdgePos edge, int& resultX, int& resultY)
   {
      switch (edge)
      {
      case eFaceEdgePos::TopEdge:
         resultX = 1;
         resultY = 0;
         break;
      case eFaceEdgePos::LeftEdge:
         resultX = 0;
         resultY = 1;
         break;
      case eFaceEdgePos::RightEdge:
         resultX = 2;
         resultY = 1;
         break;
      case eFaceEdgePos::BottomEdge:
         resultX = 1;
         resultY = 2;
         break;
      }
   }

   eCubeColor CubeSolveUtils::GetAdjacentEdgeColor(Cube& cube, eCubeFace face, eFaceEdgePos edgePos)
   {
      int adjacentX, adjacentY;
      eCubeFace adjacentFace;
      eFaceEdgePos adjacentEdgePos;
      CubeSolveUtils::GetAdjacentEdge(face, edgePos, adjacentFace, adjacentEdgePos);
      CubeSolveUtils::GetEdgePosition(adjacentEdgePos, adjacentX, adjacentY);

      return cube.GetState(adjacentFace, adjacentX, adjacentY);
   }

   bool CubeSolveUtils::IsEdgeInPosition(Cube& cube, eCubeFace face, eFaceEdgePos edgePos, 
      eCubeColor color1, eCubeColor color2, bool& isInverted)
   {
      int faceX, faceY;
      GetEdgePosition(edgePos, faceX, faceY);

      eCubeColor face1Color = cube.GetState(face, faceX, faceY);
      if (face1Color == color1)
      {
         isInverted = false;
         eCubeColor adjacentColor = GetAdjacentEdgeColor(cube, face, edgePos);
         return adjacentColor == color2;
      }
      else if (face1Color == color2)
      {
         isInverted = true;
         eCubeColor adjacentColor = GetAdjacentEdgeColor(cube, face, edgePos);
         return adjacentColor == color1;
      }

      return false;
   }

   /**
    * @brief      Rotates a face that appears on the front back left or right of the cube to the front.
    *
    * @param      cube      The cube
    * @param[in]  face      The face
    * @param      moveList  The move list
    */
   static void RotateSideFaceToFront(Cube& cube, eCubeFace face, std::vector<eCubeMove>& moveList)
   {
      if (face != eCubeFace::Front)
      {
         eCubeMove orientingMove;

         switch (face)
         {
         case cube::eCubeFace::Left:
            orientingMove = eCubeMove::YPrime;
            break;
         case cube::eCubeFace::Right:
            orientingMove = eCubeMove::Y;
            break;
         case cube::eCubeFace::Back:
            orientingMove = eCubeMove::Y2;
            break;
         default:
            // Error case? Cannot be the front, bottom, or top at this point.
            orientingMove = eCubeMove::NumMoves;
            break;
         }

         moveList.push_back(orientingMove);
         cube.ExecuteMove(orientingMove);
      }
   }

   static bool OrientCube(Cube& cube, std::ostream& outputStream, bool useSeparators)
   {
      // We want white on the bottom.
      std::vector<eCubeMove> moveList;

      if (cube.ColorOfFace(eCubeFace::Bottom) != BottomColor)
      {
         eCubeFace bottomColorFace = cube.FaceOfColor(BottomColor);
         eCubeMove orientingMove;

         switch (bottomColorFace)
         {
         case cube::eCubeFace::Front:
            orientingMove = eCubeMove::XPrime;
            break;
         case cube::eCubeFace::Left:
            orientingMove = eCubeMove::ZPrime;
            break;
         case cube::eCubeFace::Right:
            orientingMove = eCubeMove::Z;
            break;
         case cube::eCubeFace::Back:
            orientingMove = eCubeMove::X;
            break;
         case cube::eCubeFace::Top:
            orientingMove = eCubeMove::X2;
            break;
         default:
            // Error case?
            orientingMove = eCubeMove::NumMoves;
            break;
         }

         moveList.push_back(orientingMove);
         cube.ExecuteMove(orientingMove);
      }

      if (moveList.size() > 0)
      {
         outputStream << "Orienting: ";
         Cube::SerializeMoveList(outputStream, moveList.data(), moveList.size());
         outputStream << "\n";
         return true;
      }

      return false;
   }

   void SolveCrossFace(Cube& cube, eCubeFace face, std::ostream& outputStream, std::vector<eCubeMove>& moves)
   {
      eCubeColor faceColor = cube.ColorOfFace(face);

      bool isInverted;
      bool edgeInPosition = CubeSolveUtils::IsEdgeInPosition(cube, face, eFaceEdgePos::BottomEdge, 
         faceColor, BottomColor, isInverted);

      // Edge already in position.
      if (edgeInPosition && !isInverted)
      {
         return;
      }

      std::vector<eCubeMove> moveList;
      RotateSideFaceToFront(cube, face, moveList);

      // Handle the case where the edge is in position, but inverted.
      if (edgeInPosition && isInverted)
      {
         //AppendMoves(cube, moveList, tCrossAlgorithms::SolveInvertedEdge);
      }
   }

   bool SolveCross(Cube& cube, std::ostream& outputStream, bool useSeparators)
   {
      std::vector<eCubeMove> moves;
      SolveCrossFace(cube, eCubeFace::Front, outputStream, moves);
      SolveCrossFace(cube, eCubeFace::Left, outputStream, moves);
      SolveCrossFace(cube, eCubeFace::Back, outputStream, moves);
      SolveCrossFace(cube, eCubeFace::Right, outputStream, moves);

      if (moves.size() > 0)
      {
         outputStream << "Cross: ";
         Cube::SerializeMoveList(outputStream, moves.data(), moves.size());
         return true;
      }

      outputStream << "Cross: Already solved";
      return false;
   }

   void CfopSolver::Solve(std::ostream& outputStream)
   {
      if (OrientCube(mCube, outputStream, mAddSeparators) && mShowCubeAfterEachStep)
      {
         mCube.Print(outputStream);
      }

      if (SolveCross(mCube, outputStream, mAddSeparators) && mShowCubeAfterEachStep)
      {
         mCube.Print(outputStream);
      }
   }
}