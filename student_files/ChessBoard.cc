#include "ChessBoard.hh"
#include "PawnPiece.hh"
#include "RookPiece.hh"
#include "BishopPiece.hh"
#include "KingPiece.hh"
#include "KnightPiece.hh"
#include "QueenPiece.hh"

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
    else if (ty == Knight)
    {
        newPiece = new KnightPiece(*this, col, startRow, startColumn);
    }
    else if (ty == Queen)
    {
        newPiece = new QueenPiece(*this, col, startRow, startColumn);
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

    // Castling: king moves exactly 2 squares horizontally
    bool isCastle = false;
    if (piece->getType() == King && fromRow == toRow)
    {
        int colDiff = (toColumn > fromColumn) ? (toColumn - fromColumn) : (fromColumn - toColumn);
        if (colDiff == 2)
        {
            if (!isCastlingValid(fromRow, fromColumn, toRow, toColumn))
                return false;
            isCastle = true;
        }
    }

    // Delegate to piece movement logic (skip for confirmed castling — canMoveToLocation
    // rejects 2-square king moves; castling validity was already checked above)
    if (!isCastle && !piece->canMoveToLocation(toRow, toColumn))
    {
        return false;
    }

    //Part 3: check constraint — no move is valid if it leaves own King in check
    Color movingColor = piece->getColor();

    // Detect en passant: pawn moving diagonally to empty en passant target square
    bool isEnPassant = false;
    int epCapRow = -1, epCapCol = -1;
    if (piece->getType() == Pawn &&
        toColumn != fromColumn &&
        board.at(toRow).at(toColumn) == nullptr &&
        toRow == enPassantTargetRow && toColumn == enPassantTargetCol)
    {
        isEnPassant = true;
        epCapRow = enPassantPawnRow;
        epCapCol = enPassantPawnCol;
    }

    //Simulate the move
    ChessPiece *captured = board.at(toRow).at(toColumn);
    ChessPiece *epCaptured = nullptr;
    board.at(toRow).at(toColumn) = piece;
    board.at(fromRow).at(fromColumn) = nullptr;
    piece->setPosition(toRow, toColumn);
    if (isEnPassant)
    {
        epCaptured = board.at(epCapRow).at(epCapCol);
        board.at(epCapRow).at(epCapCol) = nullptr;
    }

    //Find own King's position on the board after the simulated move
    int kingRow = -1;
    int kingCol = -1;
    for (int r = 0; r < numRows; r++)
    {
        for (int c = 0; c < numCols; c++)
        {
            ChessPiece *p = board.at(r).at(c);
            if (p != nullptr && p->getColor() == movingColor && p->getType() == King)
            {
                kingRow = r;
                kingCol = c;
            }
        }
    }

    //Check if own King is under threat using raw attack rules
    bool inCheck = false;
    if (kingRow != -1)
    {
        inCheck = isPieceUnderThreat(kingRow, kingCol);
    }

    //Undo the simulation
    board.at(fromRow).at(fromColumn) = piece;
    board.at(toRow).at(toColumn) = captured;
    piece->setPosition(fromRow, fromColumn);
    if (isEnPassant)
    {
        board.at(epCapRow).at(epCapCol) = epCaptured;
    }

    if (inCheck)
    {
        return false;
    }

    return true;
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

    // Detect castling before executing the move
    bool isCastle = false;
    int castleRookFromCol = -1;
    int castleRookToCol   = -1;
    if (movingPiece->getType() == King && fromRow == toRow)
    {
        int colDiff = (toColumn > fromColumn) ? (toColumn - fromColumn) : (fromColumn - toColumn);
        if (colDiff == 2)
        {
            isCastle = true;
            int dir = (toColumn > fromColumn) ? 1 : -1;
            // Find the rook (same scan as isCastlingValid)
            for (int c = fromColumn + dir; c >= 0 && c < numCols; c += dir)
            {
                if (board.at(fromRow).at(c) != nullptr)
                {
                    castleRookFromCol = c;
                    break;
                }
            }
            castleRookToCol = fromColumn + dir; // square king skips over
        }
    }

    // Detect en passant capture before clearing state
    bool isEnPassant = false;
    int epCapRow = -1, epCapCol = -1;
    if (movingPiece->getType() == Pawn &&
        toColumn != fromColumn &&
        board.at(toRow).at(toColumn) == nullptr &&
        toRow == enPassantTargetRow && toColumn == enPassantTargetCol)
    {
        isEnPassant = true;
        epCapRow = enPassantPawnRow;
        epCapCol = enPassantPawnCol;
    }

    // Clear en passant state (expires after one turn)
    enPassantTargetRow = -1;
    enPassantTargetCol = -1;
    enPassantPawnRow = -1;
    enPassantPawnCol = -1;

    // Set new en passant state if this is a 2-square pawn advance
    int rowDiff = (toRow > fromRow) ? (toRow - fromRow) : (fromRow - toRow);
    if (movingPiece->getType() == Pawn && rowDiff == 2)
    {
        enPassantTargetRow = (fromRow + toRow) / 2;
        enPassantTargetCol = toColumn;
        enPassantPawnRow   = toRow;
        enPassantPawnCol   = toColumn;
    }

    // Remove en passant captured pawn (it is NOT at the destination square)
    if (isEnPassant)
    {
        delete board.at(epCapRow).at(epCapCol);
        board.at(epCapRow).at(epCapCol) = nullptr;
    }

    // Normal capture at destination (if any)
    ChessPiece *cP = board.at(toRow).at(toColumn);
    if (cP != nullptr)
    {
        delete cP;
        board.at(toRow).at(toColumn) = nullptr;
    }

    board.at(toRow).at(toColumn) = movingPiece;
    board.at(fromRow).at(fromColumn) = nullptr;
    movingPiece->setPosition(toRow, toColumn);
    movingPiece->setHasMoved();

    // Castling: move the rook to the square the king skipped over
    if (isCastle && castleRookFromCol != -1)
    {
        ChessPiece *rook = board.at(fromRow).at(castleRookFromCol);
        board.at(fromRow).at(castleRookToCol)   = rook;
        board.at(fromRow).at(castleRookFromCol) = nullptr;
        rook->setPosition(fromRow, castleRookToCol);
        rook->setHasMoved();
    }

    // Pawn promotion: replace pawn with queen when it reaches the back rank
    if (movingPiece->getType() == Pawn)
    {
        bool promoted = (movingPiece->getColor() == White && toRow == 0) ||
                        (movingPiece->getColor() == Black && toRow == numRows - 1);
        if (promoted)
        {
            Color promColor = movingPiece->getColor();
            delete board.at(toRow).at(toColumn);
            board.at(toRow).at(toColumn) = new QueenPiece(*this, promColor, toRow, toColumn);
        }
    }

    turn = (turn == White) ? Black : White;

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

            if (attack->canMoveToLocation(row, column))
            {
                return true;
            }
        }
    }

    return false;
}

static float pieceValue(Type t)
{
    if (t == King)   return 200.0f;
    if (t == Queen)  return 9.0f;
    if (t == Rook)   return 5.0f;
    if (t == Knight) return 3.0f;
    if (t == Bishop) return 3.0f;
    return 1.0f; // Pawn
}

float ChessBoard::scoreBoard()
{
    Color myColor = turn;

    float myPieces = 0.0f, opPieces = 0.0f;
    for (int r = 0; r < numRows; r++)
        for (int c = 0; c < numCols; c++) {
            ChessPiece *p = board.at(r).at(c);
            if (p == nullptr) continue;
            if (p->getColor() == myColor) myPieces += pieceValue(p->getType());
            else                          opPieces += pieceValue(p->getType());
        }

    float myMoves = 0.0f, opMoves = 0.0f;
    for (int fr = 0; fr < numRows; fr++)
        for (int fc = 0; fc < numCols; fc++) {
            ChessPiece *p = board.at(fr).at(fc);
            if (p == nullptr) continue;
            bool isMine = (p->getColor() == myColor);
            for (int tr = 0; tr < numRows; tr++)
                for (int tc = 0; tc < numCols; tc++) {
                    if (fr == tr && fc == tc) continue;
                    if (isValidMove(fr, fc, tr, tc)) {
                        if (isMine) myMoves += 1.0f;
                        else        opMoves += 1.0f;
                    }
                }
        }

    return (myPieces + 0.1f * myMoves) - (opPieces + 0.1f * opMoves);
}

float ChessBoard::getHighestNextScore()
{
    Color myColor = turn;

    // Snapshot every piece's state so we can restore after each trial move
    struct PieceSnap { int r, c; Color color; Type type; bool moved; };
    std::vector<PieceSnap> snaps;
    for (int r = 0; r < numRows; r++)
        for (int c = 0; c < numCols; c++) {
            ChessPiece *p = board.at(r).at(c);
            if (p != nullptr)
                snaps.push_back({r, c, p->getColor(), p->getType(), p->getHasMoved()});
        }
    Color savedTurn  = turn;
    int savedEpTR = enPassantTargetRow, savedEpTC = enPassantTargetCol;
    int savedEpPR = enPassantPawnRow,   savedEpPC = enPassantPawnCol;

    // Restore board to the snapshotted state
    auto restore = [&]() {
        for (int r = 0; r < numRows; r++)
            for (int c = 0; c < numCols; c++) {
                delete board.at(r).at(c);
                board.at(r).at(c) = nullptr;
            }
        for (auto &s : snaps) {
            createChessPiece(s.color, s.type, s.r, s.c);
            if (s.moved) board.at(s.r).at(s.c)->setHasMoved();
        }
        turn               = savedTurn;
        enPassantTargetRow = savedEpTR;
        enPassantTargetCol = savedEpTC;
        enPassantPawnRow   = savedEpPR;
        enPassantPawnCol   = savedEpPC;
    };

    float best  = 0.0f;
    bool  found = false;

    for (int fr = 0; fr < numRows; fr++) {
        for (int fc = 0; fc < numCols; fc++) {
            ChessPiece *p = board.at(fr).at(fc);
            if (p == nullptr || p->getColor() != myColor) continue;
            for (int tr = 0; tr < numRows; tr++) {
                for (int tc = 0; tc < numCols; tc++) {
                    if (!isValidMove(fr, fc, tr, tc)) continue;
                    movePiece(fr, fc, tr, tc);          // turn switches to opponent
                    float score = -scoreBoard();         // negate: back to myColor's view
                    if (!found || score > best) { best = score; found = true; }
                    restore();
                }
            }
        }
    }

    return best;
}

bool ChessBoard::isSquareUnderAttack(int row, int col, Color byColor)
{
    for (int r = 0; r < numRows; r++)
    {
        for (int c = 0; c < numCols; c++)
        {
            ChessPiece *p = board.at(r).at(c);
            if (p != nullptr && p->getColor() == byColor)
            {
                if (p->canMoveToLocation(row, col))
                    return true;
            }
        }
    }
    return false;
}

bool ChessBoard::isCastlingValid(int fromRow, int fromCol, int toRow, int toCol)
{
    ChessPiece *king = board.at(fromRow).at(fromCol);

    // King must not have moved
    if (king->getHasMoved())
        return false;

    Color kingColor = king->getColor();
    Color oppColor  = (kingColor == White) ? Black : White;

    // King must not currently be in check
    if (isSquareUnderAttack(fromRow, fromCol, oppColor))
        return false;

    int dir = (toCol > fromCol) ? 1 : -1;

    // Scan in castling direction: first piece must be an unmoved friendly rook
    // (this simultaneously checks the path is unobstructed)
    ChessPiece *rook = nullptr;
    for (int c = fromCol + dir; c >= 0 && c < numCols; c += dir)
    {
        ChessPiece *p = board.at(fromRow).at(c);
        if (p != nullptr)
        {
            if (p->getColor() == kingColor && p->getType() == Rook && !p->getHasMoved())
                rook = p;
            break; // first piece found; path up to here is clear
        }
    }
    if (rook == nullptr)
        return false;

    // The square the king skips over must not be under attack
    int skipCol = fromCol + dir;
    if (isSquareUnderAttack(fromRow, skipCol, oppColor))
        return false;

    return true;
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
