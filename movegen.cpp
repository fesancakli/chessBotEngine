// movegen.cpp
#include "movegen.h"
#include "bitboard.h"
#include <iostream>
#include <cstring>

// Global saldırı tablolarının tanımları
Bitboard knight_attacks[64];
Bitboard king_attacks[64];
Bitboard pawn_attacks[2][64];

void init_leapers_attacks() {
    int knight_rows[8] = {2, 2, 1, 1, -1, -1, -2, -2};
    int knight_cols[8] = {-1, 1, -2, 2, -2, 2, -1, 1};
    int king_rows[8] = {-1, -1, -1, 1, 1, 1, 0, 0};
    int king_cols[8] = {1, -1, 0, 1, -1, 0, 1, -1};

    for (int square = 0; square < 64; square++) {
        int row = square / 8;
        int col = square % 8;

        knight_attacks[square] = 0ULL;
        for (int i = 0; i < 8; i++) {
            int target_row = row + knight_rows[i];
            int target_col = col + knight_cols[i];
            if (target_row>=0 && target_col>=0 && target_row<8 && target_col<8) {
                int target_square = 8*target_row + target_col;
                set_bit(knight_attacks[square], target_square);
            }
        }
        for (int i = 0; i < 8; i++) {
            int target_row = row + king_rows[i];
            int target_col = col + king_cols[i];
            if (target_row>=0 && target_col>=0 && target_row<8 && target_col<8) {
                int target_square = 8*target_row + target_col;
                set_bit(king_attacks[square], target_square);
            }
        }
    }
}

Bitboard get_bishop_attacks(int square , Bitboard blockers) {
    Bitboard attacks = 0ULL;
    int  row = square / 8;
    int  col = square % 8;
    int target_r , target_c;

    // sağ yukarı
    for (target_r = row+1 , target_c = col+1; target_r <8 && target_c < 8; target_r++, target_c++) {
        attacks |= (1ULL << (target_r * 8 + target_c));
        if ((1ULL << (target_r * 8 + target_c)) & blockers) break;
    }
    // sağ aşağı
    for (target_r = row-1 , target_c = col+1; target_r >=0 && target_c < 8; target_r--, target_c++) {
        attacks |= (1ULL << (target_r * 8 + target_c));
        if ((1ULL << (target_r * 8 + target_c)) & blockers) break;
    }
    // sol yukarı
    for (target_r = row+1 , target_c = col-1; target_r <8 && target_c >=0; target_r++, target_c--) {
        attacks |= (1ULL << (target_r * 8 + target_c));
        if ((1ULL << (target_r * 8 + target_c)) & blockers) break;
    }
    // sol aşşa
    for (target_r = row-1 , target_c = col-1; target_r >=0 && target_c >=0; target_r--, target_c--) {
        attacks |= (1ULL << (target_r * 8 + target_c));
        if ((1ULL << (target_r * 8 + target_c)) & blockers) break;
    }
    return attacks;
}

Bitboard get_rook_attacks(int square, Bitboard blockers) {
    Bitboard attacks = 0ULL;
    int  row = square / 8;
    int  col = square % 8;
    int target_r ;
    int target_c;

    //yukarı
    for (target_r = row+1 ; target_r < 8 ; target_r++) {
        attacks |= (1ULL << (target_r * 8 + col));
        if ((1ULL << (target_r * 8 + col)) & blockers) break;
    }
    // aşşa
    for (target_r = row-1 ; target_r >=0 ; target_r--) {
        attacks |= (1ULL << (target_r * 8 + col));
        if ((1ULL << (target_r * 8 + col)) & blockers) break;
    }
    // sağa
    for (target_c = col+1 ; target_c <8 ; target_c++) {
        attacks |= (1ULL << (row * 8 + target_c));
        if ((1ULL << (row * 8 + target_c)) & blockers) break;
    }
    //sola
    for (target_c = col-1 ; target_c >= 0 ; target_c--) {
        attacks |= (1ULL << (row * 8 + target_c));
        if ((1ULL << (row * 8 + target_c)) & blockers) break;
    }
    return attacks;
}

Bitboard get_queen_attacks(int square, Bitboard blockers) {
    return get_rook_attacks(square, blockers) | get_bishop_attacks(square, blockers);
}

void init_pawn_attacks() {
    for (int square = 0; square < 64; square++) {
        pawn_attacks[0][square] = 0ULL;
        pawn_attacks[1][square] = 0ULL;

        int col = square % 8;
        int row = square / 8;

        if (row < 7) {
          if (col < 7)   pawn_attacks[0][square] |= (1ULL << (square+9));
          if (col > 0)  pawn_attacks[0][square] |= (1ULL << (square+7));
        }
        if (row > 0) {
            if (col > 0)   pawn_attacks[1][square] |= (1ULL << (square-9));
            if (col < 7)  pawn_attacks[1][square] |= (1ULL << (square-7));
        }
    }
}

Bitboard get_pawn_moves(int square, bool side, Bitboard blockers) {
    Bitboard moves = 0ULL;
    int  row = square / 8;
    if (!side) {
         int target = square+8;
        if (target < 64 && 0 == get_bit(blockers , target)) {
            moves |= (1ULL << (target));
            if (row == 1) {
                int double_target = square+16;
                if (double_target < 64 && 0 == get_bit(blockers , double_target)) moves |= (1ULL << (double_target));
            }
        }
    }
    else {
        int target = square-8;
        if (target >= 0 && 0 == get_bit(blockers , target)) {
            moves |= (1ULL << (target));
            if (row == 6) {
                int double_target = square-16;
                if (double_target < 64 && 0 == get_bit(blockers , double_target)) moves |= (1ULL << (double_target));
            }
        }
    }
    return moves;
}


int is_square_attacked(int square, int side) {

    if ((side == 0) && (pawn_attacks[1][square] & bitboards[P])) return 1;
    if ((side == 1) && (pawn_attacks[0][square] & bitboards[p])) return 1;


    if (knight_attacks[square] & ((side == 0) ? bitboards[N] : bitboards[n])) return 1;
    if (king_attacks[square] & ((side == 0) ? bitboards[K] : bitboards[k])) return 1;

    Bitboard occupancies = 0ULL;

    for (int i = P; i <= k; i++) occupancies |= bitboards[i];

    Bitboard bishop_attacks = get_bishop_attacks(square, occupancies);

    if (bishop_attacks & ((side == 0) ? bitboards[B] : bitboards[b])) return 1;
    if (bishop_attacks & ((side == 0) ? bitboards[Q] : bitboards[q])) return 1;

    Bitboard rook_attacks = get_rook_attacks(square, occupancies);

    if (rook_attacks & ((side == 0) ? bitboards[R] : bitboards[r])) return 1;
    if (rook_attacks & ((side == 0) ? bitboards[Q] : bitboards[q])) return 1;

    return 0;

}
void generate_moves(int side, moves *move_list) {
    move_list->count = 0;
    Bitboard occupancies[3];

    occupancies[0] = 0ULL;
    occupancies[1] = 0ULL;
    occupancies[2] = 0ULL;

    for (int i = P ;i<=K;i++) occupancies[0] |= bitboards[i];
    for (int i = p ;i<=k;i++) occupancies[1] |= bitboards[i];

    occupancies[2] = occupancies[0] | occupancies[1];
    Bitboard my_pieces , enemies;
    if (side == 0) {
        my_pieces = occupancies[0];
        enemies = occupancies[1];
    }
    else {
         my_pieces = occupancies[1];
         enemies = occupancies[0];
    }
    Bitboard attacks;
    int piece = (side==0) ? N : n;
    Bitboard copy_board = bitboards[piece];
    while (copy_board) {
        int source_index = get_lsb_index(copy_board);
        attacks = knight_attacks[source_index];
        attacks &= ~my_pieces;
        while (attacks) {
            int target_index = get_lsb_index(attacks);
            int captured = get_bit(enemies, target_index) ? 1 : 0;
            int move = encode_move(source_index, target_index, piece, captured, 0, 0, 0);
            move_list->moves[move_list->count] = move;
            move_list->count++;
            clear_bit(attacks , target_index);
        }
        clear_bit(copy_board,source_index);
    }
    piece = (side==0) ? K : k;
    copy_board = bitboards[piece];
    while (copy_board) {
        int source_index = get_lsb_index(copy_board);
        attacks = king_attacks[source_index];
        attacks &= ~my_pieces;
        while (attacks) {
            int target_index = get_lsb_index(attacks);
            int captured = get_bit(enemies, target_index) ? 1 : 0;
            int move = encode_move(source_index, target_index, piece, captured, 0, 0, 0);
            move_list->moves[move_list->count] = move;
            move_list->count++;
            clear_bit(attacks , target_index);
        }
        clear_bit(copy_board,source_index);
    }
    piece = (side==0) ? P : p;
    copy_board = bitboards[piece];
    while (copy_board) {
        int source_index = get_lsb_index(copy_board);
        Bitboard moves = get_pawn_moves(source_index,side,occupancies[2]);
        while (moves) {
            int target_index = get_lsb_index(moves);
            int double_push = (abs(target_index - source_index) == 16) ? 1 : 0;
            int move = encode_move(source_index, target_index, piece, 0, double_push, 0, 0);
            move_list->moves[move_list->count] = move;
            move_list->count++;
            clear_bit(moves, target_index);
        }
        attacks = pawn_attacks[side][source_index];
        attacks &= enemies;
        while (attacks) {
            int target_index = get_lsb_index(attacks);
            int captured = 1;
            int move = encode_move(source_index, target_index, piece, captured, 0, 0, 0);
            move_list->moves[move_list->count] = move;
            move_list->count++;
            clear_bit(attacks , target_index);
        }

        clear_bit(copy_board, source_index);

    }
    piece = (side==0) ? Q : q;
    copy_board = bitboards[piece];
    while (copy_board) {
        int source_index = get_lsb_index(copy_board);
        attacks = get_queen_attacks(source_index,occupancies[2]);
        attacks &= ~my_pieces;
        while (attacks) {
            int target_index = get_lsb_index(attacks);
            int captured = get_bit(enemies, target_index) ? 1 : 0;
            int move = encode_move(source_index, target_index, piece, captured, 0, 0, 0);
            move_list->moves[move_list->count] = move;
            move_list->count++;
            clear_bit(attacks , target_index);
        }
        clear_bit(copy_board, source_index);
    }
    piece = (side==0) ? B : b;
    copy_board = bitboards[piece];
    while (copy_board) {
        int source_index = get_lsb_index(copy_board);
        attacks = get_bishop_attacks(source_index,occupancies[2]);
        attacks &= ~my_pieces;
        while (attacks) {
            int target_index = get_lsb_index(attacks);
            int captured = get_bit(enemies, target_index) ? 1 : 0;
            int move = encode_move(source_index, target_index, piece, captured, 0, 0, 0);
            move_list->moves[move_list->count] = move;
            move_list->count++;
            clear_bit(attacks , target_index);
        }
        clear_bit(copy_board, source_index);
    }
    piece = (side==0) ? R : r;
    copy_board = bitboards[piece];
    while (copy_board) {
        int source_index = get_lsb_index(copy_board);
        attacks = get_rook_attacks(source_index,occupancies[2]);
        attacks &= ~my_pieces;
        while (attacks) {
            int target_index = get_lsb_index(attacks);
            int captured = get_bit(enemies, target_index) ? 1 : 0;
            int move = encode_move(source_index, target_index, piece, captured, 0, 0, 0);
            move_list->moves[move_list->count] = move;
            move_list->count++;
            clear_bit(attacks , target_index);
        }
        clear_bit(copy_board, source_index);
    }
}

int make_move(int move , int side) {
    Bitboard bitboards_copy[12];
    memcpy(bitboards_copy, bitboards, 96);


    int source_index = get_move_source(move);
    int target_index = get_move_target(move);
    int captured = get_move_capture(move);
    int piece = get_move_piece(move);

    clear_bit(bitboards[piece], source_index);
    set_bit(bitboards[piece], target_index);

    if (captured) {
        int start_piece, end_piece;
        if (side == 0) {
            start_piece = p; end_piece = k;
        } else {
            start_piece = P; end_piece = K;
        }

        for (int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++) {
            if (get_bit(bitboards[bb_piece], target_index)) {
                clear_bit(bitboards[bb_piece], target_index);
                break;
            }
        }
    }

    int king_square = get_lsb_index(bitboards[(side == 0) ? K : k]);

    if (is_square_attacked(king_square, side ^ 1)) {
        memcpy(bitboards, bitboards_copy, 96);
        return 0;
    }

    memcpy(bitboards, bitboards_copy, 96);

    return 1;
}