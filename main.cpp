#include <iostream>
#include <string>
#include "defs.h"
#include "bitboard.h"
#include "movegen.h"

int make_move(int move, int side);

int main() {
    reset_bitboards();
    init_leapers_attacks();
    init_pawn_attacks();


    return 0;
}