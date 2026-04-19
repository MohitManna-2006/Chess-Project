#include "KnightPiece.hh"
#include "ChessBoard.hh"

using Student::KnightPiece;

KnightPiece::KnightPiece(ChessBoard &board, Color color, int row, int column)
    : ChessPiece(board, color, row, column)
{
    type = Knight;
}

const char *KnightPiece::toString()
{
    if (color == White)
    {
        return "\u2658";
    }
    else
    {
        return "\u265E";
    }
}

bool KnightPiece::canMoveToLocation(int toRow, int toColumn)
{
    if (toRow < 0 || toRow >= board.getNumRows() || toColumn < 0 || toColumn >= board.getNumCols())
    {
        return false;
    }

    if (toRow == row && toColumn == column)
    {
        return false;
    }

    ChessPiece *dest = board.getPiece(toRow, toColumn);
    if (dest != nullptr && dest->getColor() == color)
    {
        return false;
    }

    int rowDiff = (toRow - row < 0) ? -(toRow - row) : (toRow - row);
    int colDiff = (toColumn - column < 0) ? -(toColumn - column) : (toColumn - column);

    // L-shape: (2,1) or (1,2)
    return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}
