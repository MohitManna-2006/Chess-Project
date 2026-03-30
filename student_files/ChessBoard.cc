#include "ChessBoard.hh"
#include "PawnPiece.hh"
#include "RookPiece.hh"
#include "BishopPiece.hh"
#include "KingPiece.hh"

using Student::ChessBoard;

ChessBoard::ChessBoard(int numRow, int numCol)
{
    numRows = numRow;
    numCols = numCol;
    turn = White;

    //Initialize the board with nullptr pointers
    for (int i = 0; i < numRows; i++)
    {
        std::vector<ChessPiece *> row;
        for (int j = 0; j < numCols; j++)
        {
            row.push_back(nullptr);
        }
        board.push_back(row);
    }
}

ChessBoard::ChessBoard() : ChessBoard(8, 8)
{
}

ChessBoard::~ChessBoard()
{
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            if (board.at(i).at(j) != nullptr)
            {
                delete board.at(i).at(j);
                board.at(i).at(j) = nullptr;
            }
        }
    }
}

void ChessBoard::createChessPiece(Color col, Type ty, int startRow, int startColumn)
{
    //Remove any existing piece at that position
    if (board.at(startRow).at(startColumn) != nullptr)
    {
        delete board.at(startRow).at(startColumn);
        board.at(startRow).at(startColumn) = nullptr;
    }

    //Create the correct piece type
    ChessPiece *newPiece = nullptr;
    if (ty == Pawn)
    {
        newPiece = new PawnPiece(*this, col, startRow, startColumn);
    }
    else if (ty == Rook)
    {
        newPiece = new RookPiece(*this, col, startRow, startColumn);
    }
    else if (ty == Bishop)
    {
        newPiece = new BishopPiece(*this, col, startRow, startColumn);
    }
    else if (ty == King)
    {
        newPiece = new KingPiece(*this, col, startRow, startColumn);
    }

    board.at(startRow).at(startColumn) = newPiece;
}

bool ChessBoard::isValidMove(int fromRow, int fromColumn, int toRow, int toColumn)
{
    //Check that source is in bounds
    if (fromRow < 0 || fromRow >= numRows || fromColumn < 0 || fromColumn >= numCols)
    {
        return false;
    }

    //Check that a piece exists at the source
    ChessPiece *piece = board.at(fromRow).at(fromColumn);
    if (piece == nullptr)
    {
        return false;
    }

    //Check that destination is in bounds
    if (toRow < 0 || toRow >= numRows || toColumn < 0 || toColumn >= numCols)
    {
        return false;
    }

    //Check that source and destination are different
    if (fromRow == toRow && fromColumn == toColumn)
    {
        return false;
    }

    //Delegate to the piece's own movement logic
    return piece->canMoveToLocation(toRow, toColumn);
}

bool ChessBoard::movePiece(int fromRow, int fromColumn, int toRow, int toColumn)
{
    if (fromRow < 0 || fromRow >= numRows || fromColumn < 0 || fromColumn >= numCols)
    {
        return false;
    }
    if (toRow < 0 || toRow >= numRows || toColumn < 0 || toColumn >= numCols)
    {
        return false;
    }

    ChessPiece *movingPiece = board.at(fromRow).at(fromColumn);
    if (movingPiece == nullptr)
    {
        return false;
    }
    if (movingPiece->getColor() != turn)
    {
        return false;
    }
    if (!isValidMove(fromRow, fromColumn, toRow, toColumn))
    {
        return false;
    }
    ChessPiece *cP = board.at(toRow).at(toColumn);
    if (cP != nullptr)
    {
        delete cP;
        board.at(toRow).at(toColumn) = nullptr;
    }

    board.at(toRow).at(toColumn) = movingPiece;
    board.at(fromRow).at(fromColumn) = nullptr;
    movingPiece->setPosition(toRow, toColumn);
    if (turn == White)
    {
        turn = Black;
    }
    else
    {
        turn = White;
    }

    return true;
}

bool ChessBoard::isPieceUnderThreat(int row, int column)
{
    if (row < 0 || row >= numRows || column < 0 || column >= numCols)
    {
        return false;
    }

    ChessPiece *tP = board.at(row).at(column);
    if (tP == nullptr)
    {
        return false;
    }

    Color tC = tP->getColor();
    for (int r = 0; r < numRows; r++)
    {
        for (int c = 0; c < numCols; c++)
        {
            ChessPiece *attack = board.at(r).at(c);
            if (attack == nullptr)
            {
                continue;
            }

            if (attack->getColor() == tC)
            {
                continue;
            }

            if (isValidMove(r, c, row, column))
            {
                return true;
            }
        }
    }

    return false;
}

std::ostringstream ChessBoard::displayBoard()
{
    std::ostringstream outputString;
    //top scale of numbers
    outputString << "  ";
    for (int i = 0; i < numCols; i++)
    {
        outputString << i << " ";
    }
    outputString << std::endl << "  ";
    //top border of letters
    for (int i = 0; i < numCols; i++)
    {
        outputString << "– ";
    }
    outputString << std::endl;

    for (int row = 0; row < numRows; row++)
    {
        //left border of numbers
        outputString << row << "|";
        for (int column = 0; column < numCols; column++)
        {
            ChessPiece *piece = board.at(row).at(column);
            outputString << (piece == nullptr ? " " : piece->toString()) << " ";
        }
        outputString << "|" << std::endl;
    }

    //bottom border
    outputString << "  ";
    for (int i = 0; i < numCols; i++)
    {
        outputString << "– ";
    }
    outputString << std::endl << std::endl;

    return outputString;
}
