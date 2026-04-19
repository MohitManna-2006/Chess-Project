#include "PawnPiece.hh"
#include "ChessBoard.hh"

using Student::PawnPiece;

PawnPiece::PawnPiece(ChessBoard &board, Color color, int row, int column)
    : ChessPiece(board, color, row, column)
{
    type = Pawn;
}

const char *PawnPiece::toString()
{
    if (color == White)
    {
        return "\u2659";
    }
    else
    {
        return "\u265F";
    }
}

bool PawnPiece::canMoveToLocation(int toRow, int toColumn)
{
    //Check that destination is in bounds
    if (toRow < 0 || toRow >= board.getNumRows() || toColumn < 0 || toColumn >= board.getNumCols())
    {
        return false;
    }

    //Check that the piece is not staying in the same position
    if (toRow == row && toColumn == column)
    {
        return false;
    }

    //Determine direction of movement based on color
    int direction;
    if (color == Black)
    {
        direction = 1;
    }
    else
    {
        direction = -1;
    }

    //Determine starting row based on color
    int startRow;
    if (color == Black)
    {
        startRow = 1;
    }
    else
    {
        startRow = board.getNumRows() - 2;
    }

    //Case 1: Move forward one step
    if (toColumn == column && toRow == row + direction)
    {
        //Destination must be empty
        if (board.getPiece(toRow, toColumn) == nullptr)
        {
            return true;
        }
        return false;
    }

    //Case 2: Move forward two steps from starting row
    if (toColumn == column && toRow == row + 2 * direction)
    {
        //Check that the piece is on the starting row
        if (row != startRow)
        {
            return false;
        }
        //Check that the destination is empty
        if (board.getPiece(toRow, toColumn) != nullptr)
        {
            return false;
        }
        //Check that the intermediate square is empty
        if (board.getPiece(row + direction, column) != nullptr)
        {
            return false;
        }
        return true;
    }

    //Case 3: Diagonal capture (including en passant)
    if (toRow == row + direction)
    {
        if (toColumn == column + 1 || toColumn == column - 1)
        {
            ChessPiece *destPiece = board.getPiece(toRow, toColumn);
            if (destPiece != nullptr && destPiece->getColor() != color)
            {
                return true;
            }
            // En passant: destination is empty but matches the en passant target square
            if (destPiece == nullptr &&
                toRow == board.getEnPassantTargetRow() &&
                toColumn == board.getEnPassantTargetCol())
            {
                return true;
            }
            return false;
        }
    }

    return false;
}
