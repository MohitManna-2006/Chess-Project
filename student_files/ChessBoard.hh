#ifndef _CHESSBOARD_H__
#define _CHESSBOARD_H__

#include "ChessPiece.hh"
#include "KingPiece.hh"
#include "KnightPiece.hh"
#include "QueenPiece.hh"
#include <list>
#include <vector>
#include <sstream>

namespace Student
{
    class ChessBoard
    {
    private:
        int numRows = 0;
        int numCols = 0;
        Color turn = White;
        std::vector<std::vector<ChessPiece *>> board;
        // En passant state: square the capturing pawn moves TO, and position of the capturable pawn
        int enPassantTargetRow = -1;
        int enPassantTargetCol = -1;
        int enPassantPawnRow = -1;
        int enPassantPawnCol = -1;

        // Returns true if any piece of byColor can move to (row,col)
        bool isSquareUnderAttack(int row, int col, Color byColor);
        // Returns true if a king at (fromRow,fromCol) moving to (toRow,toCol) is a legal castle
        bool isCastlingValid(int fromRow, int fromCol, int toRow, int toCol);

    public:
        /**
         * @brief
         * Allocates memory on the heap for the board.
         * Remember to initialise all pointers to nullptr.
         * @param numRow
         * Number of rows of the chess board.
         * @param numCol
         * Number of columns of the chessboard
         */
        ChessBoard(int numRow, int numCol);

        /**
         * @brief
         * Default constructor. Creates an 8x8 chess board.
         */
        ChessBoard();

        /**
         * @brief
         * Destructor. Frees all dynamically allocated pieces.
         */
        ~ChessBoard();

        /**
         * @return
         * Number of rows in chess board.
         */
        int getNumRows() { return numRows; }

        /**
         * @return
         * Number of columns in chess board.
         */
        int getNumCols() { return numCols; }

        /**
         * @return
         * Pointer to a piece.
         */
        ChessPiece *getPiece(int r, int c) { return board.at(r).at(c); }

        int getEnPassantTargetRow() { return enPassantTargetRow; }
        int getEnPassantTargetCol() { return enPassantTargetCol; }
        int getEnPassantPawnRow()   { return enPassantPawnRow; }
        int getEnPassantPawnCol()   { return enPassantPawnCol; }

        /**
         * @brief
         * Allocates memory for a new chess piece and assigns its
         * address to the corresponding pointer in the 'board' variable.
         * Remove any existing piece first before adding the new piece.
         * @param col
         * Color of the piece to be created.
         * @param ty
         * Type of the piece to be created.
         * @param startRow
         * Starting row of the piece to be created.
         * @param startColumn
         * Starting column of the piece to be created.
         */
        void createChessPiece(Color col, Type ty, int startRow, int startColumn);

        /**
         * @brief
         * Performs the move if the move is valid.
         * Account for the turn, staying within bounds and validity of the move.
         * This function is only needed for Part 2 and Part 3.
         * You can define a dummy implementation for Part 1 to get the code to compile.
         * @param fromRow
         * The row of the piece to be moved.
         * @param fromColumn
         * The column of the piece to be moved.
         * @param toRow
         * The row of the destination position.
         * @param toColumn
         * The column of the destination position.
         * @return
         * A boolean indicating whether move was executed successfully.
         */
        bool movePiece(int fromRow, int fromColumn, int toRow, int toColumn);

        /**
         * @brief
         * Checks if a move is valid without accounting for turns.
         * @param fromRow
         * The row of the piece to be moved.
         * @param fromColumn
         * The column of the piece to be moved.
         * @param toRow
         * The row of the destination position.
         * @param toColumn
         * The column of the destination position.
         * @return
         * Returns true if move may be executed without accounting for turn.
         */
        bool isValidMove(int fromRow, int fromColumn, int toRow, int toColumn);

        /**
         * @brief
         * Checks if the piece at a position is under threat.
         * This function is only needed for Part 2 and Part 3.
         * You can define a dummy implementation for Part 1 to get the code to compile.
         * @param row
         * Row of piece being checked.
         * @param column
         * Column of piece being checked.
         * @return
         * Returns true if a piece exists at the stated position, and an opponent
         * piece may move to the position.
         */
        bool isPieceUnderThreat(int row, int column);

        /**
         * @brief
         * Returns an output string stream displaying the layout of the board.
         * An ostringstream is used to automatically handle formatting of integers
         * and special characters into their string representations.
         * @return
         * An output stream containing the full board layout.
         */
        std::ostringstream displayBoard();

        /**
         * @brief Computes score from the perspective of the current player (turn).
         * Score = (own piece value + 0.1 * own legal moves)
         *       - (opponent piece value + 0.1 * opponent legal moves)
         * Piece values: King=200, Queen=9, Rook=5, Knight/Bishop=3, Pawn=1.
         */
        float scoreBoard();

        /**
         * @brief Returns the highest score the current player can achieve
         * after making a single legal move, from the current player's perspective.
         */
        float getHighestNextScore();
    };
}

#endif
