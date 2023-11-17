#pragma once
#include <cctype>
#include <optional>

struct Color {
    enum Value { White, Black } value;
    constexpr Color(Value v) : value(v) {}

    constexpr Color other() const {
        return value == White ? Color::Black : Color::White;
    }

    constexpr operator Value() const { return value; }
    explicit operator bool() const = delete;
};

struct Piece {
    enum Value { King, Queen, Rook, Bishop, Knight, Pawn } value;
    constexpr Piece(Value v) : value(v) {}
    constexpr operator Value() const { return value; }
    explicit operator bool() const = delete;

    static std::optional<Piece> fromChar(char c) {
        const char lowerC = std::tolower(c);
        if (lowerC == 'r') {
            return Piece::Rook;
        } else if (lowerC == 'n') {
            return Piece::Knight;
        } else if (lowerC == 'b') {
            return Piece::Bishop;
        } else if (lowerC == 'q') {
            return Piece::Queen;
        } else if (lowerC == 'k') {
            return Piece::King;
        } else if (lowerC == 'p') {
            return Piece::Pawn;
        }
        return std::nullopt;
    }

    char getChar() const {
        switch (value) {
        case King:
            return 'K';
        default:
            return '?';
        }
    }
};

struct ColoredPiece {
    Color color;
    Piece piece;

    static std::optional<ColoredPiece> fromChar(char c) {
        const auto pieceOpt = Piece::fromChar(c);
        if (!pieceOpt) {
            return std::nullopt;
        } else {
            const char lowerC = std::tolower(c);
            const Color color = (lowerC == c) ? Color::Black : Color::White;
            return {{color, *pieceOpt}};
        }
    }
};
