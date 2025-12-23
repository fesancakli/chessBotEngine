#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "defs.h"

void init_leapers_attacks();
void init_pawn_attacks();
void generate_moves(int side, moves *move_list);
int is_square_attacked(int square, int side);

#endif