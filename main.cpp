#include <iostream>
#include <string>
#include "defs.h"
#include "bitboard.h"
#include "movegen.h"

// make_move fonksiyonunu kullanacağımız için burada bildirmemiz gerekebilir
// (Eğer defs.h içine eklediysen gerek yok ama garanti olsun diye ekliyorum)
int make_move(int move, int side);

int main() {
    // 1. Gerekli Hazırlıklar
    reset_bitboards();
    init_leapers_attacks();
    init_pawn_attacks();

    // 2. Test Pozisyonunu Ayarla
    // Beyaz Şah e1'de, Siyah Kale e8'de (Şah çekiyor!)
    // Bu durumda şah sadece kaçmalı veya önü kapatılmalı.
    set_bit(bitboards[K], e1); // Beyaz Şah
    set_bit(bitboards[Q], d1); // Beyaz Vezir
    set_bit(bitboards[R], h1); // Beyaz Kale
    //set_bit(bitboards[P], e2); // Beyaz Piyon

    set_bit(bitboards[r], e8); // Siyah Kale (Şah çekiyor!)
    set_bit(bitboards[k], h8); // Siyah Şah

    std::cout << "--- BASLANGIC KONUMU ---" << std::endl;
    print_board();

    // 3. Hamleleri Üret (Pseudo-Legal: Yarı geçerli)
    moves move_list[1];
    generate_moves(0, move_list); // 0 = Beyaz oynuyor

    // 4. Hamleleri Kontrol Et ve Ekrana Bas
    std::cout << "\n--- GECERLI (LEGAL) HAMLELER ---" << std::endl;
    int legal_moves_count = 0;

    for (int i = 0; i < move_list->count; i++) {
        int move = move_list->moves[i];

        // !!! KRİTİK NOKTA !!!
        // Hamleyi yapmayı dene.
        // Eğer make_move 0 dönerse, Şah tehdit altında kalıyor demektir.
        // Bu yüzden o hamleyi LİSTEYE ALMA (continue de).
        if (make_move(move, 0) == 0) {
            continue;
        }

        // Eğer buraya geldiyse hamle yasaldır.
        legal_moves_count++;

        int kaynak = get_move_source(move);
        int hedef  = get_move_target(move);
        int tas    = get_move_piece(move);
        int yeme   = get_move_capture(move);

        std::string tas_ismi;
        switch(tas) {
            case P: tas_ismi = "Piyon"; break;
            case N: tas_ismi = "At   "; break;
            case B: tas_ismi = "Fil  "; break;
            case R: tas_ismi = "Kale "; break;
            case Q: tas_ismi = "Vezir"; break;
            case K: tas_ismi = "Sah  "; break;
            default: tas_ismi = "???  "; break;
        }

        // Kare isimlerini (e1, e2 gibi) yazdırmak için küçük bir dizi
        const char *kare_isimleri[] = {
            "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
            "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
            "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
            "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
            "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
            "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
            "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
            "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
        };

        std::cout << legal_moves_count << ". " << tas_ismi << " : "
                  << kare_isimleri[kaynak] << " -> " << kare_isimleri[hedef]
                  << (yeme ? " [YEME!]" : "")
                  << std::endl;
    }

    std::cout << "\n---------------------------------" << std::endl;
    std::cout << "Uretilen Toplam Hamle: " << move_list->count << std::endl;
    std::cout << "Yasal (Legal) Hamle  : " << legal_moves_count << std::endl;

    return 0;
}