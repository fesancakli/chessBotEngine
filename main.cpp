#include <iostream>
#include <string>
#include "defs.h"
#include "bitboard.h"
#include "movegen.h"

int make_move(int move, int side);

const char *square_coords[] = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
};

const char *piece_char = "PNBRQKpnbrqk";

int main() {
    reset_bitboards();
    init_leapers_attacks();
    init_pawn_attacks();

    set_bit(bitboards[R], a1); set_bit(bitboards[N], b1); set_bit(bitboards[B], c1); set_bit(bitboards[Q], d1);
    set_bit(bitboards[K], e1); set_bit(bitboards[B], f1); set_bit(bitboards[N], g1); set_bit(bitboards[R], h1);
    for (int i = a2; i <= h2; i++) set_bit(bitboards[P], i);

    set_bit(bitboards[r], a8); set_bit(bitboards[n], b8); set_bit(bitboards[b], c8); set_bit(bitboards[q], d8);
    set_bit(bitboards[k], e8); set_bit(bitboards[b], f8); set_bit(bitboards[n], g8); set_bit(bitboards[r], h8);
    for (int i = a7; i <= h7; i++) set_bit(bitboards[p], i);

    print_board();

    moves move_list[1];
    generate_moves(0, move_list);

    int legal_moves = 0;

    std::cout << "\nLegal Moves for White:\n\n";

    for (int i = 0; i < move_list->count; i++) {
        int move = move_list->moves[i];

        if (make_move(move, 0) == 0) continue;

        legal_moves++;

        int source = get_move_source(move);
        int target = get_move_target(move);
        int piece  = get_move_piece(move);
        int capture = get_move_capture(move);

        std::cout << legal_moves << ". "
                  << piece_char[piece] << "  "
                  << square_coords[source] << " -> "
                  << square_coords[target]
                  << (capture ? " +" : "")
                  << std::endl;
    }

    std::cout << "\nTotal generated: " << move_list->count << std::endl;
    std::cout << "Total legal:     " << legal_moves << std::endl;

    return 0;
}