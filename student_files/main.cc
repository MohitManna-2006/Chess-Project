#include <assert.h>
#include <cmath>
#include <iostream>
#include "Chess.h"
#include "ChessBoard.hh"
#include "ChessPiece.hh"

static bool approx(float a, float b) { return std::fabs(a - b) < 0.001f; }

// ── scoreBoard() Tests ────────────────────────────────────────────────────────

// Symmetric board: white king vs black king, both at corners far apart.
// Each king has 3 moves from its corner, so score = (200+0.3)-(200+0.3) = 0.
void test_score_symmetric()
{
    Student::ChessBoard b(8, 8);
    b.createChessPiece(White, King, 0, 0);
    b.createChessPiece(Black, King, 7, 7);

    float score = b.scoreBoard(); // white's turn
    assert(approx(score, 0.0f) && "Symmetric board should score 0");
    std::cout << "PASS: test_score_symmetric (score=" << score << ")\n";
}

// White has a queen (9), black has a pawn (1). White's turn.
// White queen at (4,4): can reach 27 squares on empty 8x8.
// Black pawn at (0,0) starting row=1 but placed at row 0, can't move forward (off board).
// Actually place black pawn at (1,7) so it can move 1 or 2 steps.
// Let's compute exactly:
//   White queen at (4,4) on 8x8: 27 reachable squares.
//   But black pawn at (1,7) is in the way on one diagonal.
//   This is complex — let's use a simpler setup: just material gap.
void test_score_material_advantage()
{
    Student::ChessBoard b(8, 8);
    // White: rook (5 pts)   Black: pawn (1 pt)
    // No kings — just check the material math (moves will be whatever the engine says)
    b.createChessPiece(White, Rook, 4, 0);
    b.createChessPiece(Black, Pawn, 1, 7);

    float score = b.scoreBoard();
    // White has 5 pts, black has 1 pt, white leads on material.
    // White rook moves + 0.1 - black pawn moves * 0.1 can be anything,
    // but material alone is +4. Score should be positive.
    assert(score > 0.0f && "White material advantage should give positive score");
    std::cout << "PASS: test_score_material_advantage (score=" << score << ")\n";
}

// White lone rook on empty board: exact move count.
// Rook at (0,0) on 8x8: 7 right + 7 down = 14 moves.
// Black has nothing, so:
//   score = (5 + 0.1*14) - (0 + 0) = 5 + 1.4 = 6.4
void test_score_rook_only()
{
    Student::ChessBoard b(8, 8);
    b.createChessPiece(White, Rook, 0, 0);

    float score = b.scoreBoard();
    assert(approx(score, 6.4f) && "Lone rook at (0,0): 5 + 14*0.1 = 6.4");
    std::cout << "PASS: test_score_rook_only (score=" << score << ")\n";
}

// Two equal rooks, one per side, same position on opposite rows.
// White rook at (0,4), Black rook at (7,4).
// White rook: 7 horizontal + 6 vertical (blocked by black rook at (7,4))...
// Actually (0,4): 4 left + 3 right = 7 horizontal. Vertical: rows 1-7 = 7, but black rook
// at (7,4) is capturable so all 7 rows below are reachable (stops at capture) = 7 squares.
// Total white rook moves: 7 + 7 = 14.
// Same for black rook (symmetric).
// score = (5 + 1.4) - (5 + 1.4) = 0.
void test_score_equal_rooks()
{
    Student::ChessBoard b(8, 8);
    b.createChessPiece(White, Rook, 0, 4);
    b.createChessPiece(Black, Rook, 7, 4);

    float score = b.scoreBoard();
    assert(approx(score, 0.0f) && "Equal rooks facing each other: score = 0");
    std::cout << "PASS: test_score_equal_rooks (score=" << score << ")\n";
}

// ── getHighestNextScore() Tests ───────────────────────────────────────────────

// White rook at (0,0) can capture black queen at (0,7) in one move.
// That's clearly the best move: removes 9-pt queen, gains mobility on that rank.
// After capture: white rook at (0,7), no black pieces.
//   score = 5 + 0.1*(7+7) - 0 = 5 + 1.4 = 6.4
// Compare to doing nothing (no capture) — we expect best >= 6.4.
void test_highest_captures_queen()
{
    Student::ChessBoard b(8, 8);
    b.createChessPiece(White, Rook, 0, 0);
    b.createChessPiece(Black, Queen, 0, 7); // on same rank, capturable

    float best = b.getHighestNextScore();
    // After capturing black queen, score = 5 + 0.1*14 = 6.4 (rook at corner, 14 moves)
    assert(approx(best, 6.4f) && "Best move: capture queen, score should be 6.4");
    std::cout << "PASS: test_highest_captures_queen (best=" << best << ")\n";
}

// getHighestNextScore > scoreBoard when a capture improves the position
void test_highest_better_than_current()
{
    Student::ChessBoard b(8, 8);
    b.createChessPiece(White, Rook, 4, 0);
    b.createChessPiece(Black, Rook, 4, 7); // same row, white can capture

    float current = b.scoreBoard();
    float best    = b.getHighestNextScore();
    assert(best > current && "Capturing opponent rook should raise score above current");
    std::cout << "PASS: test_highest_better_than_current (current=" << current
              << ", best=" << best << ")\n";
}

// Board state must be unchanged after getHighestNextScore()
void test_highest_does_not_mutate_board()
{
    Student::ChessBoard b(8, 8);
    b.createChessPiece(White, Rook,  4, 0);
    b.createChessPiece(Black, Queen, 4, 7);

    b.getHighestNextScore(); // should not permanently change the board

    assert(b.getPiece(4, 0) != nullptr && b.getPiece(4, 0)->getType() == Rook  && "Rook should still be at (4,0)");
    assert(b.getPiece(4, 7) != nullptr && b.getPiece(4, 7)->getType() == Queen && "Queen should still be at (4,7)");
    std::cout << "PASS: test_highest_does_not_mutate_board\n";
}

// After getHighestNextScore(), movePiece should still work normally
void test_highest_then_move_still_works()
{
    Student::ChessBoard b(8, 8);
    b.createChessPiece(White, Rook,  0, 0);
    b.createChessPiece(Black, Queen, 0, 7);

    b.getHighestNextScore();
    bool ok = b.movePiece(0, 0, 0, 7); // white rook captures black queen
    assert(ok && "movePiece should still work after getHighestNextScore()");
    assert(b.getPiece(0, 7)->getType() == Rook && "Rook should be at (0,7) after capture");
    std::cout << "PASS: test_highest_then_move_still_works\n";
}

int main()
{
    test_score_symmetric();
    test_score_material_advantage();
    test_score_rook_only();
    test_score_equal_rooks();
    test_highest_captures_queen();
    test_highest_better_than_current();
    test_highest_does_not_mutate_board();
    test_highest_then_move_still_works();

    std::cout << "\nAll scoring tests passed!\n";
    return EXIT_SUCCESS;
}
