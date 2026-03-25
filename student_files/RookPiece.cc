#include "RookPiece.hh"
#include "ChessBoard.hh"

using Student::RookPiece;

RookPiece::RookPiece(ChessBoard &board, Color color, int row, int column)
    : ChessPiece(board, color, row, column)
{
    type = Rook;
}

const char *RookPiece::toString()
{
    if (color == White)
    {
        return "\u2656";
    }
    else
    {
        return "\u265C";
    }
}

bool RookPiece::canMoveToLocation(int toRow, int toColumn)
{
    //Check destination is in bounds
    if (toRow < 0 || toRow >= board.getNumRows() || toColumn < 0 || toColumn >= board.getNumCols())
    {
        return false;
    }

    //Check that the piece is not staying in the same position
    if (toRow == row && toColumn == column)
    {
        return false;
    }

    //Check that the piece is not capturing a piece of the same color
    ChessPiece *destPiece = board.getPiece(toRow, toColumn);
    if (destPiece != nullptr && destPiece->getColor() == color)
    {
        return false;
    }

    //Check that the piece is moving along a row or a column, not both
    if (toRow != row && toColumn != column)
    {
        return false;
    }

    //Check that there are no pieces blocking the path
    if (toRow == row)
    {
        //Check that the piece is moving horizontally
        int step = (toColumn > column) ? 1 : -1;
        int currentCol = column + step;
        while (currentCol != toColumn)
        {
            if (board.getPiece(row, currentCol) != nullptr)
            {
                return false;
            }
            currentCol = currentCol + step;
        }
    }
    else
    {
        //Check that the piece is moving vertically
        int step = (toRow > row) ? 1 : -1;
        int currentRow = row + step;
        while (currentRow != toRow)
        {
            if (board.getPiece(currentRow, column) != nullptr)
            {
                return false;
            }
            currentRow = currentRow + step;
        }
    }

    return true;
}
