#pragma once 
#include"Board/board.hpp"
#include "MainLogic/movegen.hpp"
#include <cassert>
namespace chess{
    class AI{
        Board &brd;
        Movelist_ref &global_list_ref;
    public:
        int64_t all_nodes;
        AI(Board &board, Movelist_ref &movelist_ref):brd(board), global_list_ref(movelist_ref){}
        
        template<Color clr>
        int negamax(int d, Movelist_ref list_ref){
            
            if(d == 0){
                ++all_nodes;
                return brd.eval<clr>();
            }
            Movegen generator(brd, list_ref);

            PositionState state = generator.gen_all_moves<clr>();
            if(list_ref.no_moves()){
                constexpr int mate = -300'000;
                //++all_nodes;

                if(state >= check)//  >= check means check or double check 
                    return mate;

                constexpr int stalemate = 0;
                return stalemate;
            }

            constexpr int inf = 1'000'000;

            int alpha = -inf;

            for (Move_full_info *i = list_ref.begin; i != list_ref.end; ++i){

                const Accumulator acc = brd.unstable_make_move<clr>(*i);
                
                int loc_eval = -negamax<change_color(clr)>(d - 1, list_ref.get_ref());
            
                brd.unstable_undo_move<clr>(*i, acc);
                
                if(loc_eval > alpha){
                    alpha = loc_eval;
                }

            }
            return alpha;
        }

        template<Color clr>
        std::tuple<Move_full_info, int> best_move_negamax(int d){
            all_nodes = 0;
            Movegen generator(brd, global_list_ref);

            PositionState state = generator.gen_all_moves<clr>();

            
            if((d == 0) || (global_list_ref.no_moves()))return {{}, 0};

            constexpr int inf = 1'000'000;

            int alpha = -inf;

            Move_full_info best_move;
            
            for (Move_full_info *i = global_list_ref.begin; i != global_list_ref.end; ++i){
                const Accumulator acc = brd.unstable_make_move<clr>(*i);
                
                int loc_eval = -negamax<change_color(clr)>(d - 1, global_list_ref.get_ref());
            
                brd.unstable_undo_move<clr>(*i, acc);
                
                if(loc_eval > alpha){
                    best_move = *i;
                    alpha = loc_eval;
                }

            }
            
            return {best_move, alpha};
        } 


        std::tuple<Move_full_info, int> best_move_negamax(int d){
            return (brd.get_turn() ? best_move_negamax<White>(d) : best_move_negamax<Black>(d));
        }


        template<Color clr>
        int q_search_ab(int alpha, int beta, Movelist_ref list_ref){
            ++all_nodes;
            int eval = brd.eval<clr>();

            if(eval >= beta)
                return beta;
            if(alpha < eval)
                alpha = eval;

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

            for (Move_full_info *i = list_ref.begin; i != list_ref.end; ++i){
                if((brd[i->to_square] == No_Piece) && (i->special != SP_en_passant))
                    continue;
                
                const Accumulator acc = brd.unstable_make_move<clr>(*i);
                
                eval = -q_search_ab<change_color(clr)>(-beta, -alpha, list_ref.get_ref());
            
                brd.unstable_undo_move<clr>(*i, acc);    

                
                if(eval >= beta){
                    return beta;
                }

                if(eval > alpha){
                    alpha = eval;
                }

            }
            return alpha;
        }

        template<Color clr>
        int negamax_ab(int d, int alpha, int beta, Movelist_ref list_ref){
            if(d == 0){
                ++all_nodes;
                //return q_search_ab<clr>(alpha, beta, list_ref);
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

            for (Move_full_info *i = list_ref.begin; i != list_ref.end; ++i){
                const Accumulator acc = brd.unstable_make_move<clr>(*i);
                
                const int loc_eval = -negamax_ab<change_color(clr)>(d - 1, -beta, -alpha, list_ref.get_ref());
            
                brd.unstable_undo_move<clr>(*i, acc);    

                
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
        std::tuple<Move_full_info, int> best_move_ab(int d){
            all_nodes = 0;
            Movegen generator(brd, global_list_ref);

            PositionState state = generator.gen_all_moves<clr>();

            //brd.sort_moves<clr>(list_ref);
            
            if((d == 0) || (global_list_ref.no_moves()))return {{}, 0};

            constexpr int inf = 1'000'000'000;

            int alpha = -inf;
            //int beta = inf;

            Move_full_info best_move;
            
            for (Move_full_info *i = global_list_ref.begin; i != global_list_ref.end; ++i){

                const Accumulator acc = brd.unstable_make_move<clr>(*i);
                
                int loc_eval = -negamax_ab<change_color(clr)>(d - 1, -inf, -alpha, global_list_ref.get_ref());
            
                brd.unstable_undo_move<clr>(*i, acc);
                
                if(loc_eval > alpha){
                    best_move = *i;
                    alpha = loc_eval;
                }

            }
            
            return {best_move, alpha};
        }

        std::tuple<Move_full_info, int> best_move_ab(int d){
            return (brd.get_turn() ? best_move_ab<White>(d) : best_move_ab<Black>(d));
        }

        template<Color color>
        int64_t perft(int d, Movelist_ref list_ref){
            if(d == 0)
                return 1;
            
            Movegen generator(brd, list_ref);

            PositionState state = generator.gen_all_moves<color>();
            
            int nodes = 0;
            for (Move_full_info *i = list_ref.begin; i != list_ref.end; ++i){
                const Accumulator acc = brd.unstable_make_move<color>(*i);
                
                nodes += perft<change_color(color)>(d - 1, list_ref.get_ref());
                
                brd.unstable_undo_move<color>(*i, acc);
            }
            return nodes;
        }
        int64_t perft(int d){
            return (brd.get_turn() ? 
            perft<White>(d, global_list_ref) : 
            perft<Black>(d, global_list_ref));
        }


        template<Color color>
        int64_t hashtest(int d, Movelist_ref list_ref, u64 hash){
            assert(hash == brd.get_hash());
            if(d == 0)
                return 1;
            
            Movegen generator(brd, list_ref);

            PositionState state = generator.gen_all_moves<color>();
            
            int nodes = 0;
            for (Move_full_info *i = list_ref.begin; i != list_ref.end; ++i){
                const u64 new_hash = brd.get_hash<color>(hash, *i);
                const Accumulator acc = brd.unstable_make_move<color>(*i);
                
                nodes += hashtest<change_color(color)>(d - 1, list_ref.get_ref(), new_hash);
                
                brd.unstable_undo_move<color>(*i, acc);
            }
            return nodes;
        }


        int64_t run_hashtest(int d){
            return (brd.get_turn() ? 
                hashtest<White>(d, global_list_ref, brd.get_hash()) : 
                hashtest<Black>(d, global_list_ref, brd.get_hash()));
        }
    };
}
