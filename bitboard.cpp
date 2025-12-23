#include "bitboard.h"
#include <iostream>

// Global değişkenin GERÇEK tanımı buradadır
Bitboard bitboards[12];

void reset_bitboards() {
    for (int i = 0; i < 12; i++) {
        bitboards[i] = 0ULL;
    }
}

void print_board() {
    for (int row = 7; row >= 0; row--) {
        std::cout << row + 1 << "  ";
        for (int col = 0; col < 8; col++) {
            int piece_number = -1;
            int square = row * 8 + col;
            for (int i = 0; i < 12; i++) {
                if (get_bit(bitboards[i], square)) {
                    piece_number = i;
                    break;
                }
            }
            char ascii_pieces[] = "PNBRQKpnbrqk";
            if (piece_number != -1)
                std::cout << " " << ascii_pieces[piece_number] << " ";
            else
                std::cout << " . ";
        }
        std::cout << "\n";
    }
    std::cout << "\n    a  b  c  d  e  f  g  h \n\n";
}

void print_bitboard(Bitboard bb) {
    std::cout << "\n";
    for (int rank = 7; rank >= 0; rank--) {
        std::cout << rank + 1 << "  ";
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            if (get_bit(bb, square)) std::cout << " X ";
            else std::cout << " . ";
        }
        std::cout << "\n";
    }
    std::cout << "\n    a  b  c  d  e  f  g  h \n\n";
    std::cout << "    Bitboard Sayisal Degeri: " << bb << "\n\n";
}