#ifndef BITBOARD_H
#define BITBOARD_H

#include "defs.h"
#include <iostream>

// Platforma göre gerekli kütüphaneler
#ifdef _MSC_VER
#include <intrin.h> // Sadece Visual Studio ise bunu ekle
#endif

static inline void set_bit(Bitboard &bb, int square) {
    bb |= 1ULL << square;
}
static inline bool get_bit(Bitboard &bb, int square) {
    return (bb & (1ULL << square));
}
static inline void clear_bit(Bitboard &bb, int square) {
    bb &= ~(1ULL << square);
}

// !!! DÜZELTİLEN KISIM !!!
static inline int get_lsb_index(Bitboard bb) {
    if (bb == 0) return -1;

#ifdef _MSC_VER  // Sadece "Visual Studio" derleyicisi ise burası
    unsigned long index;
    _BitScanForward64(&index, bb);
    return (int)index;
#else            // CLion (MinGW), Linux, Mac ise burası (Daha hızlıdır)
    return __builtin_ctzll(bb);
#endif
}

void print_board();
void reset_bitboards();
void print_bitboard(Bitboard bb);

#endif