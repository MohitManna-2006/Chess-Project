#include "KingPiece.hh"
#include "ChessBoard.hh"

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
    // Stub for Part 1
    return false;
}
