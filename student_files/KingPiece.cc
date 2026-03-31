#include "KingPiece.hh"
#include "ChessBoard.hh"
#include <cstdlib>

using Student::KingPiece;

KingPiece::KingPiece(ChessBoard &board, Color color, int row, int column)
    : ChessPiece(board, color, row, column)
{
    type = King;
}

const char *KingPiece::toString()
{
    if (color == White)
    {
        return "\u2654";
    }
    else
    {
        return "\u265A";
    }
}

bool KingPiece::canMoveToLocation(int toRow, int toColumn)
{
    // Bounds check
    if (toRow < 0 || toRow >= board.getNumRows() ||
        toColumn < 0 || toColumn >= board.getNumCols())
    {
        return false;
    }

    // Same position check
    if (toRow == row && toColumn == column)
    {
        return false;
    }

    // Cannot capture own piece
    ChessPiece *dest = board.getPiece(toRow, toColumn);
    if (dest != nullptr && dest->getColor() == color)
    {
        return false;
    }

    // Must move exactly 1 square in any direction
    int rowDiff = std::abs(toRow - row);
    int colDiff = std::abs(toColumn - column);
    if (rowDiff > 1 || colDiff > 1)
    {
        return false;
    }

    return true;
}
