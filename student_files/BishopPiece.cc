#include "BishopPiece.hh"
#include "ChessBoard.hh"

using Student::BishopPiece;

BishopPiece::BishopPiece(ChessBoard &board, Color color, int row, int column)
    : ChessPiece(board, color, row, column)
{
    type = Bishop;
}

const char *BishopPiece::toString()
{
    if (color == White)
    {
        return "\u2657";
    }
    else
    {
        return "\u265D";
    }
}

bool BishopPiece::canMoveToLocation(int toRow, int toColumn)
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

    //Check that the piece is moving diagonally: abs(rowDiff) must equal abs(colDiff)
    int rowDiff = toRow - row;
    int colDiff = toColumn - column;
    int absRowDiff = (rowDiff < 0) ? -rowDiff : rowDiff;
    int absColDiff = (colDiff < 0) ? -colDiff : colDiff;

    if (absRowDiff != absColDiff)
    {
        return false;
    }

    //Check that there are no pieces blocking the diagonal path
    int stepRow = (toRow > row) ? 1 : -1;
    int stepCol = (toColumn > column) ? 1 : -1;
    int currentRow = row + stepRow;
    int currentCol = column + stepCol;

    while (currentRow != toRow && currentCol != toColumn)
    {
        if (board.getPiece(currentRow, currentCol) != nullptr)
        {
            return false;
        }
        currentRow = currentRow + stepRow;
        currentCol = currentCol + stepCol;
    }

    return true;
}
