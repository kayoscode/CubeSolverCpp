#include "Cube.hpp"
#include "CubeSolver.hpp"

#include <bitset>
#include <cassert>
#include <string>
#include <tuple>
#include <vector>

#define CUBE_ALG_DEF(name, moves)                           \
   static const std::vector<eCubeMove>& name()              \
   {                                                        \
      static auto result = GenerateMoves(moves)();          \
      return result;                                        \
   }

#define CUBE_OLL_DEF(name, moves, topFace, frontFace, rightFace, backFace, leftFace) \
   static const tOLLPattern& name()                                                  \
   {                                                                                 \
      static std::vector<eCubeMove> solve = GenerateMoves(moves)();                  \
      static tOLLPattern pattern(std::bitset<9>(topFace),                            \
                                 std::bitset<3>(frontFace),                          \
                                 std::bitset<3>(rightFace),                          \
                                 std::bitset<3>(backFace),                           \
                                 std::bitset<3>(leftFace),                           \
                                 solve);                                             \
      return pattern;                                                                \
   }

namespace cube
{
   constexpr eCubeColor BottomColor = Cube::DefaultColorOfFace(eCubeFace::Bottom);

   static auto GenerateMoves(const std::string& moveNotation)
   {
      return [moveNotation]()
      {
         std::vector<eCubeMove> moves;
         Cube::ParseMoveNotation(moveNotation, moves);
         return moves;
      };
   }

   class FirstTwoLayersAlgorithms
   {
   public:
      // To use, position the inverted edge at the right side of the right face.
      CUBE_ALG_DEF(SolveInvertedEdge, "Uw R' Uw'");

      // F2l
      // https://www.cubeskills.com/uploads/pdf/tutorials/f2l.pdf
      // Basic inserts
      CUBE_ALG_DEF(SolveF2lBasicInsertRightPair, "U (R U' R')");
      CUBE_ALG_DEF(SolveF2lBasicInsertFrontPair, "y' U' (R' U R)");
      CUBE_ALG_DEF(SolveF2lBasicInsertSoloLeftEdge, "y' (R' U' R)");
      CUBE_ALG_DEF(SolveF2lBasicInsertSoloTopEdge, "(R U R')");

      // F2l Case1_x
      CUBE_ALG_DEF(SolveF2lCase1_1, "U' (R U' R' U) y' (R' U' R)");
      CUBE_ALG_DEF(SolveF2lCase1_2, "U' (R U R' U) (R U R')");
      CUBE_ALG_DEF(SolveF2lCase1_3, "U' (R U2 R') d (R' U' R)");
      CUBE_ALG_DEF(SolveF2lCase1_4, "R' U2 R2 U R2 U R");
      CUBE_ALG_DEF(SolveF2lCase1_5, "y' U (R' U R U') (R' U' R)");
      CUBE_ALG_DEF(SolveF2lCase1_6, "U' (R U' R' U) (R U R')");

      // F2l Case2_x
      CUBE_ALG_DEF(SolveF2lCase2_1, "(U' R U R') U2 (R U' R')");
      CUBE_ALG_DEF(SolveF2lCase2_2, "d (R' U' R) U2 (R' U R)");
      CUBE_ALG_DEF(SolveF2lCase2_3, "U' (R U2 R') U2 (R U' R')");
      CUBE_ALG_DEF(SolveF2lCase2_4, "d (R' U2 R) U2 (R' U R)");

      // F2l Case3_x
      CUBE_ALG_DEF(SolveF2lCase3_1, "U (R U2 R') U (R U' R')");
      CUBE_ALG_DEF(SolveF2lCase3_2, "y' U' (R' U2 R) U' (R' U R)");
      CUBE_ALG_DEF(SolveF2lCase3_3, "(R U' R') U2 (R U R')");
      CUBE_ALG_DEF(SolveF2lCase3_4, "y' U2 (R' U' R) U' (R' U R)");

      // F2l incorrectly connected pieces
      CUBE_ALG_DEF(SolveF2lCaseIncorrectlyConnectedPieces1, "y' (R' U R) U2 y (R U R')");
      CUBE_ALG_DEF(SolveF2lCaseIncorrectlyConnectedPieces2, "U F (R U R' U') F' (U R U' R')");
      CUBE_ALG_DEF(SolveF2lCaseIncorrectlyConnectedPieces3, "(R U2 R') U' (R U R')");
      CUBE_ALG_DEF(SolveF2lCaseIncorrectlyConnectedPieces4, "y' (R' U2 R) U (R' U' R)");
      CUBE_ALG_DEF(SolveF2lCaseIncorrectlyConnectedPieces5, "U (R U' R' U') (R U' R' U) (R U' R')");
      CUBE_ALG_DEF(SolveF2lCaseIncorrectlyConnectedPieces6, "y' U' (R' U R U) (R' U R U') (R' U R)");

      // F2l Corner in place edge in u face
      CUBE_ALG_DEF(SolveF2lCaseCornerInPlaceEdgeInU1, "R' F' R U (R U' R') F");
      CUBE_ALG_DEF(SolveF2lCaseCornerInPlaceEdgeInU2, "U (R U' R') U' (F' U F)");
      CUBE_ALG_DEF(SolveF2lCaseCornerInPlaceEdgeInU3, "(R U' R' U) (R U' R')");
      CUBE_ALG_DEF(SolveF2lCaseCornerInPlaceEdgeInU4, "y' (R' U R U') (R' U R)");
      CUBE_ALG_DEF(SolveF2lCaseCornerInPlaceEdgeInU5, "y' (R' U' R U) (R' U' R)");
      CUBE_ALG_DEF(SolveF2lCaseCornerInPlaceEdgeInU6, "(R U R' U') (R U R')");

      // F2l Edge in place, corner in U face
      CUBE_ALG_DEF(SolveF2lCaseEdgeInPlaceCornerInU1, "(R U' R' U) y' (R' U R)");
      CUBE_ALG_DEF(SolveF2lCaseEdgeInPlaceCornerInU2, "(U R U' R') (U R U' R') (U R U' R')");
      CUBE_ALG_DEF(SolveF2lCaseEdgeInPlaceCornerInU3, "(U' R U' R') U2 (R U' R')");
      CUBE_ALG_DEF(SolveF2lCaseEdgeInPlaceCornerInU4, "U (R U R') U2 (R U R')");
      CUBE_ALG_DEF(SolveF2lCaseEdgeInPlaceCornerInU5, "(U' R U R') U y' (R' U' R)");
      CUBE_ALG_DEF(SolveF2lCaseEdgeInPlaceCornerInU6, "U (F' U' F) U' (R U R')");

      // F2l edge and corner in place
      CUBE_ALG_DEF(SolveF2lCaseEdgeAndCornerInPlace1, ""); // Solved case (unused)
      CUBE_ALG_DEF(SolveF2lCaseEdgeAndCornerInPlace2, "(R U' R') d (R' U2 R) U2 (R' U R)");
      CUBE_ALG_DEF(SolveF2lCaseEdgeAndCornerInPlace3, "(R U' R' U') R U R' U2 (R U' R')");
      CUBE_ALG_DEF(SolveF2lCaseEdgeAndCornerInPlace4, "(R U' R' U) (R U2 R') U (R U' R')");
      CUBE_ALG_DEF(SolveF2lCaseEdgeAndCornerInPlace5, "(F' U F) U2 (R U R' U) (R U' R')");
      CUBE_ALG_DEF(SolveF2lCaseEdgeAndCornerInPlace6, "(R U R' U') (R U' R') U2 y' (R' U' R)");
   };

   /**
    * @brief      Defines a pattern to match against an OLL case. Also stores the algorithm to solve
    * it if matched.
    */
   struct tOLLPattern
   {
      /**
       * @brief      Define your pattern as a bit set
       *
       * @param[in]  pattern  The pattern
       */
      constexpr tOLLPattern(std::bitset<9> top, std::bitset<3> front, 
         std::bitset<3> right, std::bitset<3> back, std::bitset<3> left,
         std::vector<eCubeMove>& moves)
         : mTop(top.to_string()), 
           mFront(front.to_string()), mRight(right.to_string()), mBack(back.to_string()), mLeft(left.to_string()),
           mMoves(moves)
      {
         // Validate, there should be a total of 9 ones.
         int count = 0;
         for (int i = 0; i < mTop.size(); i++)
         {
            count += mTop[i] == '1';
         }

         for (int i = 0; i < mFront.size(); i++)
         {
            count += mFront[i] == '1';
            count += mRight[i] == '1';
            count += mBack[i] == '1';
            count += mLeft[i] == '1';
         }

         assert(count == 9 && "Invalid OLL spec");
      }

      /**
       * @param      cube  The cube
       * @return      Returns true if the pattern matches what's on the cube.
       */
      bool CheckMatch(Cube& cube) const
      {
         eCubeColor topColor = cube.ColorOfFace(eCubeFace::Top);

         // Check top.
         for (int j = 0; j < 3; j++)
         {
            for (int i = 0; i < 3; i++)
            {
               if (!CheckSquareMatch(cube, i, j, eCubeFace::Top, topColor, mTop))
               {
                  return false;
               }
            }
         }

         // Check sides
         for (int i = 0; i < 3; i++)
         {
            if (!CheckSquareMatch(cube, i, 0, eCubeFace::Front, topColor, mFront) ||
                !CheckSquareMatch(cube, i, 0, eCubeFace::Right, topColor, mRight) ||
                !CheckSquareMatch(cube, i, 0, eCubeFace::Back, topColor, mBack) ||
                !CheckSquareMatch(cube, i, 0, eCubeFace::Left, topColor, mLeft))
            {
               return false;
            }
         }

         return true;
      }

      /**
       * @brief      Executes the OLL if the current orientation is an exact match with the OLL alg
       * @param      cube      The cube
       * @param      moveList  The move list
       * @return     True if perform alg if matches, False otherwise.
       */
      bool PerformAlgIfMatches(Cube& cube, CubeMoveList& moveList) const
      {
         // Check the OLL for a match at the current layer.
         if (CheckMatch(cube))
         {
            moveList.PushMoves(mMoves);
            return true;
         }

         return false;
      }

   private:
      bool CheckSquareMatch(Cube& cube, int x, int y, eCubeFace face, eCubeColor expectedColor, const std::string& pattern) const
      {
         if (pattern[CubeDimsToIdx(x, y)] == '1')
         {
            return cube.GetState(face, x, y) == expectedColor;
         }
         else 
         {
            return cube.GetState(face, x, y) != expectedColor;
         }
      }

      const std::string mTop;
      const std::string mFront;
      const std::string mRight;
      const std::string mBack;
      const std::string mLeft;
      const std::vector<eCubeMove> mMoves;
   };

   static tOLLPattern CreateOLLPattern(int top, int front, int right, int back, int left, const std::string& moves)
   {
      std::vector<eCubeMove> solve = GenerateMoves(moves)();
      return tOLLPattern(std::bitset<9>(top), 
                         std::bitset<3>(front), 
                         std::bitset<3>(right), 
                         std::bitset<3>(back), 
                         std::bitset<3>(left),
                         solve);
   }

   /**
    * @brief      Defines algorithms and state matching used for OLL.
    */
   class OLLUtils
   {
   public:
      /**
       * @brief      Searches through the list of valid OLL patterns and executes the correct one
       * to solve the oll. Rejects the current pending moves if it fails.
       *
       * @param      cube      The cube
       * @param      moveList  The move list
       *
       * @return     True if successful in solving the OLL.
       */
      static bool FindAndExecuteCorrectOLL(Cube& cube, CubeMoveList& moveList)
      {
         static std::vector<tOLLPattern> allOLLs = []()
         {
            std::vector<tOLLPattern> results 
            {
               // Awkward shape
               CreateOLLPattern(0b011110001, 0b110, 0b010, 0b001, 0b000, "R U R' U' R U' R' F' U' F R U R'"), // OLL29
               CreateOLLPattern(0b010110101, 0b010, 0b011, 0b000, 0b100, "F R' F R2 U' R' U' R U R' F2"),     // OLL30
               CreateOLLPattern(0b010110101, 0b010, 0b010, 0b101, 0b000, "R U R' U R U2 R' F R U R' U' F'"),  // OLL41
               CreateOLLPattern(0b101110010, 0b101, 0b010, 0b010, 0b000, "R' U' R U' R' U2 R F R U R' U' F'"), // OLL42

               // Big lightning bolt
               CreateOLLPattern(0b001111100, 0b010, 0b100, 0b011, 0b000,  "L F' L' U' L U F U' L'"), // OLL39
               CreateOLLPattern(0b100111001, 0b010, 0b000, 0b110, 0b001,  "R' F R U R' U' F' U R"), // OLL40

               // C shape
               CreateOLLPattern(0b000111101, 0b010, 0b001, 0b010, 0b100, "R U R2 U' R' F R U R U' F'"), // OLL34
               CreateOLLPattern(0b110010110, 0b000, 0b111, 0b000, 0b010, "R' U' R' F R F' U R"), // OLL46

               // Corners oriented
               CreateOLLPattern(0b111110101, 0b010, 0b010, 0b000, 0b000, "r U R' U' r' R U R U' R'"), // OLL28
               CreateOLLPattern(0b101111101, 0b010, 0b000, 0b010, 0b000, "R U R' U' M' U R U' r'"), // OLL57

               // Cross
               CreateOLLPattern(0b010111010, 0b101, 0b000, 0b101, 0b000, "R U2 R' U' R U R' U' R U' R'"), // OLL21
               CreateOLLPattern(0b010111010, 0b001, 0b000, 0b100, 0b101, "R U2 R2 U' R2 U' R2 U2 R"), // OLL22
               CreateOLLPattern(0b010111111, 0b000, 0b000, 0b101, 0b000, "R2 D' R U2 R' D R U2 R"), // OLL23
               CreateOLLPattern(0b011111011, 0b100, 0b000, 0b001, 0b000, "r U R' U' r' F R F'"), // OLL24
               CreateOLLPattern(0b011111110, 0b001, 0b000, 0b000, 0b100, "F' r U R' U' r' F R"), // OLL25
               CreateOLLPattern(0b011111010, 0b100, 0b100, 0b000, 0b100, "R U2 R' U' R U' R'"), // OLL26
               CreateOLLPattern(0b010111110, 0b001, 0b001, 0b001, 0b000, "R U R' U R U2 R'"), // OLL27

               // Dot
               CreateOLLPattern(0b000010000, 0b010, 0b111, 0b010, 0b111, "R U2 R2 F R F' U2 R' F R F'"), // OLL01
               CreateOLLPattern(0b000010000, 0b010, 0b110, 0b111, 0b011, "r U r' U2 r U2 R' U2 R U' r'"), // OLL02
               CreateOLLPattern(0b000010100, 0b011, 0b011, 0b011, 0b010, "r' R2 U R' U r U2 r' U M'"), // OLL03
               CreateOLLPattern(0b000010001, 0b110, 0b010, 0b110, 0b110, "M U' r U2 r' U' R U' R' M'"), // OLL04
               CreateOLLPattern(0b100010001, 0b110, 0b011, 0b010, 0b010, "F R' F' R2 r' U R U' R' U' M'"), // OLL17
               CreateOLLPattern(0b101010000, 0b111, 0b010, 0b010, 0b010, "r U R' U R U2 r2 U' R U' R' U2 r"), // OLL18
               CreateOLLPattern(0b101010000, 0b010, 0b110, 0b010, 0b011, "r' R U R U R' U' M' R' F R F'"), // OLL19
               CreateOLLPattern(0b101010101, 0b010, 0b010, 0b010, 0b010, "r U R' U' M2 U R U' R' U' M'"), // OLL20

               // Fish shape
               CreateOLLPattern(0b010110001, 0b110, 0b010, 0b100, 0b100, "R U R' U' R' F R2 U R' U' F'"), // OLL09
               CreateOLLPattern(0b001110010, 0b001, 0b010, 0b011, 0b001, "R U R' U R' F R F' R U2 R'"), // OLL10
               CreateOLLPattern(0b100011011, 0b100, 0b001, 0b010, 0b010, "R U2 R2 F R F' R U2 R'"), // OLL35
               CreateOLLPattern(0b110110001, 0b110, 0b011, 0b000, 0b000, "F R' F' R U R U' R'"), // OLL37

               // I shape
               CreateOLLPattern(0b000111000, 0b110, 0b101, 0b011, 0b000, "F U R U' R' U R U' R' F'"), // OLL51
               CreateOLLPattern(0b010010010, 0b100, 0b111, 0b001, 0b010, "R U R' U R U' B U' B' R'"), // OLL52
               CreateOLLPattern(0b000111000, 0b111, 0b000, 0b111, 0b000, "R' F R U R U' R2 F' R2 U' R' U R U R'"), // OLL55
               CreateOLLPattern(0b000111000, 0b010, 0b101, 0b010, 0b101, "r' U' r U' R' U R U' R' U R r' U r"), // OLL56

               // Knight move shape
               CreateOLLPattern(0b000111100, 0b011, 0b001, 0b011, 0b000, "F U R U' R2 F' R U R U' R'"), // OLL13
               CreateOLLPattern(0b000111001, 0b110, 0b000, 0b110, 0b100, "R' F R U R' F' R F U' F'"), // OLL14
               CreateOLLPattern(0b100111000, 0b011, 0b001, 0b010, 0b001, "l' U' l L' U' L U l' U l"), // OLL15
               CreateOLLPattern(0b001111000, 0b110, 0b100, 0b010, 0b100, "r U r' R U R' U' r U' r'"), // OLL16

               // P-Shape
               CreateOLLPattern(0b011011001, 0b110, 0b000, 0b001, 0b010, "R' U' F U R U' R' F' R"), // OLL31
               CreateOLLPattern(0b110110100, 0b011, 0b010, 0b100, 0b000, "L U F' U' L' U L F L'"), // OLL32
               CreateOLLPattern(0b011011001, 0b010, 0b000, 0b000, 0b111, "F' U' L' U L F"), // OLL43
               CreateOLLPattern(0b110110100, 0b010, 0b111, 0b000, 0b000, "F U R U' R' F'"), // OLL44

               // Small L shape
               CreateOLLPattern(0b010011000, 0b110, 0b101, 0b001, 0b010, "R' U' R' F R F' R' F R F' U R"), // OLL47
               CreateOLLPattern(0b010110000, 0b011, 0b010, 0b100, 0b101, "F R U R' U' R U R' U' F'"), // OLL48
               CreateOLLPattern(0b010011000, 0b011, 0b000, 0b100, 0b111, "r U' r2 U r2 U r2 U' r"), // OLL49
               CreateOLLPattern(0b000011010, 0b001, 0b000, 0b110, 0b111, "r' U r2 U' r2 U' r2 U r'"), // OLL50
               CreateOLLPattern(0b010011000, 0b111, 0b000, 0b101, 0b010, "l' U2 L U L' U' L U L' U l"), // OLL53
               CreateOLLPattern(0b010110000, 0b111, 0b010, 0b101, 0b000, "r U2 R' U' R U R' U' R U' r'"), // OLL54

               // Small lightning bolt
               CreateOLLPattern(0b010110100, 0b011, 0b011, 0b001, 0b000, "r U R' U R U2 r'"), // OLL07
               CreateOLLPattern(0b010011001, 0b110, 0b000, 0b100, 0b110, "l' U' L U' L' U2 l"), // OLL08
               CreateOLLPattern(0b011110000, 0b011, 0b010, 0b001, 0b001, "r U R' U R' F R F' R U2 r'"), // OLL11
               CreateOLLPattern(0b110011000, 0b110, 0b100, 0b100, 0b010, "M' R' U' R U' R' U2 R U' R r'"), // OLL12

               // Square shape
               CreateOLLPattern(0b110110000, 0b011, 0b011, 0b000, 0b001, "l' U2 L U L' U l"), // OLL05
               CreateOLLPattern(0b011011000, 0b110, 0b100, 0b000, 0b110, "r U2 R' U' R U' r'"), // OLL06

               // T-Shape
               CreateOLLPattern(0b001111001, 0b110, 0b000, 0b011, 0b000, "R U R' U' R' F R F'"), // OLL33
               CreateOLLPattern(0b001111001, 0b010, 0b000, 0b010, 0b101, "F R U R' U' F'"), // OLL45

               // W-Shape
               CreateOLLPattern(0b110011001, 0b010, 0b000, 0b100, 0b011, "L' U' L U' L' U L U L F' L' F"), // OLL36
               CreateOLLPattern(0b011110100, 0b010, 0b110, 0b001, 0b000, "R U R' U R U' R' U' R' F R F'"), // OLL38
            };

            return results;
         }();

         // Check current orientation
         for (int i = 0; i < allOLLs.size(); i++)
         {
            if (allOLLs[i].PerformAlgIfMatches(cube, moveList))
            {
               return true;
            }
         }

         // Check rotation once
         moveList.PushMove(eCubeMove::Up);
         for (int i = 0; i < allOLLs.size(); i++)
         {
            if (allOLLs[i].PerformAlgIfMatches(cube, moveList))
            {
               return true;
            }
         }
         moveList.RejectPendingMoves();

         // Check rotation backwards
         moveList.PushMove(eCubeMove::UpPrime);
         for (int i = 0; i < allOLLs.size(); i++)
         {
            if (allOLLs[i].PerformAlgIfMatches(cube, moveList))
            {
               return true;
            }
         }
         moveList.RejectPendingMoves();

         // Check rotation backwards
         moveList.PushMove(eCubeMove::Up2);
         for (int i = 0; i < allOLLs.size(); i++)
         {
            if (allOLLs[i].PerformAlgIfMatches(cube, moveList))
            {
               return true;
            }
         }
         moveList.RejectPendingMoves();

         assert(false && "Could not match OLL case");
         return false;
      }
   };

   struct tPLLPattern
   {
      /**
       * @brief      Define the pattern as what you expect to see in the cube at each side when it's solved
       * using the pll. 
       * L: Left Face
       * R: Right Face
       * F: Front Face
       * B: Back Face
       * 
       * The system will not use the colors on the actual cube, rather it defines the colors as it goes.
       * The first time it sees L, it will see what color is on the cube at the spot its expecting to see L
       * then define that as L. In the future, every time it sees L it will expect it to be that color.
       * When all colors match, the pll is a match.
       *
       * @param[in]  frontPattern  The front pattern
       */
      tPLLPattern(const std::string& frontPattern, const std::string& rightPattern, 
         const std::string& backPattern, const std::string& leftPattern, const std::vector<eCubeMove>& solve)
         : mFrontPattern(frontPattern), mRightPattern(rightPattern), mBackPattern(backPattern), mLeftPattern(leftPattern),
         mSolve(solve)
      {
      }

      /**
       * @param      cube  The cube
       * @return      Returns true if the pattern matches what's on the cube.
       */
      bool CheckMatch(Cube& cube) const
      {
         eCubeColor frontColor = eCubeColor::NumColors;
         eCubeColor rightColor = eCubeColor::NumColors;
         eCubeColor backColor = eCubeColor::NumColors;
         eCubeColor leftColor = eCubeColor::NumColors;

         // Check sides
         for (int i = 0; i < 3; i++)
         {
            if (!CheckSquareMatch(cube, i, 0, eCubeFace::Front, frontColor, rightColor, backColor, leftColor, mFrontPattern) ||
                !CheckSquareMatch(cube, i, 0, eCubeFace::Right, frontColor, rightColor, backColor, leftColor, mRightPattern) ||
                !CheckSquareMatch(cube, i, 0, eCubeFace::Back, frontColor, rightColor, backColor, leftColor, mBackPattern) ||
                !CheckSquareMatch(cube, i, 0, eCubeFace::Left, frontColor, rightColor, backColor, leftColor, mLeftPattern))
            {
               return false;
            }
         }

         return true;
      }

      /**
       * @brief      Executes the OLL if the current orientation is an exact match with the OLL alg
       * @param      cube      The cube
       * @param      moveList  The move list
       * @return     True if perform alg if matches, False otherwise.
       */
      bool PerformAlgIfMatches(Cube& cube, CubeMoveList& moveList) const
      {
         // Check the OLL for a match at the current layer.
         if (CheckMatch(cube))
         {
            moveList.PushMoves(mSolve);
            return true;
         }

         return false;
      }

   private:
      bool CheckSquareMatch(Cube& cube, int x, int y, eCubeFace face, 
         eCubeColor& frontColor, eCubeColor& rightColor, eCubeColor& backColor, eCubeColor& leftColor,
         const std::string& pattern) const
      {
         char patternValue = pattern[CubeDimsToIdx(x, y)];
         eCubeColor* expectedColor = nullptr;

         switch (patternValue)
         {
         case 'F':
            expectedColor = &frontColor;
            break;
         case 'R':
            expectedColor = &rightColor;
            break;
         case 'B':
            expectedColor = &backColor;
            break;
         case 'L':
            expectedColor = &leftColor;
            break;
         default:
            assert(false && "Invalid pattern spec");
            break;
         }

         // Check case where the expected color isn't set yet.
         if (*expectedColor == eCubeColor::NumColors)
         {
            *expectedColor = cube.GetState(face, x, y);

            // We always match in this case.
            return true;
         }

         return *expectedColor == cube.GetState(face, x, y);
      }

      std::string mFrontPattern;
      std::string mRightPattern;
      std::string mBackPattern;
      std::string mLeftPattern;
      std::vector<eCubeMove> mSolve;
   };

   static tPLLPattern CreatePLLPattern(const std::string& front, const std::string& right, 
      const std::string& back, const std::string& left, const std::string& moves)
   {
      std::vector<eCubeMove> solve = GenerateMoves(moves)();
      return tPLLPattern(front, right, back, left, solve);
   }

   class PLLUtils
   {
   public:
      static bool FindAndExecuteCorrectPLL(Cube& cube, CubeMoveList& moveList)
      {
         static std::vector<tPLLPattern> allPLLs = []()
         {
            std::vector<tPLLPattern> results 
            {
               CreatePLLPattern("LFF", "RRL", "FBR", "BLB", "x L2 D2 L' U' L D2 L' U L'"), // PLL_Aa
               CreatePLLPattern("RFB", "LRR", "BBL", "FLF", "x' L2 D2 L U L' D2 L U' L"), // PLL_Ab
               CreatePLLPattern("FBR", "BRF", "RFB", "LLL", "R' U' F' R U R' U' R' F R2 U' R' U' R U R' U R"), // PLL_F
               CreatePLLPattern("FRR", "BLF", "RFB", "LBL", "R2 U R' U R' U' R U' R2 U' D R' U R D'"), // PLL_Ga
               CreatePLLPattern("FBR", "BFF", "RLB", "LRL", "R' U' R U D' R2 U R' U R U' R U' R2 D"), // PLL_Gb
               CreatePLLPattern("FBR", "BLF", "RRB", "LFL", "R2 U' R U' R U R' U R2 U D' R U' R' D"), // PLL_Gc
               CreatePLLPattern("FLR", "BBF", "RFB", "LRL", "R U R' U' D R2 U' R U' R' U R' U R2 D'"), // PLL_Gd
               CreatePLLPattern("FFR", "BBF", "RRB", "LLL", "x R2 F R F' R U2 r' U r U2"), // PLL_Ja
               CreatePLLPattern("LFF", "RLL", "FRR", "BBB", "R U R' F' R U R' U' R' F R2 U' R'"), // PLL_Jb
               CreatePLLPattern("LLF", "RFL", "FBR", "BRB", "R U' R' U' R U R D R' U' R D' R' U2 R'"), // PLL_Ra
               CreatePLLPattern("RFB", "LBR", "BLL", "FRF", "R2 F R U R U' R' F' R U2 R' U2 R"), // PLL_Rb
               CreatePLLPattern("FFR", "BLF", "RBB", "LRL", "R U R' U' R' F R2 U' R' U' R U R' F'"), // PLL_T
               CreatePLLPattern("LFR", "BRF", "RBL", "FLB", "x' L' U L D' L' U' L D L' U' L D' L' U L D"), // PLL_E
               CreatePLLPattern("BFF", "RLL", "FBB", "LRR", "R U R' U R U R' F' R U R' U' R' F R2 U' R' U2 R U' R'"), // PLL_Na
               CreatePLLPattern("FFB", "LLR", "BBF", "RRL", "R' U R U' R' F' U' F R U R' F R' F' R U' R"), // PLL_Nb
               CreatePLLPattern("FFB", "LBR", "BRF", "RLL", "R' U R' U' y R' F' R2 U' R' U R' F R F"), // PLL_V
               CreatePLLPattern("FFB", "LRR", "BLF", "RBL", "F R U' R' U' R U R' F' R U R' U' R' F R F'"), // PLL_Y
               CreatePLLPattern("FBF", "RLR", "BFB", "LRL", "M2 U M2 U2 M2 U M2"), // PLL_H
               CreatePLLPattern("FRF", "RLR", "BBB", "LFL", "M2 U M U2 M' U M2"), // PLL_Ua
               CreatePLLPattern("FLF", "RFR", "BBB", "LRL", "M2 U' M U2 M' U' M2"), // PLL_Ub
               CreatePLLPattern("LBL", "FRF", "RFR", "BLB", "M' U M2 U M2 U M' U2 M2"), // PLL_Z
            };

            return results;
         }();

         // Check current orientation
         for (int i = 0; i < allPLLs.size(); i++)
         {
            if (allPLLs[i].PerformAlgIfMatches(cube, moveList))
            {
               return true;
            }
         }

         // Check rotation once
         moveList.PushMove(eCubeMove::Up);
         for (int i = 0; i < allPLLs.size(); i++)
         {
            if (allPLLs[i].PerformAlgIfMatches(cube, moveList))
            {
               return true;
            }
         }
         moveList.RejectPendingMoves();

         // Check rotation backwards
         moveList.PushMove(eCubeMove::UpPrime);
         for (int i = 0; i < allPLLs.size(); i++)
         {
            if (allPLLs[i].PerformAlgIfMatches(cube, moveList))
            {
               return true;
            }
         }
         moveList.RejectPendingMoves();

         // Check rotation backwards
         moveList.PushMove(eCubeMove::Up2);
         for (int i = 0; i < allPLLs.size(); i++)
         {
            if (allPLLs[i].PerformAlgIfMatches(cube, moveList))
            {
               return true;
            }
         }
         moveList.RejectPendingMoves();

         assert(false && "Could not match PLL case");
         return false;
      }
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

   eCubeColor CubeSolveUtils::GetEdgeColor(Cube& cube, eCubeFace face, eFaceEdgePos edgePos)
   {
      int x, y;
      GetEdgePosition(edgePos, x, y);
      return cube.GetState(face, x, y);
   }

   eCubeColor CubeSolveUtils::GetAdjacentEdgeColor(Cube& cube, eCubeFace face, eFaceEdgePos edgePos)
   {
      eCubeFace adjacentFace;
      eFaceEdgePos adjacentEdgePos;
      GetAdjacentEdge(face, edgePos, adjacentFace, adjacentEdgePos);
      return GetEdgeColor(cube, adjacentFace, adjacentEdgePos);
   }

   eCubeColor CubeSolveUtils::GetCornerColor(Cube& cube, eCubeFace face, eFaceCornerPos corner)
   {
      int x, y;
      GetCornerPosition(corner, x, y);
      return cube.GetState(face, x, y);
   }

   void CubeSolveUtils::GetCornerColors(Cube& cube, const tCornerDescriptor& cornerDescriptor, 
      eCubeColor& yColor, eCubeColor& xColor, eCubeColor& zColor)
   {
      yColor = GetCornerColor(cube, cornerDescriptor.FaceY, cornerDescriptor.CornerY);
      xColor = GetCornerColor(cube, cornerDescriptor.FaceX, cornerDescriptor.CornerX);
      zColor = GetCornerColor(cube, cornerDescriptor.FaceZ, cornerDescriptor.CornerZ);
   }

   void CubeSolveUtils::GetCornerColors(Cube& cube, eCubeFace face, eFaceCornerPos corner,
      eCubeColor& yColor, eCubeColor& xColor, eCubeColor& zColor)
   {
      tCornerDescriptor cornerDescriptor = GetCornerDescriptor(face, corner);
      GetCornerColors(cube, cornerDescriptor, yColor, xColor, zColor);
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
   static void RotateSideFaceToFront(Cube& cube, eCubeFace face, CubeMoveList& moveList)
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

         moveList.PushMove(orientingMove);
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
   static void RotateSideFaceToRight(Cube& cube, eCubeFace face, CubeMoveList& moveList)
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

         moveList.PushMove(orientingMove);
      }
   }

   static void RotateColorToBottom(Cube& cube, CubeMoveList& moveList, eCubeColor bottomColor)
   {
      if (cube.ColorOfFace(eCubeFace::Bottom) != bottomColor)
      {
         eCubeFace bottomColorFace = cube.FaceOfColor(bottomColor);
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
   }

   static bool OrientCube(Cube& cube, std::ostream& outputStream, bool useSeparators)
   {
      // We want white on the bottom.
      CubeMoveList moveList(cube);

      RotateColorToBottom(cube, moveList, BottomColor);

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
                  moveList.PushMoves(FirstTwoLayersAlgorithms::SolveInvertedEdge());
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
                  moveList.PushMoves(FirstTwoLayersAlgorithms::SolveInvertedEdge());
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
                  moveList.PushMoves(FirstTwoLayersAlgorithms::SolveInvertedEdge());
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
                  moveList.PushMoves(FirstTwoLayersAlgorithms::SolveInvertedEdge());
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
      RotateSideFaceToRight(cube, face, moveList);

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

   /**
    * @return      Returns the face corner relative to sideFace1 such that the corner is adjacent to 
    */
   static eFaceCornerPos GetF2lPairCorner(Cube& cube, eCubeFace sideFace1, eCubeFace sideFace2)
   {
      assert(sideFace1 != eCubeFace::Top && sideFace1 != eCubeFace::Bottom);
      assert(sideFace2 != eCubeFace::Top && sideFace2 != eCubeFace::Bottom);
      assert(sideFace1 != sideFace2);
      assert(GetOppositeFace(sideFace1) != sideFace2);

      if (sideFace1 == eCubeFace::Front)
      {
         if (sideFace2 == eCubeFace::Left)
         {
            return eFaceCornerPos::BottomLeft;
         }
         else if(sideFace2 == eCubeFace::Right)
         {
            return eFaceCornerPos::BottomRight;
         }
      }
      else if (sideFace1 == eCubeFace::Back)
      {
         if (sideFace2 == eCubeFace::Left)
         {
            return eFaceCornerPos::BottomRight;
         }
         else if(sideFace2 == eCubeFace::Right)
         {
            return eFaceCornerPos::BottomLeft;
         }
      }
      else if (sideFace1 == eCubeFace::Left)
      {
         if (sideFace2 == eCubeFace::Front)
         {
            return eFaceCornerPos::BottomRight;
         }
         else if(sideFace2 == eCubeFace::Back)
         {
            return eFaceCornerPos::BottomLeft;
         }
      }
      else if (sideFace1 == eCubeFace::Right)
      {
         if (sideFace2 == eCubeFace::Front)
         {
            return eFaceCornerPos::BottomLeft;
         }
         else if(sideFace2 == eCubeFace::Back)
         {
            return eFaceCornerPos::BottomRight;
         }
      }

      // Invalid case.
      assert(false);
      return eFaceCornerPos::TopLeft;
   }

   static eFaceEdgePos GetF2lEdgePair(Cube& cube, eCubeFace sideFace1, eCubeFace sideFace2)
   {
      assert(sideFace1 != eCubeFace::Top && sideFace1 != eCubeFace::Bottom);
      assert(sideFace2 != eCubeFace::Top && sideFace2 != eCubeFace::Bottom);
      assert(sideFace1 != sideFace2);
      assert(GetOppositeFace(sideFace1) != sideFace2);

      if (sideFace1 == eCubeFace::Front)
      {
         if (sideFace2 == eCubeFace::Left)
         {
            return eFaceEdgePos::LeftEdge;
         }
         else if(sideFace2 == eCubeFace::Right)
         {
            return eFaceEdgePos::RightEdge;
         }
      }
      else if (sideFace1 == eCubeFace::Back)
      {
         if (sideFace2 == eCubeFace::Left)
         {
            return eFaceEdgePos::RightEdge;
         }
         else if(sideFace2 == eCubeFace::Right)
         {
            return eFaceEdgePos::LeftEdge;
         }
      }
      else if (sideFace1 == eCubeFace::Left)
      {
         if (sideFace2 == eCubeFace::Front)
         {
            return eFaceEdgePos::RightEdge;
         }
         else if(sideFace2 == eCubeFace::Back)
         {
            return eFaceEdgePos::LeftEdge;
         }
      }
      else if (sideFace1 == eCubeFace::Right)
      {
         if (sideFace2 == eCubeFace::Front)
         {
            return eFaceEdgePos::LeftEdge;
         }
         else if(sideFace2 == eCubeFace::Back)
         {
            return eFaceEdgePos::RightEdge;
         }
      }

      // Invalid case.
      assert(false);
      return eFaceEdgePos::TopEdge;
   }

   static bool IsF2lPairSolved(Cube& cube, eCubeFace sideFace1, eCubeFace sideFace2)
   {
      eCubeAxis sideFace1Axis = CubeSolveUtils::GetAxisForFace(sideFace1);
      eCubeAxis sideFace2Axis = CubeSolveUtils::GetAxisForFace(sideFace2);

      assert(sideFace1Axis == eCubeAxis::XAxis || sideFace1Axis == eCubeAxis::ZAxis);
      assert(sideFace2Axis == eCubeAxis::XAxis || sideFace2Axis == eCubeAxis::ZAxis);
      assert(sideFace1Axis != sideFace2Axis);

      eCubeColor face1Color = cube.ColorOfFace(sideFace1);
      eCubeColor face2Color = cube.ColorOfFace(sideFace2);
      eFaceCornerPos f2lCorner = GetF2lPairCorner(cube, sideFace1, sideFace2);
      eFaceEdgePos f2lEdge = GetF2lEdgePair(cube, sideFace1, sideFace2);

      bool isEdgeInverted;
      bool isEdgeInPosition = CubeSolveUtils::IsEdgeInPosition(cube, sideFace1, f2lEdge, face1Color, face2Color, isEdgeInverted);

      // Check to see if both the edge and corner are in the correct positions.
      if (!isEdgeInverted && isEdgeInPosition)
      {
         eCubeColor yColor, xColor, zColor;
         CubeSolveUtils::GetCornerColors(cube, sideFace1, f2lCorner, yColor, xColor, zColor);

         if (yColor == BottomColor)
         {
            if (sideFace1Axis == eCubeAxis::XAxis)
            {
               return xColor == face1Color && zColor == face2Color;
            }
            else if (sideFace1Axis == eCubeAxis::ZAxis)
            {
               return zColor == face1Color && xColor == face2Color;
            }
         }
      }

      return false;
   }

   /**
    * @brief      Verifies as a quick self test that all f2l pairs have actaully been solved.
    * Asserts false if it's hasn't.
    */
   static void EnsureF2lSolved(Cube& cube)
   {
      // Check if the cross has been solved as well since that's a prereq for f2l.
      EnsureCrossSolved(cube);

      assert(IsF2lPairSolved(cube, eCubeFace::Front, eCubeFace::Left));
      assert(IsF2lPairSolved(cube, eCubeFace::Front, eCubeFace::Right));
      assert(IsF2lPairSolved(cube, eCubeFace::Back, eCubeFace::Left));
      assert(IsF2lPairSolved(cube, eCubeFace::Back, eCubeFace::Right));
   }

   /**
    * @brief      Finds the F2l corner (assuming the corner we want to solve is positioned in the right)
    * and places it either in the top right if it needs to move it. Otherwise, leaves it in the bottom right.
    * This function does not mess with the orientation of the corner. Nor does it accept the move list.
    */
   static void PositionF2lCornerInRightTopOrBottom(Cube& cube, CubeMoveList& moveList)
   {
      eCubeColor color1 = cube.ColorOfFace(eCubeFace::Front);
      eCubeColor color2 = cube.ColorOfFace(eCubeFace::Right);
      eCubeColor color3 = BottomColor;

      // Since we want the corner to be on the right, we only have to check the left two corner positions 
      // for the front face.
      if (CubeSolveUtils::IsCornerInPosition(cube, eCubeFace::Front, eFaceCornerPos::TopLeft, color1, color2, color3))
      {
         moveList.PushMove(eCubeMove::UpPrime);
      }
      else if (CubeSolveUtils::IsCornerInPosition(cube, eCubeFace::Front, eFaceCornerPos::BottomLeft, color1, color2, color3))
      {
         moveList.PushMove(eCubeMove::LeftPrime);
         moveList.PushMove(eCubeMove::UpPrime);
         moveList.PushMove(eCubeMove::Left);
      }
      // Check all four back corners.
      else if (CubeSolveUtils::IsCornerInPosition(cube, eCubeFace::Back, eFaceCornerPos::TopLeft, color1, color2, color3))
      {
         moveList.PushMove(eCubeMove::Up);
      }
      else if (CubeSolveUtils::IsCornerInPosition(cube, eCubeFace::Back, eFaceCornerPos::TopRight, color1, color2, color3))
      {
         moveList.PushMove(eCubeMove::Up2);
      }
      else if (CubeSolveUtils::IsCornerInPosition(cube, eCubeFace::Back, eFaceCornerPos::BottomLeft, color1, color2, color3))
      {
         moveList.PushMove(eCubeMove::Y);
         moveList.PushMove(eCubeMove::Right);
         moveList.PushMove(eCubeMove::Up);
         moveList.PushMove(eCubeMove::RightPrime);
         moveList.PushMove(eCubeMove::YPrime);
      }
      else if (CubeSolveUtils::IsCornerInPosition(cube, eCubeFace::Back, eFaceCornerPos::BottomRight, color1, color2, color3))
      {
         moveList.PushMove(eCubeMove::YPrime);
         moveList.PushMove(eCubeMove::LeftPrime);
         moveList.PushMove(eCubeMove::Up2);
         moveList.PushMove(eCubeMove::Left);
         moveList.PushMove(eCubeMove::Y);
      }

      // Ensure the right corner is correctly positioned on the right.
      assert(CubeSolveUtils::IsCornerInPosition(cube, eCubeFace::Front, eFaceCornerPos::TopRight, color1, color2, color3) ||
             CubeSolveUtils::IsCornerInPosition(cube, eCubeFace::Front, eFaceCornerPos::BottomRight, color1, color2, color3));
   }

   /**
    * @brief      Position the f2l edge at the top of the cube in preparation for solving the f2l alg.
    * Does not accept the move list yet.
    */
   static void PositionF2lEdgeAtTopOrMiddleRight(Cube& cube, CubeMoveList& moveList)
   {
      // The edge can be positioned anywhere on the top, but that's not the case were interested in.
      // The only edges we have to correct are the middle edges in the front-left, back-left, and back-right.
      eCubeColor color1 = cube.ColorOfFace(eCubeFace::Front);
      eCubeColor color2 = cube.ColorOfFace(eCubeFace::Right);

      // Store whether the corner is on top or below. This may change the method of removal we choose.
      // This difference is mainly for solve efficiency.
      bool isCornerInBottomRight = CubeSolveUtils::IsCornerInPosition(cube, eCubeFace::Front, 
         eFaceCornerPos::BottomRight, color1, color2, BottomColor);

      bool isInverted;
      if (CubeSolveUtils::IsEdgeInPosition(cube, eCubeFace::Front, eFaceEdgePos::LeftEdge, color1, color2, isInverted))
      {
         if (isCornerInBottomRight)
         {
            moveList.PushMove(eCubeMove::LeftPrime);
            moveList.PushMove(eCubeMove::UpPrime);
            moveList.PushMove(eCubeMove::Left);
         }
         else 
         {
            moveList.PushMove(eCubeMove::YPrime);
            moveList.PushMove(eCubeMove::Right);
            moveList.PushMove(eCubeMove::UpPrime);
            moveList.PushMove(eCubeMove::RightPrime);
            moveList.PushMove(eCubeMove::Y);
         }
      }
      else if (CubeSolveUtils::IsEdgeInPosition(cube, eCubeFace::Back, eFaceEdgePos::LeftEdge, color1, color2, isInverted))
      {
         if (isCornerInBottomRight)
         {
            // Position the edge in the front-top
            moveList.PushMove(eCubeMove::RightPrime);
            moveList.PushMove(eCubeMove::Up);
            moveList.PushMove(eCubeMove::Right);
         }
         else 
         {
            // Position the edge in the back-top
            moveList.PushMove(eCubeMove::RightPrime);
            moveList.PushMove(eCubeMove::UpPrime);
            moveList.PushMove(eCubeMove::Right);
         }
      }
      else if (CubeSolveUtils::IsEdgeInPosition(cube, eCubeFace::Back, eFaceEdgePos::RightEdge, color1, color2, isInverted))
      {
         if (isCornerInBottomRight)
         {
            moveList.PushMove(eCubeMove::Left);
            moveList.PushMove(eCubeMove::UpPrime);
            moveList.PushMove(eCubeMove::LeftPrime);
         }
         else
         {
            moveList.PushMove(eCubeMove::Left);
            moveList.PushMove(eCubeMove::UpPrime);
            moveList.PushMove(eCubeMove::LeftPrime);
            moveList.PushMove(eCubeMove::Up);
         }
      }

      // If the corner is at the bottom, position the edge over it's respective side.
      if (isCornerInBottomRight)
      {
         if (CubeSolveUtils::IsEdgeInPosition(cube, eCubeFace::Top, eFaceEdgePos::LeftEdge, color1, color2, isInverted))
         {
            if (isInverted)
            {
               moveList.PushMove(eCubeMove::UpPrime);
            }
            else 
            {
               moveList.PushMove(eCubeMove::Up2);
            }
         }
         else if (CubeSolveUtils::IsEdgeInPosition(cube, eCubeFace::Top, eFaceEdgePos::TopEdge, color1, color2, isInverted))
         {
            if (isInverted)
            {
               moveList.PushMove(eCubeMove::Up2);
            }
            else 
            {
               moveList.PushMove(eCubeMove::Up);
            }
         }
         else if (CubeSolveUtils::IsEdgeInPosition(cube, eCubeFace::Top, eFaceEdgePos::RightEdge, color1, color2, isInverted))
         {
            if (isInverted)
            {
               // Position the same color over the front face instead.
               moveList.PushMove(eCubeMove::Up);
            }
            else 
            {
               // Nothing to do, it's already positioned correctly.
            }
         }
         else if (CubeSolveUtils::IsEdgeInPosition(cube, eCubeFace::Top, eFaceEdgePos::BottomEdge, color1, color2, isInverted))
         {
            if (isInverted)
            {
               // Nothing to do, it's already positioned correctly.
            }
            else 
            {
               // Position the same color over the right face instead.
               moveList.PushMove(eCubeMove::UpPrime);
            }
         }
      }

      // Ensure the corner hasn't moved and that the edge is positioned at the top.
      assert(CubeSolveUtils::IsCornerInPosition(cube, eCubeFace::Front, eFaceCornerPos::TopRight, color1, color2, BottomColor) ||
             CubeSolveUtils::IsCornerInPosition(cube, eCubeFace::Front, eFaceCornerPos::BottomRight, color1, color2, BottomColor));

      assert(
         CubeSolveUtils::IsEdgeInPosition(cube, eCubeFace::Top, eFaceEdgePos::TopEdge, color1, color2, isInverted) ||
         CubeSolveUtils::IsEdgeInPosition(cube, eCubeFace::Top, eFaceEdgePos::LeftEdge, color1, color2, isInverted) ||
         CubeSolveUtils::IsEdgeInPosition(cube, eCubeFace::Top, eFaceEdgePos::RightEdge, color1, color2, isInverted) ||
         CubeSolveUtils::IsEdgeInPosition(cube, eCubeFace::Top, eFaceEdgePos::BottomEdge, color1, color2, isInverted) ||

             // It can also be in the front right and be valid.
         CubeSolveUtils::IsEdgeInPosition(cube, eCubeFace::Front, eFaceEdgePos::RightEdge, color1, color2, isInverted)
      );
   }

   struct tF2lCase
   {
      eCubeFace CornerFace;
      eFaceCornerPos CornerPos;
      eCubeColor CornerColor;

      eCubeFace EdgeFace;
      eFaceEdgePos EdgePos;
      eCubeColor EdgeColor;

      eCubeColor FrontColor;
      eCubeColor RightColor;
   };

   static bool SolveF2lBasicInserts(Cube& cube, tF2lCase& f2lCase, CubeMoveList& moveList)
   {
      if (f2lCase.CornerFace != eCubeFace::Front || f2lCase.CornerPos != eFaceCornerPos::TopRight || f2lCase.EdgeFace != eCubeFace::Top)
      {
         // Cannot basic insert if it's not at the top.
         return false;
      }

      // If the edge and corner are directly paired and oriented correctly.
      if (f2lCase.EdgePos == eFaceEdgePos::RightEdge && f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == BottomColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lBasicInsertRightPair());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::BottomEdge && f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == f2lCase.FrontColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lBasicInsertFrontPair());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::LeftEdge && f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == BottomColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lBasicInsertSoloLeftEdge());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::TopEdge && f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == f2lCase.FrontColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lBasicInsertSoloTopEdge());
         return true;
      }

      return false;
   }

   static bool SolveF2lCase1(Cube& cube, tF2lCase& f2lCase, CubeMoveList& moveList)

   {
      if (f2lCase.CornerFace != eCubeFace::Front || f2lCase.CornerPos != eFaceCornerPos::TopRight || f2lCase.EdgeFace != eCubeFace::Top)
      {
         // This case requires the corner to be at the top.
         return false;
      }

      // If the edge and corner are directly paired and oriented correctly.
      if (f2lCase.EdgePos == eFaceEdgePos::TopEdge && f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == BottomColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCase1_1());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::LeftEdge && f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == f2lCase.FrontColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCase1_2());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::RightEdge && f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == BottomColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCase1_3());
         return true;
      }

      // Inverted edge-corner pair
      if (f2lCase.EdgePos == eFaceEdgePos::BottomEdge && f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == f2lCase.FrontColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCase1_4());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::BottomEdge && f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == BottomColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCase1_5());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::RightEdge && f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == f2lCase.FrontColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCase1_6());
         return true;
      }

      return false;
   }

   static bool SolveF2lCase2(Cube& cube, tF2lCase& f2lCase, CubeMoveList& moveList)
   {
      if (f2lCase.CornerFace != eCubeFace::Front || f2lCase.CornerPos != eFaceCornerPos::TopRight || f2lCase.EdgeFace != eCubeFace::Top)
      {
         // This case requires the corner to be at the top.
         return false;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::TopEdge && f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == BottomColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCase2_1());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::LeftEdge && f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == f2lCase.FrontColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCase2_2());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::LeftEdge && f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == BottomColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCase2_3());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::TopEdge && f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == f2lCase.FrontColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCase2_4());
         return true;
      }

      return false;
   }

   static bool SolveF2lCase3(Cube& cube, tF2lCase& f2lCase, CubeMoveList& moveList)
   {
      if (f2lCase.CornerFace != eCubeFace::Front || f2lCase.CornerPos != eFaceCornerPos::TopRight || f2lCase.EdgeFace != eCubeFace::Top)
      {
         // This case requires the corner to be at the top.
         return false;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::TopEdge && f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == f2lCase.RightColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCase3_1());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::LeftEdge && f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == f2lCase.RightColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCase3_2());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::LeftEdge && f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == f2lCase.RightColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCase3_3());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::TopEdge && f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == f2lCase.RightColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCase3_4());
         return true;
      }

      return false;
   }

   static bool SolveF2lCaseIncorrectlyConnectedPieces(Cube& cube, tF2lCase& f2lCase, CubeMoveList& moveList)
   {
      if (f2lCase.CornerFace != eCubeFace::Front || f2lCase.CornerPos != eFaceCornerPos::TopRight || f2lCase.EdgeFace != eCubeFace::Top)
      {
         // This case requires the corner to be at the top.
         return false;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::BottomEdge && f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == BottomColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseIncorrectlyConnectedPieces1());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::RightEdge && f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == f2lCase.FrontColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseIncorrectlyConnectedPieces2());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::RightEdge && f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == f2lCase.RightColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseIncorrectlyConnectedPieces3());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::BottomEdge && f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == f2lCase.RightColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseIncorrectlyConnectedPieces4());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::BottomEdge && f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == f2lCase.RightColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseIncorrectlyConnectedPieces5());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::RightEdge && f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == f2lCase.RightColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseIncorrectlyConnectedPieces6());
         return true;
      }

      return false;
   }

   static bool SolveF2lCaseCornerInPlaceEdgeInUFace(Cube& cube, tF2lCase& f2lCase, CubeMoveList& moveList)
   {
      if (f2lCase.CornerFace != eCubeFace::Front || f2lCase.CornerPos != eFaceCornerPos::BottomRight || f2lCase.EdgeFace != eCubeFace::Top)
      {
         // This case requires the corner to be in the bottom and the edge to be somewhere in the top.
         return false;
      }

      // Since we know the corner is on the bottom, and the edge is on top, it is required that the edge is in the bottom or right position.
      assert(f2lCase.EdgePos == eFaceEdgePos::BottomEdge || f2lCase.EdgePos == eFaceEdgePos::RightEdge);

      if (f2lCase.EdgePos == eFaceEdgePos::RightEdge && f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == f2lCase.FrontColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseCornerInPlaceEdgeInU1());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::BottomEdge && f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == f2lCase.FrontColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseCornerInPlaceEdgeInU2());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::RightEdge && f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == BottomColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseCornerInPlaceEdgeInU3());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::BottomEdge && f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == f2lCase.RightColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseCornerInPlaceEdgeInU4());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::BottomEdge && f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == BottomColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseCornerInPlaceEdgeInU5());
         return true;
      }

      if (f2lCase.EdgePos == eFaceEdgePos::RightEdge && f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == f2lCase.RightColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseCornerInPlaceEdgeInU6());
         return true;
      }

      return false;
   }

   static bool SolveF2lCaseEdgeInPlaceCornerInUFace(Cube& cube, tF2lCase& f2lCase, CubeMoveList& moveList)
   {
      if (f2lCase.CornerFace != eCubeFace::Front || f2lCase.CornerPos != eFaceCornerPos::TopRight || 
         f2lCase.EdgeFace != eCubeFace::Front || f2lCase.EdgePos != eFaceEdgePos::RightEdge)
      {
         // This case requires the edge is positioned in the middle right with the corner just above it.
         return false;
      }

      if (f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == f2lCase.RightColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseEdgeInPlaceCornerInU1());
         return true;
      }

      if (f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == f2lCase.RightColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseEdgeInPlaceCornerInU2());
         return true;
      }

      if (f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == BottomColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseEdgeInPlaceCornerInU3());
         return true;
      }

      if (f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == f2lCase.FrontColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseEdgeInPlaceCornerInU4());
         return true;
      }

      if (f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == BottomColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseEdgeInPlaceCornerInU5());
         return true;
      }

      if (f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == f2lCase.FrontColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseEdgeInPlaceCornerInU5());
         return true;
      }

      return false;
   }

   static bool SolveF2lCaseEdgeAndCornerInPlace(Cube& cube, tF2lCase& f2lCase, CubeMoveList& moveList)
   {
      if (f2lCase.CornerFace != eCubeFace::Front || f2lCase.CornerPos != eFaceCornerPos::BottomRight || 
         f2lCase.EdgeFace != eCubeFace::Front || f2lCase.EdgePos != eFaceEdgePos::RightEdge)
      {
         // This case requires the edge is positioned in the middle right with the corner just above it.
         return false;
      }

      // Unused solved case.
      if (f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == f2lCase.FrontColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseEdgeInPlaceCornerInU1());
         assert(false && "F2l case should not be solved yet, but it is");
         return true;
      }

      if (f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == f2lCase.FrontColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseEdgeInPlaceCornerInU2());
         return true;
      }

      if (f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == BottomColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseEdgeInPlaceCornerInU3());
         return true;
      }

      if (f2lCase.EdgeColor == f2lCase.FrontColor && f2lCase.CornerColor == f2lCase.RightColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseEdgeInPlaceCornerInU4());
         return true;
      }

      if (f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == BottomColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseEdgeInPlaceCornerInU5());
         return true;
      }

      if (f2lCase.EdgeColor == f2lCase.RightColor && f2lCase.CornerColor == f2lCase.RightColor)
      {
         moveList.PushMoves(FirstTwoLayersAlgorithms::SolveF2lCaseEdgeInPlaceCornerInU6());
         return true;
      }

      return false;
   }

   /**
    * @brief      Solves the F2l case assuming the corner is positioned on the right front
    * and the edge is either in the front right, or top.
    *
    * @param      cube      The cube
    * @param      moveList  The move list
    */
   static void SolveF2lCase(Cube& cube, tF2lCase& f2lCase, CubeMoveList& moveList)
   {
      // Basic inserts
      if (SolveF2lBasicInserts(cube, f2lCase, moveList))
      {
         return;
      }

      if (SolveF2lCase1(cube, f2lCase, moveList))
      {
         return;
      }

      if (SolveF2lCase2(cube, f2lCase, moveList))
      {
         return;
      }

      if (SolveF2lCase3(cube, f2lCase, moveList))
      {
         return;
      }

      if (SolveF2lCaseIncorrectlyConnectedPieces(cube, f2lCase, moveList))
      {
         return;
      }

      if (SolveF2lCaseCornerInPlaceEdgeInUFace(cube, f2lCase, moveList))
      {
         return;
      }

      if (SolveF2lCaseEdgeInPlaceCornerInUFace(cube, f2lCase, moveList))
      {
         return;
      }

      if (SolveF2lCaseEdgeAndCornerInPlace(cube, f2lCase, moveList))
      {
         return;
      }

      assert(false && "Checked every case, but couldn't solve f2l pair");
   }

   /**
    * @return      Returns a tuple representing the f2l case.
    */
   static void FindF2lCase(Cube& cube, tF2lCase& f2lCase)
   {
      eCubeColor color1 = cube.ColorOfFace(eCubeFace::Front);
      eCubeColor color2 = cube.ColorOfFace(eCubeFace::Right);

      f2lCase.FrontColor = color1;
      f2lCase.RightColor = color2;

      // The corner can be in one of 2 places.
      if (CubeSolveUtils::IsCornerInPosition(cube, eCubeFace::Front, eFaceCornerPos::TopRight, color1, color2, BottomColor))
      {
         f2lCase.CornerFace = eCubeFace::Front;
         f2lCase.CornerPos = eFaceCornerPos::TopRight;
      }
      else 
      {
         f2lCase.CornerFace = eCubeFace::Front;
         f2lCase.CornerPos = eFaceCornerPos::BottomRight;
      }

      // The edge can be somewhere on the top, or in the front middle layer.
      bool isInverted;

      // Default values in case it's not found in the top.
      f2lCase.EdgeFace = eCubeFace::Front;
      f2lCase.EdgePos = eFaceEdgePos::RightEdge;

      for (int i = 0; i < 4; i++)
      {
         eFaceEdgePos thisEdge = static_cast<eFaceEdgePos>(i);
         if (CubeSolveUtils::IsEdgeInPosition(cube, eCubeFace::Top, thisEdge, color1, color2, isInverted))
         {
            f2lCase.EdgeFace = eCubeFace::Top;
            f2lCase.EdgePos = thisEdge;
            break;
         }
      }

      f2lCase.CornerColor = CubeSolveUtils::GetCornerColor(cube, f2lCase.CornerFace, f2lCase.CornerPos);
      f2lCase.EdgeColor = CubeSolveUtils::GetEdgeColor(cube, f2lCase.EdgeFace, f2lCase.EdgePos);

      // Ensure we actually found it for the sake of correctness.
      assert(CubeSolveUtils::IsCornerInPosition(cube, f2lCase.CornerFace, f2lCase.CornerPos, color1, color2, BottomColor));
      assert(CubeSolveUtils::IsEdgeInPosition(cube, f2lCase.EdgeFace, f2lCase.EdgePos, color1, color2, isInverted));
   }

   /**
    * @return      Returns true if something had to be done to solve f2l assuming the given face
    * is at the right.
    */
   static bool SolveFirstTwoLayers(Cube& cube, eCubeFace rightFace, CubeMoveList& moveList)
   {
      RotateSideFaceToRight(cube, rightFace, moveList);

      if (IsF2lPairSolved(cube, eCubeFace::Front, eCubeFace::Right))
      {
         moveList.RejectPendingMoves();
         return false;
      }

      // First, position the corner to the right side of the cube, either the top right, or bottom right.
      PositionF2lCornerInRightTopOrBottom(cube, moveList);
      PositionF2lEdgeAtTopOrMiddleRight(cube, moveList);

      // Now find the position of the edge and corner, then solve the case
      tF2lCase f2lCase;
      FindF2lCase(cube, f2lCase);
      SolveF2lCase(cube, f2lCase, moveList);

      moveList.AcceptPendingMoves();
      return true;
   }

   static bool SolveFirstTwoLayers(Cube& cube, std::ostream& outputStream, bool addSeparators)
   {
      CubeMoveList moveList(cube);

      bool neededSolve = true;
      int count = 0;
      while (neededSolve)
      {
         neededSolve = false;
         if (SolveFirstTwoLayers(cube, eCubeFace::Right, moveList))
         {
            neededSolve = true;
            continue;
         }

         if (SolveFirstTwoLayers(cube, eCubeFace::Front, moveList))
         {
            neededSolve = true;
            continue;
         }

         if (SolveFirstTwoLayers(cube, eCubeFace::Back, moveList))
         {
            neededSolve = true;
            continue;
         }

         if (SolveFirstTwoLayers(cube, eCubeFace::Left, moveList))
         {
            neededSolve = true;
            continue;
         }

         if (neededSolve)
         {
            count++;
         }
      };

      assert(count <= 4);
      EnsureF2lSolved(cube);

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

   #pragma region Orient last FirstTwoLayersAlgorithms

   static void EnsureOLLSolved(Cube& cube)
   {
      for (int i = 0; i < CubeSize; i++)
      {
         for (int j = 0; j < CubeSize; j++)
         {
            assert(cube.GetState(eCubeFace::Top, i, j) == cube.ColorOfFace(eCubeFace::Top));
         }
      }
   }

   static bool SolveOrientLastLayer(Cube& cube, std::ostream& outputStream, bool addSeparators)
   {
      CubeMoveList moveList(cube);

      OLLUtils::FindAndExecuteCorrectOLL(cube, moveList);
      moveList.AcceptPendingMoves();
      EnsureOLLSolved(cube);

      if (moveList.GetNumMoves() > 0)
      {
         outputStream << "OLL: ";
         moveList.SerializeMoves(outputStream);
         outputStream << "\n";
         return true;
      }

      outputStream << "OLL: Already Solved";
      return false;
   }

   #pragma endregion

   #pragma region Orient last FirstTwoLayersAlgorithms

   static void EnsurePllSolved(Cube& cube)
   {
      //assert(cube.IsSolved());
   }

   static bool SolvePermeateLastLayer(Cube& cube, std::ostream& outputStream, bool addSeparators)
   {
      CubeMoveList moveList(cube);

      PLLUtils::FindAndExecuteCorrectPLL(cube, moveList);

      // Rotate last layer to solved position if needed.
      RotateColorToBottom(cube, moveList, BottomColor);
      eCubeColor frontColor = cube.ColorOfFace(eCubeFace::Front);
      eCubeColor currentTopLayerFrontColor = cube.GetState(eCubeFace::Front, 0, 0);

      if (frontColor != currentTopLayerFrontColor)
      {
         if (cube.ColorOfFace(eCubeFace::Right) == currentTopLayerFrontColor)
         {
            moveList.PushMove(eCubeMove::UpPrime);
         }
         else if (cube.ColorOfFace(eCubeFace::Left) == currentTopLayerFrontColor)
         {
            moveList.PushMove(eCubeMove::Up);
         }
         else if (cube.ColorOfFace(eCubeFace::Back) == currentTopLayerFrontColor)
         {
            moveList.PushMove(eCubeMove::Up2);
         }
      }

      moveList.AcceptPendingMoves();

      EnsurePllSolved(cube);

      if (moveList.GetNumMoves() > 0)
      {
         outputStream << "PLL: ";
         moveList.SerializeMoves(outputStream);
         outputStream << "\n";
         return true;
      }

      outputStream << "PLL: Already Solved";
      return false;
   }

   #pragma endregion

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

      if (SolveOrientLastLayer(mCube, outputStream, mAddSeparators) && mShowCubeAfterEachStep)
      {
         mCube.Print(outputStream);
      }

      if (SolvePermeateLastLayer(mCube, outputStream, mAddSeparators) && mShowCubeAfterEachStep)
      {
         mCube.Print(outputStream);
      }
   }
}