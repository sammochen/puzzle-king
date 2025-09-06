#pragma once
#include "game.h"
#include "piece.h"

struct Evaluation {
    // if has value, + is white's mate, - is black's mate
    std::optional<std::pair<Color, int>> colorHasMate;
    double eval; //-1 -> 1

    static Evaluation hasMate(Color color, int mate) {
        return Evaluation{
            std::make_optional<std::pair<Color, int>>({color, mate}),
            color == Color::White ? 1.0 : -1.0};
    }

    static Evaluation hasNoMate(double eval) {
        return Evaluation{std::nullopt, eval};
    }

    // return as -1 -> 1
    double evalAsColor(Color color) const {
        return color == Color::White ? eval : -eval;
    }
};

std::ostream &operator<<(std::ostream &os, const Evaluation &rhs) {
    if (rhs.colorHasMate) {
        os << (rhs.colorHasMate->first == Color::White ? "W" : "B")
           << rhs.colorHasMate->second;
    } else {
        os << rhs.eval;
    }
    return os;
}

// Heuristic - how to valuate one position
struct Eval {
    // Return 1 when white is winning and -1 when black is winning
    Evaluation evaluate(const Game &game) {
        const auto status = game.getStatus();

        if (status == GameStatus::WhiteWin) {
            return Evaluation::hasMate(Color::White, 0);
        }
        if (status == GameStatus::BlackWin) {
            return Evaluation::hasMate(Color::Black, 0);
        }
        if (status == GameStatus::Draw) {
            return Evaluation::hasNoMate(0);
        }

        const double naive_sum = calc_naive_sum(game);
        const double result = sigmoid(naive_sum) * 2 - 1;
        assert(result >= -1 && result <= 1);
        return Evaluation::hasNoMate(result);
    }

    double calc_naive_sum(const Game &game) const {
        double sum = 0;
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                const auto &piece = game.board.pieces[row][col];
                if (piece == std::nullopt)
                    continue; // no piece here

                sum += (piece->color == Color::White ? 1 : -1) *
                       calc_naive_value(piece->piece);
            }
        }
        return sum;
    }

    static double flip(double value) { return -value; }

    double sigmoid(double x) const {
        const double res = 1.0 / (1.0 + std::exp(-x / 8));
        // if (res > 0.9)
        //     return 0.9;
        // if (res < 0.1)
        //     return 0.1;

        assert(res >= 0 && res <= 1);
        return res;
    }

    // map to a probability of how much you want to make each move
    static std::vector<double> normalise(const std::vector<Evaluation> &P,
                                         Color color) {
        int n = P.size();

        std::vector<double> result(n);

        double sumOfResult = 0;
        for (int i = 0; i < n; i++) {
            // map to [0,1] where 1 is good from the color's perspective
            result[i] = (P[i].evalAsColor(color) + 1) / 2.0;
            sumOfResult += result[i];
        }

        // and then divide by sumOfResult
        if (sumOfResult == 0) {
            return std::vector<double>(n, 1.0 / n);
        }
        if (sumOfResult < 0) {
            throw std::runtime_error("sumOfResult < 0 not allowed to be <0");
        }

        for (int i = 0; i < n; i++) {
            result[i] /= sumOfResult;
            if (result[i] < 0) {
                throw std::runtime_error("result[i] < 0 not allowed to be <0");
            }
        }

        return result;
    }

    double calc_naive_value(Piece piece) const {
        switch (piece) {
        case Piece::King:
            return 0;
        case Piece::Queen:
            return 9;
        case Piece::Rook:
            return 5;
        case Piece::Bishop:
            return 3;
        case Piece::Knight:
            return 3;
        case Piece::Pawn:
            return 1;
        }
        assert(false);
    }
};
