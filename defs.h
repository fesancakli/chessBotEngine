#ifndef DEFS_H
#define DEFS_H

#include <cstdint>

typedef uint64_t Bitboard;

#define encode_move(source, target, piece, capture, double_push, enpassant, castling) \
(source) | (target << 6) | (piece << 12) | (capture << 16) | (double_push << 17) | (enpassant << 18) | (castling << 19)

#define get_move_source(move) (move & 0x3f)
#define get_move_target(move) ((move & 0xfc0) >> 6)
#define get_move_piece(move) ((move & 0xf000) >> 12)
#define get_move_capture(move) (move & 0x10000)


enum { P, N, B, R, Q, K, p, n, b, r, q, k };

enum {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};


typedef struct {
    int moves[256];
    int count;
} moves;


extern Bitboard bitboards[12];
extern Bitboard knight_attacks[64];
extern Bitboard king_attacks[64];
extern Bitboard pawn_attacks[2][64];
extern int enpassant_square;


#define copy_board() \
Bitboard bitboards_copy[12]; \
memcpy(bitboards_copy, bitboards, 96); \
int side_copy = side;

#define take_back() \
memcpy(bitboards, bitboards_copy, 96); \
side = side_copy;
#endif