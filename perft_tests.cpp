#include "MainLogic/fenParser.hpp"
#include "Graphic/graphic.hpp"
#include "Board/board.hpp"
#include "MainLogic/movegen.hpp"
#include "tables.hpp"
#include <string>
#include <cassert>
#include <vector>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace chess;


Movelist<3000> list;
int64_t all_nodes = 0;

template<Color color>
int perft(Board &brd, int d, Movelist_ref list_ref = Movelist_ref(list)){
    if(d == 0){
       // Movegen generator(brd, list_ref);
        //PositionState state = generator.gen_all_moves<color>();
        return 1;
    }
    
    Movegen generator(brd, list_ref);

    PositionState state = generator.gen_all_moves<color>();
    
    int nodes = 0;
    for (Move_full_info *i = list_ref.begin; i < list_ref.end; ++i){
        const Accumulator acc = brd.get_accumulator();
        const Piece piece = brd[i->to_square];
        brd.unstable_make_move<color>(*i);
        const int local_nodes = perft<change_color(color)>(brd, d - 1, list_ref.get_ref());
        nodes += local_nodes;
        brd.unstable_undo_move<color>(*i, piece);
        brd.restore_info(acc);
    }
    return nodes;
}

struct test_case{
    string fen;
    vector<int> results;
    int shallow_d;
    int deep_d;
    test_case(const string &str_fen, const vector<int> &perft_results):fen(str_fen), results(perft_results){
        deep_d = results.size();
        shallow_d = deep_d - 1;
        while(results[shallow_d] > 1'000'000)--shallow_d;
    }

    int run_test(bool shallow){
        int all_nodes_loc = 0;
        const int d = shallow ? shallow_d : deep_d;
        Board brd;
        fenParser parser;
        parser.parse_from_FEN(fen, brd);
        for(int i = 0; i < d; ++i){
            const Board clone(brd);
            const int nodes = (brd.get_turn() ? perft<White>(brd, i) : perft<Black>(brd, i));
            all_nodes += nodes;
            all_nodes_loc += nodes;
            if(clone != brd){
                cout << "ERRORERRORERROR    " << fen << " depth: " << i << "Board is changed\n"; 
            }
            assert(clone == brd);
            if(nodes != results[i]){
                cout << "ERRORERRORERROR    " << fen << " depth: " << i << " Expected: " << results[i] << " given: " << nodes << "\n"; 
            } 
            assert(nodes == results[i]);
            cout << fen << " depth: " << i << " SUCCESS\n";
        }
        return all_nodes_loc;
    }
};



int main(){
    test_case cases[] = {
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", {1, 20, 400, 8902, 197281, 4865609, 119060324}},
        {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", {1, 48, 2039, 97862, 4085603, 193690690}},
        {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", {1, 14, 191, 2812, 43238, 674624, 11030083, 178633661}},
        {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", {1, 6, 264, 9467, 422333, 15833292, 706045033}},
        {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", {1, 44, 1486, 62379, 2103487, 89941194}},
        {"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", {1, 46, 2079, 89890, 3894594, 164075551}},
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", {1, 26, 568, 13744, 314346, 7594526}},
        {"k7/8/8/8/3p1p2/8/2P1P1P1/4K3 w - - 0 1", {1, 10, 58, 572, 4310, 39995, 332995, 3006689, 26192730}},
        {"8/PPP4k/8/8/8/8/ppp4K/8 w - - 0 1", {1, 17, 277, 5156, 92652, 1931144, 38426019}}
    };
    cout << std::scientific;
    auto start = std::chrono::system_clock::now();
    for(auto &i : cases){
        auto loc_start = std::chrono::system_clock::now();
        int all_nodes_loc = i.run_test(true);
        auto loc_end = std::chrono::system_clock::now();
        auto elapsed = loc_end - loc_start;
        cout << (all_nodes_loc / (std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() * 1e-6) ) << "nps\n"; 
    }
    auto end = std::chrono::system_clock::now();
    auto elapsed = end - start;
    cout << (all_nodes / (std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() * 1e-6) ) << "nps\n"; 
    //3.166122e+07nps
    //3.270218e+07nps
    //3.372886e+07nps
    //3.449790e+07nps 
}