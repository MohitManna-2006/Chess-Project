#include <iostream>
#include <string>
#include <cstdio>
#include "Chess.h"
#include "ChessBoard.hh"
#include "ChessPiece.hh"

static void setupStandardBoard(Student::ChessBoard &b)
{
    // Black back rank (row 0)
    b.createChessPiece(Black, Rook,   0, 0);
    b.createChessPiece(Black, Knight, 0, 1);
    b.createChessPiece(Black, Bishop, 0, 2);
    b.createChessPiece(Black, Queen,  0, 3);
    b.createChessPiece(Black, King,   0, 4);
    b.createChessPiece(Black, Bishop, 0, 5);
    b.createChessPiece(Black, Knight, 0, 6);
    b.createChessPiece(Black, Rook,   0, 7);
    for (int c = 0; c < 8; c++)
        b.createChessPiece(Black, Pawn, 1, c);

    // White back rank (row 7)
    for (int c = 0; c < 8; c++)
        b.createChessPiece(White, Pawn, 6, c);
    b.createChessPiece(White, Rook,   7, 0);
    b.createChessPiece(White, Knight, 7, 1);
    b.createChessPiece(White, Bishop, 7, 2);
    b.createChessPiece(White, Queen,  7, 3);
    b.createChessPiece(White, King,   7, 4);
    b.createChessPiece(White, Bishop, 7, 5);
    b.createChessPiece(White, Knight, 7, 6);
    b.createChessPiece(White, Rook,   7, 7);
}

static bool isInCheck(Student::ChessBoard &b, Color color)
{
    for (int r = 0; r < b.getNumRows(); r++)
        for (int c = 0; c < b.getNumCols(); c++) {
            Student::ChessPiece *p = b.getPiece(r, c);
            if (p && p->getColor() == color && p->getType() == King)
                return b.isPieceUnderThreat(r, c);
        }
    return false;
}

static bool hasAnyLegalMove(Student::ChessBoard &b, Color color)
{
    for (int fr = 0; fr < b.getNumRows(); fr++)
        for (int fc = 0; fc < b.getNumCols(); fc++) {
            Student::ChessPiece *p = b.getPiece(fr, fc);
            if (!p || p->getColor() != color) continue;
            for (int tr = 0; tr < b.getNumRows(); tr++)
                for (int tc = 0; tc < b.getNumCols(); tc++)
                    if (b.isValidMove(fr, fc, tr, tc))
                        return true;
        }
    return false;
}

int main()
{
    Student::ChessBoard board;
    setupStandardBoard(board);

    Color currentTurn = White;

    std::cout << "\n=== Chess ===\n";
    std::cout << "Row 0 = Black's back rank   Row 7 = White's back rank\n";
    std::cout << "Enter moves as:  fromRow fromCol toRow toCol\n";
    std::cout << "Example:         6 4 4 4   (White pawn e2-e4)\n";
    std::cout << "Type 'quit' to exit\n\n";

    while (true) {
        std::cout << board.displayBoard().str();
        std::cout << (currentTurn == White ? "White" : "Black") << "'s turn > ";
        std::cout.flush();

        std::string line;
        if (!std::getline(std::cin, line)) break;
        if (line == "quit" || line == "q") {
            std::cout << "Goodbye!\n";
            break;
        }

        int fr, fc, tr, tc;
        if (std::sscanf(line.c_str(), "%d %d %d %d", &fr, &fc, &tr, &tc) != 4) {
            std::cout << "Bad input. Format: fromRow fromCol toRow toCol  (e.g. 6 4 4 4)\n\n";
            continue;
        }

        if (!board.movePiece(fr, fc, tr, tc)) {
            std::cout << "Invalid move — try again.\n\n";
            continue;
        }

        currentTurn = (currentTurn == White) ? Black : White;

        if (isInCheck(board, currentTurn)) {
            if (!hasAnyLegalMove(board, currentTurn)) {
                std::cout << board.displayBoard().str();
                std::cout << "Checkmate! "
                          << (currentTurn == White ? "Black" : "White")
                          << " wins!\n";
                break;
            }
            std::cout << (currentTurn == White ? "White" : "Black") << " is in check!\n\n";
        } else if (!hasAnyLegalMove(board, currentTurn)) {
            std::cout << board.displayBoard().str();
            std::cout << "Stalemate — it's a draw!\n";
            break;
        }
    }

    return 0;
}
