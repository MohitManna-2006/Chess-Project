#include "QueenPiece.hh"
#include "ChessBoard.hh"

using Student::QueenPiece;

QueenPiece::QueenPiece(ChessBoard &board, Color color, int row, int column)
    : ChessPiece(board, color, row, column)
{
    type = Queen;
}

const char *QueenPiece::toString()
{
    if (color == White)
    {
        return "\u2655";
    }
    else
    {
        return "\u265B";
    }
}

bool QueenPiece::canMoveToLocation(int toRow, int toColumn)
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

    int rowDiff = toRow - row;
    int colDiff = toColumn - column;
    int absRowDiff = (rowDiff < 0) ? -rowDiff : rowDiff;
    int absColDiff = (colDiff < 0) ? -colDiff : colDiff;

    bool isDiagonal = (absRowDiff == absColDiff);
    bool isStraight = (toRow == row || toColumn == column);

    if (!isDiagonal && !isStraight)
    {
        return false;
    }

    // Check for obstruction along path
    int stepRow = 0;
    int stepCol = 0;
    if (rowDiff != 0) stepRow = (rowDiff > 0) ? 1 : -1;
    if (colDiff != 0) stepCol = (colDiff > 0) ? 1 : -1;

    int curRow = row + stepRow;
    int curCol = column + stepCol;
    while (curRow != toRow || curCol != toColumn)
    {
        if (board.getPiece(curRow, curCol) != nullptr)
        {
            return false;
        }
        curRow += stepRow;
        curCol += stepCol;
    }

    return true;
}
