#include "Cube.hpp"
#include "CubeSolver.hpp"

#include <cassert>
#include <map>
#include <vector>

#define CUBE_ALG_DEF(name, moves)                           \
   static const std::vector<eCubeMove>& name()              \
   {                                                        \
      static auto result = GenerateMoves(moves)();          \
      return result;                                        \
   }

namespace cube
{
   constexpr eCubeColor BottomColor = Cube::DefaultColorOfFace(eCubeFace::Bottom);
   constexpr eCubeColor FrontColor = Cube::DefaultColorOfFace(eCubeFace::Front);

   static auto GenerateMoves(const std::string& moveNotation)
   {
      return [moveNotation]()
      {
         std::vector<eCubeMove> moves;
         Cube::ParseMoveNotation(moveNotation, moves);
         return moves;
      };
   }

   class FirstLayerAlgorithms
   {
   public:
      // To use, position the inverted edge at the right side of the right face.
      CUBE_ALG_DEF(SolveInvertedEdge, "Uw R' Uw'");
   };

   /**
    * @brief      Returns the face opposite to the given one (On the other side)
    *
    * @param[in]  face  The face
    *
    * @return     The opposite face.
    */
   static eCubeFace GetOppositeFace(eCubeFace face)
   {
      switch (face)
      {
      case eCubeFace::Front:
         return eCubeFace::Back;
      case eCubeFace::Back:
         return eCubeFace::Front;
      case eCubeFace::Left:
         return eCubeFace::Right;
      case eCubeFace::Right:
         return eCubeFace::Left;
      case eCubeFace::Top:
         return eCubeFace::Bottom;
      case eCubeFace::Bottom:
         return eCubeFace::Top;
      default:
         assert(false);
         return eCubeFace::Top;
      }
   }

   tCornerDescriptor CubeSolveUtils::GetCornerDescriptor(eCubeFace face, eFaceCornerPos corner)
   {
      switch (face)
      {
      case eCubeFace::Front:
         switch (corner)
         {
         case eFaceCornerPos::TopLeft:
            return tCornerDescriptor(eCubeFace::Top, eFaceCornerPos::BottomLeft,
                                     eCubeFace::Left, eFaceCornerPos::TopRight,
                                     face, corner);
         case eFaceCornerPos::TopRight:
            return tCornerDescriptor(eCubeFace::Top, eFaceCornerPos::BottomRight,
                                     eCubeFace::Right, eFaceCornerPos::TopLeft,
                                     face, corner);
         case eFaceCornerPos::BottomLeft:
            return tCornerDescriptor(eCubeFace::Bottom, eFaceCornerPos::TopLeft,
                                     eCubeFace::Left, eFaceCornerPos::BottomRight,
                                     face, corner);
         case eFaceCornerPos::BottomRight:
            return tCornerDescriptor(eCubeFace::Bottom, eFaceCornerPos::TopRight,
                                     eCubeFace::Right, eFaceCornerPos::BottomLeft,
                                     face, corner);
         }
      case eCubeFace::Back:
         switch (corner)
         {
         case eFaceCornerPos::TopLeft:
            return tCornerDescriptor(eCubeFace::Top, eFaceCornerPos::TopRight,
                                     eCubeFace::Right, eFaceCornerPos::TopRight,
                                     face, corner);
         case eFaceCornerPos::TopRight:
            return tCornerDescriptor(eCubeFace::Top, eFaceCornerPos::TopLeft,
                                     eCubeFace::Left, eFaceCornerPos::TopLeft,
                                     face, corner);
         case eFaceCornerPos::BottomLeft:
            return tCornerDescriptor(eCubeFace::Bottom, eFaceCornerPos::BottomRight,
                                     eCubeFace::Right, eFaceCornerPos::BottomRight,
                                     face, corner);
         case eFaceCornerPos::BottomRight:
            return tCornerDescriptor(eCubeFace::Bottom, eFaceCornerPos::BottomLeft,
                                     eCubeFace::Left, eFaceCornerPos::BottomLeft,
                                     face, corner);
         }
      case eCubeFace::Left:
         switch (corner)
         {
         case eFaceCornerPos::TopLeft:
            return tCornerDescriptor(eCubeFace::Top, eFaceCornerPos::TopLeft,
                                     face, corner,
                                     eCubeFace::Back, eFaceCornerPos::TopRight);
         case eFaceCornerPos::TopRight:
            return tCornerDescriptor(eCubeFace::Top, eFaceCornerPos::BottomLeft,
                                     face, corner,
                                     eCubeFace::Front, eFaceCornerPos::TopLeft);
         case eFaceCornerPos::BottomLeft:
            return tCornerDescriptor(eCubeFace::Bottom, eFaceCornerPos::BottomLeft,
                                     face, corner,
                                     eCubeFace::Back, eFaceCornerPos::BottomRight);
         case eFaceCornerPos::BottomRight:
            return tCornerDescriptor(eCubeFace::Bottom, eFaceCornerPos::TopLeft,
                                     face, corner,
                                     eCubeFace::Front, eFaceCornerPos::BottomLeft);
         }
      case eCubeFace::Right:
         switch (corner)
         {
         case eFaceCornerPos::TopLeft:
            return tCornerDescriptor(eCubeFace::Top, eFaceCornerPos::BottomRight,
                                     face, corner,
                                     eCubeFace::Front, eFaceCornerPos::TopRight);
         case eFaceCornerPos::TopRight:
            return tCornerDescriptor(eCubeFace::Top, eFaceCornerPos::TopRight,
                                     face, corner,
                                     eCubeFace::Back, eFaceCornerPos::TopLeft);
         case eFaceCornerPos::BottomLeft:
            return tCornerDescriptor(eCubeFace::Bottom, eFaceCornerPos::TopRight,
                                     face, corner,
                                     eCubeFace::Front, eFaceCornerPos::BottomRight);
         case eFaceCornerPos::BottomRight:
            return tCornerDescriptor(eCubeFace::Bottom, eFaceCornerPos::BottomRight,
                                     face, corner,
                                     eCubeFace::Back, eFaceCornerPos::BottomLeft);
         }
      case eCubeFace::Top: 
         switch (corner)
         {
         case eFaceCornerPos::TopLeft:
            return tCornerDescriptor(face, corner,
                                     eCubeFace::Left, eFaceCornerPos::TopLeft,
                                     eCubeFace::Back, eFaceCornerPos::TopRight);
         case eFaceCornerPos::TopRight:
            return tCornerDescriptor(face, corner,
                                     eCubeFace::Right, eFaceCornerPos::TopRight,
                                     eCubeFace::Back, eFaceCornerPos::TopLeft);
         case eFaceCornerPos::BottomLeft:
            return tCornerDescriptor(face, corner,
                                     eCubeFace::Left, eFaceCornerPos::TopRight,
                                     eCubeFace::Front, eFaceCornerPos::TopLeft);
         case eFaceCornerPos::BottomRight:
            return tCornerDescriptor(face, corner,
                                     eCubeFace::Right, eFaceCornerPos::TopLeft,
                                     eCubeFace::Front, eFaceCornerPos::TopRight);
         }
      case eCubeFace::Bottom:
         switch (corner)
         {
         case eFaceCornerPos::TopLeft:
            return tCornerDescriptor(face, corner,
                                     eCubeFace::Left, eFaceCornerPos::BottomRight,
                                     eCubeFace::Front, eFaceCornerPos::BottomLeft);
         case eFaceCornerPos::TopRight:
            return tCornerDescriptor(face, corner,
                                     eCubeFace::Right, eFaceCornerPos::BottomLeft,
                                     eCubeFace::Front, eFaceCornerPos::BottomRight);
         case eFaceCornerPos::BottomLeft:
            return tCornerDescriptor(face, corner,
                                     eCubeFace::Left, eFaceCornerPos::BottomLeft,
                                     eCubeFace::Back, eFaceCornerPos::BottomRight);
         case eFaceCornerPos::BottomRight:
            return tCornerDescriptor(face, corner,
                                     eCubeFace::Right, eFaceCornerPos::BottomRight,
                                     eCubeFace::Back, eFaceCornerPos::BottomLeft);
         }
      default:
         assert(false);
         // Dummy return value.
         return tCornerDescriptor(face, corner, face, corner, face, corner);
         break;
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
      std::tuple<eCubeFace, eFaceEdgePos> result;

      switch (face)
      {
      case eCubeFace::Front:
         switch (edge)
         {
            case eFaceEdgePos::TopEdge: result = { eCubeFace::Top, eFaceEdgePos::BottomEdge }; break;
            case eFaceEdgePos::LeftEdge: result = { eCubeFace::Left, eFaceEdgePos::RightEdge }; break;
            case eFaceEdgePos::RightEdge: result = { eCubeFace::Right, eFaceEdgePos::LeftEdge }; break;
            case eFaceEdgePos::BottomEdge: result = { eCubeFace::Bottom, eFaceEdgePos::TopEdge }; break;
         }
         break;
      case eCubeFace::Back:
         switch (edge)
         {
            case eFaceEdgePos::TopEdge: result = { eCubeFace::Top, eFaceEdgePos::TopEdge }; break;
            case eFaceEdgePos::LeftEdge: result = { eCubeFace::Right, eFaceEdgePos::RightEdge }; break;
            case eFaceEdgePos::RightEdge: result = { eCubeFace::Left, eFaceEdgePos::LeftEdge }; break;
            case eFaceEdgePos::BottomEdge: result = { eCubeFace::Bottom, eFaceEdgePos::BottomEdge }; break;
         }
         break;
      case eCubeFace::Left:
         switch (edge)
         {
            case eFaceEdgePos::TopEdge: result = { eCubeFace::Top, eFaceEdgePos::LeftEdge }; break;
            case eFaceEdgePos::LeftEdge: result = { eCubeFace::Back, eFaceEdgePos::RightEdge }; break;
            case eFaceEdgePos::RightEdge: result = { eCubeFace::Front, eFaceEdgePos::LeftEdge }; break;
            case eFaceEdgePos::BottomEdge: result = { eCubeFace::Bottom, eFaceEdgePos::LeftEdge }; break;
         }
         break;
      case eCubeFace::Right:
         switch (edge)
         {
            case eFaceEdgePos::TopEdge: result = { eCubeFace::Top, eFaceEdgePos::RightEdge }; break;
            case eFaceEdgePos::LeftEdge: result = { eCubeFace::Front, eFaceEdgePos::RightEdge }; break;
            case eFaceEdgePos::RightEdge: result = { eCubeFace::Back, eFaceEdgePos::LeftEdge }; break;
            case eFaceEdgePos::BottomEdge: result = { eCubeFace::Bottom, eFaceEdgePos::RightEdge }; break;
         }
         break;
      case eCubeFace::Top:
         switch (edge)
         {
            case eFaceEdgePos::TopEdge: result = { eCubeFace::Back, eFaceEdgePos::TopEdge }; break;
            case eFaceEdgePos::LeftEdge: result = { eCubeFace::Left, eFaceEdgePos::TopEdge }; break;
            case eFaceEdgePos::RightEdge: result = { eCubeFace::Right, eFaceEdgePos::TopEdge }; break;
            case eFaceEdgePos::BottomEdge: result = { eCubeFace::Front, eFaceEdgePos::TopEdge }; break;
         }
         break;
      case eCubeFace::Bottom:
         switch (edge)
         {
            case eFaceEdgePos::TopEdge: result = { eCubeFace::Front, eFaceEdgePos::BottomEdge }; break;
            case eFaceEdgePos::LeftEdge: result = { eCubeFace::Left, eFaceEdgePos::BottomEdge }; break;
            case eFaceEdgePos::RightEdge: result = { eCubeFace::Right, eFaceEdgePos::BottomEdge }; break;
            case eFaceEdgePos::BottomEdge: result = { eCubeFace::Back, eFaceEdgePos::BottomEdge }; break;
         }
         break;
      default:
         assert(false);
         break;
      };

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
      default:
         assert(false);
         break;
      }
   }

   void CubeSolveUtils::GetCornerPosition(eFaceCornerPos corner, int& resultX, int& resultY)
   {
      switch (corner)
      {
      case eFaceCornerPos::TopLeft:
         resultX = 0;
         resultY = 0;
         break;
      case eFaceCornerPos::TopRight:
         resultX = 2;
         resultY = 0;
         break;
      case eFaceCornerPos::BottomLeft:
         resultX = 0;
         resultY = 2;
         break;
      case eFaceCornerPos::BottomRight:
         resultX = 2;
         resultY = 2;
         break;
      default:
         assert(false);
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
   static void RotateSideFaceToFront(Cube& cube, eCubeFace face, CubeMoveList& moveList, bool acceptMoves)
   {
      if (face != eCubeFace::Front)
      {
         eCubeMove orientingMove;

         switch (face)
         {
         case eCubeFace::Left:
            orientingMove = eCubeMove::YPrime;
            break;
         case eCubeFace::Right:
            orientingMove = eCubeMove::Y;
            break;
         case eCubeFace::Back:
            orientingMove = eCubeMove::Y2;
            break;
         default:
            // Error case? Cannot be the front, bottom, or top at this point.
            orientingMove = eCubeMove::NumMoves;
            break;
         }

         moveList.PushMove(orientingMove, acceptMoves);
      }
   }

   /**
    * @brief      Rotates a face that appears on the front back left or right of the cube to the right side
    * of the cube..
    *
    * @param      cube      The cube
    * @param[in]  face      The face
    * @param      moveList  The move list
    */
   static void RotateSideFaceToRight(Cube& cube, eCubeFace face, CubeMoveList& moveList, bool acceptMoves)
   {
      if (face != eCubeFace::Right)
      {
         eCubeMove orientingMove;

         switch (face)
         {
         case eCubeFace::Front:
            orientingMove = eCubeMove::YPrime;
            break;
         case eCubeFace::Back:
            orientingMove = eCubeMove::Y;
            break;
         case eCubeFace::Left:
            orientingMove = eCubeMove::Y2;
            break;
         default:
            // Error case? Cannot be the front, bottom, or top at this point.
            orientingMove = eCubeMove::NumMoves;
            break;
         }

         moveList.PushMove(orientingMove, acceptMoves);
      }
   }

   static bool OrientCube(Cube& cube, std::ostream& outputStream, bool useSeparators)
   {
      // We want white on the bottom.
      CubeMoveList moveList(cube);

      if (cube.ColorOfFace(eCubeFace::Bottom) != BottomColor)
      {
         eCubeFace bottomColorFace = cube.FaceOfColor(BottomColor);
         eCubeMove orientingMove;

         switch (bottomColorFace)
         {
         case eCubeFace::Front:
            orientingMove = eCubeMove::XPrime;
            break;
         case eCubeFace::Left:
            orientingMove = eCubeMove::ZPrime;
            break;
         case eCubeFace::Right:
            orientingMove = eCubeMove::Z;
            break;
         case eCubeFace::Back:
            orientingMove = eCubeMove::X;
            break;
         case eCubeFace::Top:
            orientingMove = eCubeMove::X2;
            break;
         default:
            // Error case?
            orientingMove = eCubeMove::NumMoves;
            break;
         }

         moveList.PushMove(orientingMove, true);
      }

      if (moveList.GetNumMoves() > 0)
      {
         outputStream << "Orienting: ";
         moveList.SerializeMoves(outputStream);
         outputStream << "\n";
         return true;
      }

      return false;
   }

   #pragma region Cross

   /**
    * @brief      Returns true if the edge on the existing face is solved. False otherwise
    */
   static bool IsFaceCrossSolved(Cube& cube, eCubeFace face)
   {
      eCubeColor faceColor = cube.ColorOfFace(face);
      bool isInverted;
      bool edgeInPosition = CubeSolveUtils::IsEdgeInPosition(cube, face, eFaceEdgePos::BottomEdge, 
         faceColor, BottomColor, isInverted);

      return edgeInPosition && !isInverted;
   }

   /**
    * @brief      Performs the series of moves to get the edge on the top of the front face instead of
    * The back.
    *
    * @param      cube      The cube
    * @param      moveList  The move list
    * @return     True if the edge was found on the left face and it's now on the top of the right face.
    */
   static void SolveCrossEdgeOnLeftFace(Cube& cube, CubeMoveList& moveList)
   {
      // Just bring the edge to the top edge, then do a U2.
      // We know the opposite face is the left
      eCubeColor edgeColor = cube.ColorOfFace(eCubeFace::Right);

      for (int i = 0; i < GetNumEdgeTypes(); i++)
      {
         eFaceEdgePos edgePos = static_cast<eFaceEdgePos>(i);
         bool isInverted;
         bool edgeInPosition = CubeSolveUtils::IsEdgeInPosition(cube, eCubeFace::Left, edgePos, edgeColor, 
            BottomColor, isInverted);

         bool isLeftFaceSolvedBefore = IsFaceCrossSolved(cube, eCubeFace::Left);
         if (edgeInPosition)
         {
            switch (edgePos)
            {
            case eFaceEdgePos::TopEdge:
               // Simple, do a U2.
               moveList.PushMove(eCubeMove::Up2);
               break;
            case eFaceEdgePos::BottomEdge:
               // Edge cannot be solved, L2 U2
               moveList.PushMove(eCubeMove::Left2);
               moveList.PushMove(eCubeMove::Up2);
               break;
            case eFaceEdgePos::LeftEdge:
               // We have to be careful to not mess up the edge if it's already solved.
               moveList.PushMove(eCubeMove::Left);
               moveList.PushMove(eCubeMove::Up2);
               if (isLeftFaceSolvedBefore)
               {
                  moveList.PushMove(eCubeMove::LeftPrime);
               }
               break;
            case eFaceEdgePos::RightEdge:
               moveList.PushMove(eCubeMove::LeftPrime);
               moveList.PushMove(eCubeMove::Up2);
               if (isLeftFaceSolvedBefore)
               {
                  moveList.PushMove(eCubeMove::Left);
               }
               break;
            default:
               assert(false);
               break;
            }

            // Accept all pending moves here.
            moveList.AcceptPendingMoves();
         }
      }
   }

   /**
    * @brief      Solves the edge assuming it exists somewhere on the right face.
    * Note: we are solving the edge of the right face.
    *
    * @param      cube      The cube
    * @param      moveList  The move list
    */
   static void SolveCrossEdgeOnRightFace(Cube& cube, CubeMoveList& moveList)
   {
      eCubeColor color = cube.ColorOfFace(eCubeFace::Right);

      // Now that we know it's on the right, find it and solve.
      for (int i = 0; i < GetNumEdgeTypes(); i++)
      {
         eFaceEdgePos edgePos = static_cast<eFaceEdgePos>(i);
         bool isInverted;
         bool isEdgeInPosition = CubeSolveUtils::IsEdgeInPosition(cube, eCubeFace::Right, edgePos, color, 
            BottomColor, isInverted);

         if (isEdgeInPosition)
         {
            switch (edgePos)
            {
            case eFaceEdgePos::TopEdge:
               if (!isInverted)
               {
                  // Just move it to the bottom and it's solved
                  moveList.PushMove(eCubeMove::Right2);
               }
               else 
               {
                  // Move to the right edge, then solve the inverted edge.
                  moveList.PushMove(eCubeMove::Right);
                  moveList.PushMoves(FirstLayerAlgorithms::SolveInvertedEdge());
               }
               break;
            case eFaceEdgePos::BottomEdge:
               // If it's not inverted, it's solved, but if it is, then we have to solve.
               if (!isInverted)
               {
                  // We shouldn't get here since we checked at the beginning that the edge was solved.
                  assert(false);
               }
               else 
               {
                  // Prep with R prime to get the inverted edge on the right.
                  moveList.PushMove(eCubeMove::RightPrime);
                  moveList.PushMoves(FirstLayerAlgorithms::SolveInvertedEdge());
               }

               break;
            case eFaceEdgePos::LeftEdge:
               if (!isInverted)
               {
                  // Move it to the bottom with RPrime
                  moveList.PushMove(eCubeMove::RightPrime);
               }
               else
               {
                  // Prep then solve
                  moveList.PushMove(eCubeMove::Right2);
                  moveList.PushMoves(FirstLayerAlgorithms::SolveInvertedEdge());
               }
               break;
            case eFaceEdgePos::RightEdge:
               if (!isInverted)
               {
                  // Just move it to the bottom
                  moveList.PushMove(eCubeMove::Right);
               }
               else
               {
                  // It's already prepped, so just solve
                  moveList.PushMoves(FirstLayerAlgorithms::SolveInvertedEdge());
               }
               break;
            default:
               assert(false);
               break;
            }
            moveList.AcceptPendingMoves();
            return;
         }
      }
   }

   /**
    * Completely solves the edge if it appears in the bottom middle layer.
    *
    * @param      cube      The cube
    * @param      moveList  The move list
    *
    * @return     { description_of_the_return_value }
    */
   static bool SolveCrossEdgeInBottomMiddle(Cube& cube, CubeMoveList& moveList)
   {
      eCubeColor color = cube.ColorOfFace(eCubeFace::Right);

      bool isInverted;
      if (CubeSolveUtils::IsEdgeInPosition(cube, eCubeFace::Bottom, 
         eFaceEdgePos::TopEdge, BottomColor, color, isInverted))
      {
         if (!isInverted)
         {
            moveList.PushMoves({ eCubeMove::Front2, eCubeMove::UpPrime, eCubeMove::Right2 });
         }
         else
         {
            moveList.PushMoves({ eCubeMove::FrontPrime, eCubeMove::RightPrime });
         }

         moveList.AcceptPendingMoves();
         return true;
      }

      // If it's on the bottom back.
      if (CubeSolveUtils::IsEdgeInPosition(cube, eCubeFace::Bottom, 
         eFaceEdgePos::BottomEdge, BottomColor, color, isInverted))
      {
         if (!isInverted)
         {
            moveList.PushMoves({ eCubeMove::Back2, eCubeMove::Up, eCubeMove::Right2 });
         }
         else
         {
            moveList.PushMoves({ eCubeMove::Back, eCubeMove::Right });
         }

         moveList.AcceptPendingMoves();
         return true;
      }

      return false;
   }

   /**
    * @brief      Solves the case where the right edge appears in the top middle layer
    * Bring it to the right side.
    *
    * @param      cube      The cube
    * @param      moveList  The move list
    *
    * @return     { description_of_the_return_value }
    */
   static bool SolveCrossEdgeInTopMiddle(Cube& cube, CubeMoveList& moveList)
   {
      eCubeColor color = cube.ColorOfFace(eCubeFace::Right);

      bool isInverted;
      if (CubeSolveUtils::IsEdgeInPosition(cube, eCubeFace::Top, 
         eFaceEdgePos::TopEdge, BottomColor, color, isInverted))
      {
         if (!isInverted)
         {
            moveList.PushMoves({ eCubeMove::Up, eCubeMove::Right2 });
         }
         else
         {
            bool wasBackSolved = IsFaceCrossSolved(cube, eCubeFace::Back);
            moveList.PushMoves({ eCubeMove::BackPrime, eCubeMove::Right });

            if (wasBackSolved)
            {
               // Fix the back face which we just obliterated.
               moveList.PushMove(eCubeMove::Back);
            }
         }

         moveList.AcceptPendingMoves();
         return true;
      }

      // If it's on the bottom back.
      if (CubeSolveUtils::IsEdgeInPosition(cube, eCubeFace::Top, 
         eFaceEdgePos::BottomEdge, BottomColor, color, isInverted))
      {
         if (!isInverted)
         {
            moveList.PushMoves({ eCubeMove::UpPrime, eCubeMove::Right2 });
         }
         else
         {
            bool wasFrontSolved = IsFaceCrossSolved(cube, eCubeFace::Front);
            moveList.PushMoves({ eCubeMove::Front, eCubeMove::RightPrime });

            if (wasFrontSolved)
            {
               // Fix the front edge which we just obliterated.
               moveList.PushMove(eCubeMove::FrontPrime);
            }
         }

         moveList.AcceptPendingMoves();
         return true;
      }

      return false;
   }

   static bool SolveCrossFace(Cube& cube, eCubeFace face, CubeMoveList& moveList)
   {
      if (IsFaceCrossSolved(cube, face))
      {
         // Nothing to do.
         return false;
      }

      // To solve the cross, we should first rotate the face we want to solve to the right but not 
      // accept that move. Then iterate through all the edges on that face and see if it's in that positiion
      // either inverted or not. If it's inverted, we have to solve the inverted edge, otherwise we can
      // just rotate it into position. 
      // If the edge isn't found on this face, then we know it's on the back face (either inverted or not).
      // Repeat this space process on the back, but solve it using the prep step of bringing the edge
      // to the top and rotating it around to this face.

      // First, bring the face to the right for simpler logic.
      RotateSideFaceToRight(cube, face, moveList, false);

      if (SolveCrossEdgeInBottomMiddle(cube, moveList))
      {
         // Edge solved
         assert(IsFaceCrossSolved(cube, eCubeFace::Right) && "Edge should have been solved by now.");
         return true;
      }

      if (SolveCrossEdgeInTopMiddle(cube, moveList))
      {
         // Edge solved
         assert(IsFaceCrossSolved(cube, eCubeFace::Right) && "Edge should have been solved by now.");
         return true;
      }

      // Now, we know it's either on the left or the right faces, solve that case.
      SolveCrossEdgeOnLeftFace(cube, moveList);
      SolveCrossEdgeOnRightFace(cube, moveList);

      // Make sure it's actually solved.
      assert(IsFaceCrossSolved(cube, eCubeFace::Right) && "Edge should have been solved by now.");
      return true;
   }

   /**
    * @brief      Verifies as a quick self test that the cross has actually been solved.
    * Asserts false if it's hasn't.
    */
   static void EnsureCrossSolved(Cube& cube)
   {
      assert(BottomColor == cube.ColorOfFace(eCubeFace::Bottom) && "Bottom color incorrect.");
      assert(IsFaceCrossSolved(cube, eCubeFace::Front) && "Front face not solved");
      assert(IsFaceCrossSolved(cube, eCubeFace::Back) && "Back face not solved");
      assert(IsFaceCrossSolved(cube, eCubeFace::Left) && "Left face not solved");
      assert(IsFaceCrossSolved(cube, eCubeFace::Right) && "Right face not solved");
   }

   static bool SolveCross(Cube& cube, std::ostream& outputStream, bool useSeparators)
   {
      CubeMoveList moveList(cube);

      int faceSolveCounter = 0;
      // 4 faces.
      constexpr int maxIterations = 4;

      while (faceSolveCounter < maxIterations)
      {
         bool neededSolve = false;

         // Attempt to solve each side face.
         if (SolveCrossFace(cube, eCubeFace::Right, moveList))
         {
            neededSolve = true;
            faceSolveCounter++;
            continue;
         }

         if (SolveCrossFace(cube, eCubeFace::Front, moveList))
         {
            neededSolve = true;
            faceSolveCounter++;
            continue;
         }

         if (SolveCrossFace(cube, eCubeFace::Back, moveList))
         {
            neededSolve = true;
            faceSolveCounter++;
            continue;
         }

         if (SolveCrossFace(cube, eCubeFace::Left, moveList))
         {
            neededSolve = true;
            faceSolveCounter++;
            continue;
         }

         // If we got all the way around without having to solve a face, were done
         if (!neededSolve)
         {
            break;
         }
      }

      // Quick self test to make sure the cross has been solved.
      EnsureCrossSolved(cube);

      if (moveList.GetNumMoves() > 0)
      {
         outputStream << "Cross: ";
         moveList.SerializeMoves(outputStream);
         outputStream << "\n";
         return true;
      }

      outputStream << "Cross: Already solved";
      return false;
   }

   #pragma endregion // Cross

   #pragma region F2L

   static bool SolveFirstTwoLayers(Cube& cube, std::ostream& outputStream, bool addSeparators)
   {
      CubeMoveList moveList(cube);

      // TODO Ensure F2L solved

      if (moveList.GetNumMoves() > 0)
      {
         outputStream << "F2L: ";
         moveList.SerializeMoves(outputStream);
         outputStream << "\n";
         return true;
      }

      outputStream << "F2L: Already Solved";
      return false;
   }

   #pragma endregion F2L

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

      if (SolveFirstTwoLayers(mCube, outputStream, mAddSeparators) && mShowCubeAfterEachStep)
      {
         mCube.Print(outputStream);
      }
   }
}