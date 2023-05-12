#include <chrono>
#include <iostream>
// #define MAESTRO_DEBUG
#define CLEAR std::cout << "\x1B[2J\x1B[H";
#define ERROR std::cout << "\nERROR\n";
#include "MainLogic/fenParser.hpp"
#include "MainLogic/PlayerVPlayer.hpp"
#include "Graphic/graphic.hpp"
#include "Board/board.hpp"
#include "MainLogic/movegen.hpp"
#include <fstream>
#include "ai.hpp"
#include "tables.hpp"
#include <bitset>
#include <cassert>

using std::string;

using std::cout, std::cin, std::tuple;
using namespace chess;

const string START_POS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const string EXAMPLE_POS_1 = "rnb1kbnr/pppppppp/8/3q4/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1";
const string EXAPMLE_POS_2 = "r1bqkbnr/pp1ppp1p/2n3p1/8/3NP3/8/PPP2PPP/RNBQKB1R w KQkq - 0 1";
const string EXAMPLE_POS_3 = "rnb2bnr/p1pk1ppp/4q3/1p1pp3/3P1BP1/1PP5/P3PP1P/RN1QKBNR b KQha - 0 1";
const string EXPEREMENTAL = "rnbqkbnr/pppp1ppp/4p3/8/3P4/5N2/PPP1PPPP/RNBQKB1R b KQkq - 0 2";
const int depth = 5;

const string test1{"8/4n3/8/1k3p2/3K4/7r/7q/8 w - - 0 1"};
const string test2{"8/4n3/1k6/5p2/3K4/8/7q/8 w - - 0 1"};
const string test3{"8/4n3/1k6/5p2/3K4/7r/7q/8 w - - 0 1"};
const string test4{"8/8/8/8/8/7k/7p/7K w - - 0 1"};
const string test5{"k7/8/8/8/8/3pp3/3p4/3K4 w - - 0 1"};
const string test6{"k7/8/8/8/8/3p4/3p4/3K4 w - - 0 1"};
const string test7{"8/8/4kk2/8/2k1K3/8/4k3/8 w - - 0 1"};

const string test11{"3r4/8/8/8/8/2Q5/8/3K4 w - - 0 1"};
const string test12{"8/8/8/8/8/N7/2p5/3K4 w - - 0 1"};

int mates = 0;
int checks = 0;
int double_checks = 0;
int stalemate = 0;
int captures = 0;
int ep = 0;
PositionState stater;
Movelist<3000> list;

int all_nodes = 0;

template <Color clr>
int negamax(Board &brd, int d, Movelist_ref list_ref = Movelist_ref(list))
{
    ++all_nodes;
    if (d == 0)
    {
        // Graphic gr(brd);
        // gr.draw_board(White);
        return brd.eval<clr>();
    }
    Movegen generator(brd, list_ref);

    PositionState state = generator.gen_all_moves<clr>();
    // brd.sort_moves<clr>(list_ref);
    if (list_ref.no_moves())
    {
        constexpr int mate = -300'000;

        if (state >= check)
            return mate;

        constexpr int stalemate = 0;
        return stalemate;
    }

    constexpr int inf = 1'000'000;

    int alpha = -inf;

    for (Move_full_info *i = list_ref.begin; i < list_ref.end; ++i)
    {

        const Accumulator acc = brd.get_accumulator();
        const Piece piece = brd[i->to_square];
        brd.unstable_make_move<clr>(*i);

        int loc_eval = -negamax<change_color(clr)>(brd, d - 1, list_ref.get_ref());

        brd.unstable_undo_move<clr>(*i, piece);
        brd.restore_info(acc);

        // cout << square_to_str(static_cast<Square>(i->from_square)) << " : " << square_to_str(static_cast<Square>(i->to_square)) << " " << loc_eval << " " << "\n";

        if (loc_eval > alpha)
        {
            alpha = loc_eval;
        }
    }
    return alpha;
}

template <Color clr>
tuple<Move_full_info, int> best_move(Board &brd, int d)
{
    ++all_nodes;
    Movelist_ref list_ref(list);
    Movegen generator(brd, list_ref);

    PositionState state = generator.gen_all_moves<clr>();
    // brd.sort_moves<clr>(list_ref);

    if ((d == 0) || (list_ref.no_moves()))
        return {};

    constexpr int inf = 1'000'000'000;

    int alpha = -inf;
    // int beta = inf;

    Move_full_info best_move;

    for (Move_full_info *i = list_ref.begin; i < list_ref.end; ++i)
    {

        const Accumulator acc = brd.get_accumulator();
        const Piece piece = brd[i->to_square];
        brd.unstable_make_move<clr>(*i);
        int loc_eval = -negamax<change_color(clr)>(brd, d - 1, list_ref.get_ref());
        brd.unstable_undo_move<clr>(*i, piece);
        brd.restore_info(acc);
        cout << square_to_str(static_cast<Square>(i->from_square)) << " : " << square_to_str(static_cast<Square>(i->to_square)) << " " << loc_eval << " "
             << "\n";

        if (loc_eval > alpha)
        {
            best_move = *i;
            alpha = loc_eval;
        }
    }

    return {best_move, alpha};
}

Square parse_str_to_sq(const string &str)
{
    return static_cast<Square>((str[0] - 'a') + (str[1] - '1') * 8);
}

Promotion parse_str_to_prom(const string &str)
{
    if (str == "no")
        return No_promotion;
    else if (str == "bishop")
        return promote_to_bishop;
    else if (str == "knight")
        return promote_to_knight;
    else if (str == "rook")
        return promote_to_rook;
    else if (str == "queen")
        return promote_to_queen;

    return No_promotion;
}

Special parse_str_to_SP(const string &str)
{
    if (str == "no")
        return No_special;
    else if (str == "ep")
        return SP_en_passant;
    else if (str == "cast")
        return SP_castling;
    else if (str == "prom")
        return SP_Promotion;

    return No_special;
}

template <typename T, int size>
void parse_to_c_array(const std::array<T, size> &arr, const std::string nameOfArr)
{
    std::cout << "constexpr u64 " << nameOfArr << "[" << size << "] = {\n";
    std::cout << arr[real_board_id(0)];
    for (int i = 1; i < size; ++i)
    {
        std::cout << ", ";

        if ((i % 8) == 0)
            cout << "\n";

        if (abs(arr[real_board_id(i)]) <= 9)
            std::cout << ' ';
        std::cout << arr[real_board_id(i)];
    }
    std::cout << "};\n";
}

int main()
{

    fenParser f;
    Board brd;
    Graphic graphic(brd);
    f.parse_from_FEN("rnbqkbnr/1ppppppp/p7/1N6/8/8/PPPPPPPP/R1BQKBNR b KQkq - 1 2", brd);
    
    //Movelist_ref list_ref(list);
    //Movegen gen(brd, list_ref);
    cout << (brd.get_hash() ^ black_side_to_move_hash ^ zobrist_hashtable[18][W_Knight] ^ zobrist_hashtable[33][W_Knight])  << "\n";
    
    f.parse_from_FEN("rnbqkbnr/1ppppppp/8/1p6/8/8/PPPPPPPP/R1BQKBNR w KQkq - 0 3", brd);
    cout << brd.get_hash() << "\n";
    
    return 0;

    constexpr int depth = 3;
    Movelist_ref list_ref(list);
    
    AI bot(brd, list_ref);
    int eval = 0;
    Move_full_info move;


    std::tie(move, eval) = bot.best_move_negamax(depth);
    cout << square_to_str(static_cast<Square>(move.from_square)) << " : " << square_to_str(static_cast<Square>(move.to_square)) << "      " << eval << '\n';
    cout << bot.all_nodes << '\n';


    for(int i = 0; i < 20; ++i){
        std::tie(move, eval) = bot.best_move_ab(5);
        cout << square_to_str(static_cast<Square>(move.from_square)) << " : " << square_to_str(static_cast<Square>(move.to_square)) << "      " << eval << '\n';
        cout << bot.all_nodes << '\n';
        brd.get_turn() ? brd.unstable_make_move<White>(move) : brd.unstable_make_move<Black>(move);
        cout << "\n\n\n"; 
    }

    cout << f.parse_to_Fen(brd) << "\n\n";
    
    
    std::tie(move, eval) = bot.best_move_ab(depth);
    cout << square_to_str(static_cast<Square>(move.from_square)) << " : " << square_to_str(static_cast<Square>(move.to_square)) << "      " << eval << '\n';
    cout << bot.all_nodes << '\n';
    
    std::tie(move, eval) = bot.best_move_ab(depth);
    cout << square_to_str(static_cast<Square>(move.from_square)) << " : " << square_to_str(static_cast<Square>(move.to_square)) << "      " << eval << '\n';
    cout << bot.all_nodes << '\n';
    
    //cout << bot.perft(4) << '\n';
    //PlayerWithPlayer pvp(gen, brd, f, graphic);
    //pvp.do_move<White>("e5d6");
    cout << brd.get_turn() << '\n';
    const Accumulator accum = brd.unstable_make_move<Black>(move);
    cout << brd.get_turn() << '\n';
    brd.unstable_undo_move<Black>(move, accum);
    cout << brd.get_turn() << '\n';
   
    return 0;
}

 // for(int i = 0; i < 64; ++i){
    //     int newi = i ^ 56;
    //     int eval_W = brd.square_eval(i);
    //     int eval_b = brd.square_eval(newi);
    //     if((i%8) == 0)cout<<"\n";
    //     cout.width(4);
    //     cout <<  eval_W << " ";
    // }
    // cout << "\n\n\n";
    // Move_full_info move = brd.get_turn() ? best_move<White>(brd, depth) : best_move<Black>(brd, depth);
    // cout << "\n\n" << square_to_str(static_cast<Square>(move.from_square)) << " : " << square_to_str(static_cast<Square>(move.to_square)) << " all nodes: " << all_nodes<< "\n";
    // cout << "\n\n" << brd.eval<White>() << "\n";
    // cout << "\n\n" << brd.eval<Black>() << "\n";


    // gen.gen_all_moves<White>();
    // brd.sort_moves<White>(list_ref);
    // for(Move_full_info *i = list_ref.begin; i < list_ref.end; ++i){
    //     cout << square_to_str(static_cast<Square>(i->from_square)) << " : " << square_to_str(static_cast<Square>(i->to_square)) << "\n";
    // }



    /*graphic.draw_board(White);
    brd.unstable_make_move<White>(Move_full_info(SQ_A2, SQ_A4, No_promotion, No_special));
    graphic.draw_board(White);

    Movelist<1000> list;
    Movelist_ref list_ref(list);
    Movegen generator(brd, list_ref);
    generator.gen_en_passant<Black>();
    graphic.draw_board(White);
    for (Move_full_info *i = list_ref.begin; i < list_ref.end; ++i)
    {
        brd.unstable_make_move<Black>(*i);
    }
    graphic.draw_board(White);
    for (Move_full_info *i = list_ref.begin; i < list_ref.end; ++i)
    {
        brd.unstable_undo_move<Black>(*i);
    }
    graphic.draw_board(White);*/
    /*
    Movelist<1000> list;
    Movelist_ref list_ref(list);

    graphic.draw_board(White);
    Movegen generator(brd, list_ref);

    // generator.all_king_rays<White>(generator.get_king<White>());

    PositionState state;
    int attack_sq;
    Direction attack_dir;
    //int king_id = brd.get_right_king_position<White>();


    // tie(state, attack_sq, attack_dir) = generator.get_attackers_information<White>(king_id);
    // u64 mask = (state == check) ? generator.generate_check_mask<White>(king_id, attack_sq, attack_dir) : 0;

    // bool pinned;
    // u64 pinned_mask;

    // generator.gen_all_pinned_moves<White>(king_id);
    generator.gen_all_moves<White>();

    for (Move_full_info *i = list_ref.begin; i < list_ref.end; ++i)
    {
        brd[i->to_square] = W_Pawn;
    }
    graphic.draw_board(White);


    // CLEAR

    // graphic.draw_board(Black);
    // std::cout <<brd[SQ_B7] << "\n";
    */
    // for(;;){
    //     all_nodes = 0;
    //     Move_full_info move = brd.get_turn() ? best_move<White>(brd, depth) : best_move<Black>(brd, depth);
    //     brd.unstable_make_move<White>(move);
    //     cout << square_to_str(static_cast<Square>(move.from_square)) << " : " << square_to_str(static_cast<Square>(move.to_square)) << " all nodes: " << all_nodes<< "\n";
    //     string from, to, prom, special;
    //     cout << "Move?: ";
    //     cin >> from >> to >> prom >> special;
    //     Move_full_info mymove(parse_str_to_sq(from), parse_str_to_sq(to), parse_str_to_prom(prom), parse_str_to_SP(special));
    //     brd.unstable_make_move<Black>(mymove);
    //     //cout << "captures: " << captures << "\n";
    //     //cout << "mates: " << mates << "\n";
    //     //cout << "checks: " << checks << "\n";
    //     //cout << "en passant: " << ep << "\n";
    //     //cout << "double check: " << double_checks << "\n";
    //     //graphic.draw_board(White);
    // }

    // cout << stater;