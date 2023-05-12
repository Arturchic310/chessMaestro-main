#include "MainLogic/fenParser.hpp"
#include "Graphic/graphic.hpp"
#include "Board/board.hpp"
#include "MainLogic/movegen.hpp"
#include<fstream>
#include "tables.hpp"
#include <bitset>
#include <cassert>
#include <chrono>

using namespace std;
using namespace chess;

int64_t all_nodes = 0;

Movelist<5000> list;

template<Color clr>
int q_search(Board &brd, Movelist_ref list_ref){
    ++all_nodes;

    Movegen generator(brd, list_ref);

    PositionState state = generator.gen_all_moves<clr>();
    if(list_ref.no_moves()){
        constexpr int mate = -300'000;
        
        if(state >= check)
            return mate;

        constexpr int stalemate = 0;
        return stalemate;
    }

    constexpr int inf = 1'000'000;

    bool no_moves = true;
    int alpha = -inf;

    for (Move_full_info *i = list_ref.begin; i < list_ref.end; ++i){
        if((brd[i->to_square] == No_Piece) && (i->special != SP_en_passant))
            continue;

        no_moves = false;
        const Accumulator acc = brd.get_accumulator();
        const Piece piece = brd[i->to_square];
        brd.unstable_make_move<clr>(*i);
        
        int loc_eval = -q_search<change_color(clr)>(brd, list_ref.get_ref());
       
        brd.unstable_undo_move<clr>(*i, piece);
        brd.restore_info(acc);
        
        if(loc_eval > alpha){
            alpha = loc_eval;
        }
    }
    if(no_moves){
        return brd.eval<clr>();
    }
    return alpha;
}

template<Color clr>
int negamax(Board &brd, int d, Movelist_ref list_ref){
    //++all_nodes;
    if(d == 0){
       ++all_nodes;
        return brd.eval<clr>();
    }
    Movegen generator(brd, list_ref);

    PositionState state = generator.gen_all_moves<clr>();
    if(list_ref.no_moves()){
        constexpr int mate = -300'000;
        
        ++all_nodes;
        if(state >= check)
            return mate;

        constexpr int stalemate = 0;
        return stalemate;
    }

    constexpr int inf = 1'000'000;

    int alpha = -inf;

    for (Move_full_info *i = list_ref.begin; i < list_ref.end; ++i){

        const Accumulator acc = brd.get_accumulator();
        const Piece piece = brd[i->to_square];
        brd.unstable_make_move<clr>(*i);
        
        int loc_eval = -negamax<change_color(clr)>(brd, d - 1, list_ref.get_ref());
       
        brd.unstable_undo_move<clr>(*i, piece);
        brd.restore_info(acc);
        
        if(loc_eval > alpha){
            alpha = loc_eval;
        }

    }
    return alpha;

}

template<Color clr>
tuple<Move_full_info, int> best_move_minmax(Board &brd, int d){

    Movelist_ref list_ref(list);
    Movegen generator(brd, list_ref);

    PositionState state = generator.gen_all_moves<clr>();

    
    if((d == 0) || (list_ref.no_moves()))return {{}, 0};

    constexpr int inf = 1'000'000'000;

    int alpha = -inf;

    Move_full_info best_move;
    
    for (Move_full_info *i = list_ref.begin; i < list_ref.end; ++i){

        const Accumulator acc = brd.get_accumulator();
        const Piece piece = brd[i->to_square];
        brd.unstable_make_move<clr>(*i);
        
        int loc_eval = -negamax<change_color(clr)>(brd, d - 1, list_ref.get_ref());
        
        brd.unstable_undo_move<clr>(*i, piece);
        brd.restore_info(acc);
        
        if(loc_eval > alpha){
            best_move = *i;
            alpha = loc_eval;
        }

    }
    
    return {best_move, alpha};
}



template<Color clr>
int q_search_ab(Board &brd, int alpha, int beta, Movelist_ref list_ref){
    const int momentum_eval = brd.eval<clr>();

    if(momentum_eval >= beta)
        return beta;
    if(momentum_eval > alpha)
        alpha = momentum_eval;
    
    ++all_nodes;

    Movegen generator(brd, list_ref);

    PositionState state = generator.gen_all_moves<clr>();

    


    if(list_ref.no_moves()){
        constexpr int mate = -300'000;
        
        if(state >= check)
            return mate;

        constexpr int stalemate = 0;
        return stalemate;
    }


    bool no_moves = true;

    for (Move_full_info *i = list_ref.begin; i < list_ref.end; ++i){
        if((brd[i->to_square] == No_Piece) && (i->special != SP_en_passant))
            continue;

        no_moves = false;
        const Accumulator acc = brd.get_accumulator();
        const Piece piece = brd[i->to_square];
        brd.unstable_make_move<clr>(*i);
        
        int loc_eval = -q_search_ab<change_color(clr)>(brd, -beta, -alpha, list_ref.get_ref());
       
        brd.unstable_undo_move<clr>(*i, piece);
        brd.restore_info(acc);
        
        if(loc_eval >= beta){
            return beta;
        }

        if(loc_eval > alpha){
            alpha = loc_eval;
        }
    }
    if(no_moves){
        return brd.eval<clr>();
    }
    return alpha;
}

template<Color clr>
int negamax_ab(Board &brd, int d, int alpha, int beta, Movelist_ref list_ref = Movelist_ref(list)){
    
    if(d == 0){
        ++all_nodes;
        return brd.eval<clr>();
    }
    Movegen generator(brd, list_ref);

    PositionState state = generator.gen_all_moves<clr>();

    brd.sort_moves<clr>(list_ref);

    if(list_ref.no_moves()){
        constexpr int mate = -300'000;
        ++all_nodes;
        if(state >= check)
            return mate;

        constexpr int stalemate = 0;
        return stalemate;
    }

    for (Move_full_info *i = list_ref.begin; i < list_ref.end; ++i){

        const Accumulator acc = brd.get_accumulator();
        const Piece piece = brd[i->to_square];
        brd.unstable_make_move<clr>(*i);
        
        const int loc_eval = -negamax_ab<change_color(clr)>(brd, d - 1, -beta, -alpha, list_ref.get_ref());
       
        brd.unstable_undo_move<clr>(*i, piece);
        brd.restore_info(acc);
        
        if(loc_eval >= beta){
            return beta;
        }

        if(loc_eval > alpha){
            alpha = loc_eval;
        }

    }
    return alpha;

}

template<Color clr>
tuple<Move_full_info, int> best_move_ab(Board &brd, int d){
    Movelist_ref list_ref(list);
    Movegen generator(brd, list_ref);

    PositionState state = generator.gen_all_moves<clr>();

    //brd.sort_moves<clr>(list_ref);
    
    if((d == 0) || (list_ref.no_moves()))return {{}, 0};

    constexpr int inf = 1'000'000'000;

    int alpha = -inf;
    //int beta = inf;

    Move_full_info best_move;
    
    for (Move_full_info *i = list_ref.begin; i < list_ref.end; ++i){

        const Accumulator acc = brd.get_accumulator();
        const Piece piece = brd[i->to_square];
        brd.unstable_make_move<clr>(*i);
        
        int loc_eval = -negamax_ab<change_color(clr)>(brd, d - 1, -inf, -alpha, list_ref.get_ref());
        
        brd.unstable_undo_move<clr>(*i, piece);
        brd.restore_info(acc);
        
        if(loc_eval > alpha){
            best_move = *i;
            alpha = loc_eval;
        }

    }
    
    return {best_move, alpha};
}



struct test_case{
    string fen;
    int shallow_d;
    int deep_d;
    
    test_case(const string &str_fen, const int d):fen(str_fen), deep_d(d), shallow_d(d - 2) {}

    void run_test(bool shallow){
        Board brd;
        fenParser parser;
        parser.parse_from_FEN(fen, brd);

        AI<5000> bot(brd, list);
        
        const int depth = shallow ? shallow_d : deep_d;

        Move_full_info move;
        int eval;
        all_nodes = 0;
        auto loc_start = std::chrono::system_clock::now();
        //tie(move, eval) = brd.get_turn() ? best_move_minmax<White>(brd, depth) : best_move_minmax<Black>(brd, depth);
        auto loc_end = std::chrono::system_clock::now();
        auto elapsed = loc_end - loc_start;
        cout << "minimax: " << (std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() * 1e-6) << " all nodes: " << all_nodes << '\n';
        Move_full_info move1;
        int eval1;

        all_nodes = 0;

        auto loc_start1 = std::chrono::system_clock::now();
        tie(move1, eval1) = brd.get_turn() ? best_move_ab<White>(brd, depth) : best_move_ab<Black>(brd, depth);
        auto loc_end1 = std::chrono::system_clock::now();
        auto elapsed1 = loc_end1 - loc_start1;
        cout << square_to_str(static_cast<Square>(move1.from_square)) << " : " << square_to_str(static_cast<Square>(move1.to_square)) << "\n";
        cout << fen << " alpha-beta: " << (std::chrono::duration_cast<std::chrono::microseconds>(elapsed1).count() * 1e-6) << " all nodes: " << all_nodes << '\n';
        
        
        //assert(move == move1);
        //assert(eval == eval1);

    }
};

int main(){
    test_case cases[] = {
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 4},
        {"4k3/8/8/pppppppp/PPPPPPPP/8/8/4K3 w - - 0 1", 3},
        {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 3},
        {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 3},
        {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 3},
        {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 3},
        {"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 3},
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", 3},
        {"k7/8/8/8/3p1p2/8/2P1P1P1/4K3 w - - 0 1", 3},
        {"8/PPP4k/8/8/8/8/ppp4K/8 w - - 0 1", 3}
    };


    cout << std::scientific;
    Board brd;
    fenParser fen;

    fen.parse_from_FEN("r3k2r/p1ppqpb1/Bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPB1PPP/R3K2R b KQkq - 0 1", brd);
    all_nodes = 0; 
    Move_full_info move = get<0>(best_move_ab<Black>(brd, 4));
    cout << square_to_str(static_cast<Square>(move.from_square)) << " : " << square_to_str(static_cast<Square>(move.to_square)) << " " << all_nodes << " " << "\n";
    return 0;
    cout << q_search_ab<White>(brd, -100'000'000, 100'000'000, Movelist_ref(list)) << " " << all_nodes;
    //return 0;
    for(auto &i : cases){
        i.run_test(true);
        cout << "SUCCESS\n";
    }
}