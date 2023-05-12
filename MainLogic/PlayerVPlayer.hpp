#pragma once
#include <iostream>
#include "movegen.hpp"
#include "fenParser.hpp"
#include "../Graphic/graphic.hpp"
#include "../Board/board.hpp"

namespace chess
{
    using u8 = uint8_t;
    class PlayerWithPlayer
    {
    private:
        Movegen &movegen;
        Board &board;
        fenParser &parser;
        Graphic &graphic;

        PositionState state;

        const std::string START_POS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        const std::string EXPR = "r3k2r/ppp1pppp/8/3pP3/8/8/PPPP1PPP/R3K2R w KQkq - 0 1";
        template <Color clr>
        void get_position_state()
        {
            state = movegen.gen_all_moves<clr>();
        }

    public:
        PlayerWithPlayer(Movegen &move, Board &brd, fenParser &pars, Graphic &graph) : movegen(move), board(brd), parser(pars), graphic(graph)
        {
            parser.parse_from_FEN(EXPR, board);
        }

        Square parse_str_to_sq(const std::string &str)
        {
            if (str.size() < 2 || str.size() > 2)
            {
                std::cout << " Incorrect format for parsing " << str.size();
                return No_Square;
            }

            return static_cast<Square>((str[0] - 'a') + (str[1] - '1') * 8);
        }
        template <Color clr>
        std::tuple<Square, Square, Special, Promotion> parse_input_move(const std::string &move)
        {
            Square from = No_Square, to = No_Square;
            Special spec = No_special;
            Promotion prom = No_promotion;

            if (move.compare("O-O") == 0)
            {

                spec = SP_castling;
                return {from, clr ? SQ_H1 : SQ_H8, spec, prom};
            }

            if (move.compare("O-O-O") == 0)
            {
                spec = SP_castling;
                return {from, clr ? SQ_A1 : SQ_A8, spec, prom};
            }

            from = parse_str_to_sq(move.substr(0, 2));
            to = parse_str_to_sq(move.substr(2, 3));

            if (move.size() == 5 && move.at(0) != 'O')
            {
                spec = SP_Promotion;
            }

            if (board[from] == Pawn_with_color<clr>())
            {
                std::cout << "\n\nERROR";
                if (abs(to - from) % 8 != 0)
                {
                    std::cout << "\n\nERROR";
                    spec = SP_en_passant;
                }
            }

            if (spec != No_special)
                prom = parse_prom_from_input(move.at(move.size() - 1));

            
            std::cout << "\n\n" << from << " " << to << " " << (int)spec << " " << (int)prom;  
            return {from, to, spec, prom};
        }

        Promotion parse_prom_from_input(const char prom_char)
        {
            if (prom_char == 'b')
                return promote_to_bishop;
            else if (prom_char == 'k')
                return promote_to_knight;
            else if (prom_char == 'r')
                return promote_to_rook;
            else if (prom_char == 'q')
                return promote_to_queen;

            return No_promotion;
        }
        template <Color clr>
        Move_full_info pars_to_move_full_info(const std::string &move)
        {
            Square from, to;
            Special spec;
            Promotion prom;

            std::tie(from, to, spec, prom) = parse_input_move<clr>(move);

            return Move_full_info(from, to, prom, spec);
        }

        template <Color clr>
        void do_move(const std::string &move)
        {
            get_position_state<clr>();
            Move_full_info target = pars_to_move_full_info<clr>(move);
            Movelist_ref list = movegen.get_list();

            graphic.draw_board(White);

            for (int i = 0; i < list.count(); ++i)
            {
                if (target == list[i])
                {
                    board.unstable_make_move<clr>(target);
                    //std::cout << "\n\n\nASDDASDASDASDASDASD\n\n\n";
                    break;
                }
            }

            graphic.draw_board(White);
        }
    };

}