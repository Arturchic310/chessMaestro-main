#include "MainLogic/fenParser.hpp"
#include "Graphic/graphic.hpp"
#include "Board/board.hpp"
#include "MainLogic/movegen.hpp"
#include "ai.hpp"
#include<fstream>
#include "tables.hpp"
#include <bitset>
#include <cassert>
#include <chrono>

using namespace std;
using namespace chess;

int64_t all_nodes = 0;

Movelist<5000> list;

struct test_case{
    string fen;
    int shallow_d;
    int deep_d;
    
    test_case(const string &str_fen, const int d):fen(str_fen), deep_d(d), shallow_d(d - 2) {}

    void run_test(bool shallow){
        Board brd;
        fenParser parser;
        parser.parse_from_FEN(fen, brd);

        Movelist_ref list_ref(list);


        AI bot(brd, list_ref);
        
        const int depth = shallow ? shallow_d : deep_d;

        Move_full_info move_1, move_2, move_3;
        int eval_1, eval_2, eval_3;
        int64_t nodes_1, nodes_2, nodes_3, perft;
        perft = bot.perft(depth);
        tie(move_1, eval_1) = bot.best_move_negamax(depth);
        nodes_1 = bot.all_nodes;

        tie(move_2, eval_2) = bot.best_move_ab(depth);
        nodes_2 = bot.all_nodes;

        tie(move_3, eval_3) = bot.best_move_ab(depth);
        nodes_3 = bot.all_nodes;
        
        assert(move_1 == move_2);
        assert(eval_1 == eval_2);
        assert(move_1 == move_3);
        assert(eval_1 == eval_3);
        assert(nodes_1 == perft);
        assert(nodes_2 == nodes_3);
        cout << "Negamax nodes: " << nodes_1 << '\n';
        cout << "Alpha-beta nodes: " << nodes_2 << '\n';
        cout << "Second alpha-beta nodes: " << nodes_3 << "\n\n";
    }
};

int main(){
    test_case cases[] = {
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 4},
        {"4k3/8/8/pppppppp/PPPPPPPP/8/8/4K3 w - - 0 1", 4},
        {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 4},
        {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 4},
        {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 4},
        {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 4},
        {"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 4},
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", 4},
        {"k7/8/8/8/3p1p2/8/2P1P1P1/4K3 w - - 0 1", 4},
        {"8/PPP4k/8/8/8/8/ppp4K/8 w - - 0 1", 4}
    };


    cout << std::scientific;

    for(auto &i : cases){
        i.run_test(false);
        cout << "SUCCESS\n";
    }
}