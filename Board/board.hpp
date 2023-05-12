#pragma once
#include "../types.hpp"
#include "evaltables.hpp"
#include "hashing.hpp"
namespace chess{

   

    
    

    class Board
    {
    private:
        types::array<Piece, 64>  table;
        Color turn = White;
        Square white_king_position, black_king_position;
        CastlingRights castl_rights = White_OO_OOO | Black_OO_OOO;

        Square en_passant = No_Square;

        int fifty_moves_rule = 0;
        int total_moves = 0;

    public:
        
        Board(){}
        
        Board(const Board &board):table(board.table), turn(board.turn), white_king_position(board.white_king_position),
        black_king_position(board.black_king_position), castl_rights(board.castl_rights), en_passant(board.en_passant),
        fifty_moves_rule(board.fifty_moves_rule),total_moves(board.total_moves)  {}

        friend bool operator==(const Board &left, const Board &right){
            return (left.table == right.table) && (left.turn== right.turn) && (left.white_king_position == right.white_king_position) && 
            (left.black_king_position == right.black_king_position) && (left.castl_rights == right.castl_rights) && 
            (left.en_passant == right.en_passant) &&
            (left.fifty_moves_rule == right.fifty_moves_rule) && (left.total_moves == right.total_moves);
        }

        friend bool operator!=(const Board &left, const Board &right){
            return (left.table != right.table) || (left.turn!= right.turn) || (left.white_king_position != right.white_king_position) || 
            (left.black_king_position != right.black_king_position) || (left.castl_rights != right.castl_rights) || 
            (left.en_passant != right.en_passant) ||
            (left.fifty_moves_rule != right.fifty_moves_rule) || (left.total_moves != right.total_moves);
        }

        template<Color color>
        void remove_short_castle(){
            if constexpr(color)
                castl_rights &= static_cast<CastlingRights>(~White_OO);
            else 
                castl_rights &= static_cast<CastlingRights>(~Black_OO);
        }
        
        template<Color color>
        void remove_long_castle(){
            if constexpr(color)
                castl_rights &= static_cast<CastlingRights>(~White_OOO);
            else 
                castl_rights &= static_cast<CastlingRights>(~Black_OOO);
        }

        template<Color color>
        void remove_all_castle(){
            if constexpr(color)
                castl_rights &= static_cast<CastlingRights>(~White_OO_OOO);
            else 
                castl_rights &= static_cast<CastlingRights>(~Black_OO_OOO);
        }

        template<Color color>
        bool can_castle_long(){
            if constexpr(color){
                return castl_rights & White_OOO;
            }
            else
                return castl_rights & Black_OOO;
        }
         
        template<Color color>
        bool can_castle_short(){
            if constexpr(color){
                return castl_rights & White_OO;
            }
            else
                return castl_rights & Black_OO;
        }

        inline Piece& operator[](const uint id){
            #ifdef MAESTRO_DEBUG
            //Something
            #endif
            return table[id];
        }

        inline const Piece& operator[](const uint id)const{
            #ifdef MAESTRO_DEBUG
            if(id >= 64)
                throw std::out_of_range("Invalid id: " + id);
            #endif
            return table[id];
        }
        // setters
        void set_turn(Color color){
            turn = color;
        }
        void set_castling(const CastlingRights rights){
            castl_rights = rights;
        }

        void add_castling(const CastlingRights rights){
            castl_rights |= rights;
        }
        bool has_castling(const CastlingRights rights)const{
            return (castl_rights & rights) == rights;
        }
        inline void set_en_passant(chess::Square sq){
            #ifdef MAESTRO_DEBUG
            if(((sq > SQ_H3) && (sq < SQ_A3)) || ((sq > SQ_H6) && (sq < SQ_A6)))
                throw std::invalid_argument("Invalid en passant square: " + sq);
            #endif
            en_passant = sq;
        }
        void set_fifty_rule(int move){
            #ifdef MAESTRO_DEBUG
            if (move >= 50)
                throw std::invalid_argument("Invalid fifty moves: " + move);
            #endif
            fifty_moves_rule = move;
        }
        void set_total_moves(int moves){
            #ifdef MAESTRO_DEBUG
            if(moves < 0)
                throw std::invalid_argument("Invalid moves passed: " + moves);
            #endif
            total_moves = moves;
        }

        // getters
        inline Color get_turn()const{return turn;}
        inline CastlingRights get_castling()const{ return castl_rights; }
        inline Square get_en_passant()const{ return en_passant;}
        inline int get_fifty_rule()const{return fifty_moves_rule;}
        inline int get_total_moves()const{return total_moves;}

        // methods
        void clear_board(){
            for (Piece &piece : table)
            {
                piece = No_Piece;
            }
        }

        template<Color color>
        inline void do_en_passant_remove(const int id){
            if constexpr(color)
                table[id - 8] = No_Piece;
            else
                table[id + 8] = No_Piece;
        }
        template<Color color>
        inline void do_rook_castling(const int id){
            if(id == short_castling_square<color>()){
                table[short_castling_rook_from_square<color>()] = No_Piece;
                table[short_castling_rook_to_square<color>()] = Rook_with_color<color>();
            }   
            else{
                table[long_castling_rook_from_square<color>()] = No_Piece;
                table[long_castling_rook_to_square<color>()] = Rook_with_color<color>();
            }
        }

        template<Color color>
        constexpr Square& get_right_king_position(){
            if constexpr(color)
                return white_king_position;            
            else 
                return black_king_position;
        }



        template<Color color>
        inline Accumulator unstable_make_move(const Move_full_info move){
            #ifdef MAESTRO_DEBUG
            /// TODO: write checks
            #endif
            turn = static_cast<Color>(!turn);
            const Accumulator accumulator{static_cast<u8>(castl_rights), static_cast<u8>(white_king_position), static_cast<u8>(black_king_position), static_cast<u8>(en_passant), static_cast<u8>(table[move.to_square])};
            if(table[move.from_square] == King_with_color<color>()){
                get_right_king_position<color>() = static_cast<Square>(move.to_square);
                remove_all_castle<color>();
            }
            switch (move.from_square)
            {
            case short_castling_rook_from_square<color>():
                remove_short_castle<color>();
                break;
            case long_castling_rook_from_square<color>():
                remove_long_castle<color>();
            }
            
            switch (move.to_square)
            {
            case short_castling_rook_from_square<change_color(color)>():
                remove_short_castle<change_color(color)>();
                break;
            case long_castling_rook_from_square<change_color(color)>():
                remove_long_castle<change_color(color)>();
            }

            en_passant = No_Square;
            if((table[move.from_square] == Pawn_with_color<color>()) && 
            ((move.to_square - move.from_square) == pawn_double_move_distance<color>()))
                en_passant = static_cast<Square>(move.from_square + pawn_en_passant_direction_from<color>());
            

            switch (move.special)
            {
            case No_special:
                table[move.to_square] = table[move.from_square];
                break;
                
            case SP_Promotion:
                table[move.to_square] = static_cast<Piece>(move.promotion + Knight_with_color<color>());
                break;
            case SP_castling:
                do_rook_castling<color>(move.to_square);
                table[move.to_square] = King_with_color<color>();
                break;
            case SP_en_passant:
                do_en_passant_remove<color>(move.to_square);
                table[move.to_square] = Pawn_with_color<color>();
            }
            table[move.from_square] = No_Piece;
            return accumulator;
        }

        template<Color color>
        inline void undo_rook_castling(const int id){
            if(id == short_castling_square<color>()){
                table[short_castling_rook_from_square<color>()] = Rook_with_color<color>();
                table[short_castling_rook_to_square<color>()] = No_Piece;
            }   
            else{
                table[long_castling_rook_from_square<color>()] = Rook_with_color<color>();
                table[long_castling_rook_to_square<color>()] = No_Piece;
            }
        }

        template<Color color>
        inline void undo_en_passant_remove(const int id){
            if constexpr(color)
                table[id - 8] = Pawn_with_color<change_color(color)>();
            else
                table[id + 8] = Pawn_with_color<change_color(color)>();
        }

        template<Color color>
        inline void unstable_undo_move(const Move_full_info move, const Accumulator accumulator){
            #ifdef MAESTRO_DEBUG
            /// TODO: write checks
            #endif
            turn = static_cast<Color>(!turn);

            table[move.from_square] = table[move.to_square];
            switch (move.special)
            {
            case SP_castling:
                undo_rook_castling<color>(move.to_square);
                table[move.to_square] = King_with_color<color>();
                break;
            case SP_en_passant:
                undo_en_passant_remove<color>(move.to_square);
                //table[move.to_square] = Pawn_with_color<color>();
                break;
            case SP_Promotion:
                table[move.from_square] = Pawn_with_color<color>();
            }
            table[move.to_square] = static_cast<Piece>(accumulator.piece_to_revive);
            restore_info(accumulator);
        }

        void find_kings(){
            white_king_position = No_Square;
            black_king_position = No_Square;
            for(int i = 0; i < 64; ++i){
                switch(table[i]){
                case W_King:
                    #ifdef MAESTRO_DEBUG 
                        if(white_king_position != No_Square)
                            throw std::logic_error("Too many white kings!");
                    #endif
                    white_king_position = static_cast<Square>(i);
                    
                break;
                
                case B_King:
                    #ifdef MAESTRO_DEBUG 
                        if(black_king_position != No_Square)
                            throw std::logic_error("Too many black kings!");
                    #endif    
                    black_king_position = static_cast<Square>(i);
                }
            }
            #ifdef MAESTRO_DEBUG 
                if(white_king_position == No_Square)
                    throw std::logic_error("No white king!");
                if(black_king_position == No_Square)
                    throw std::logic_error("No black king!");
            #endif
        }

        inline Accumulator get_accumulator()const{
            return {static_cast<u8>(castl_rights), static_cast<u8>(white_king_position), static_cast<u8>(black_king_position), static_cast<u8>(en_passant)};
        }
        inline void restore_info(const Accumulator& accumulator){
            castl_rights = static_cast<CastlingRights>(accumulator.castling_rights);
            white_king_position = static_cast<Square>(accumulator.white_king_position);
            black_king_position = static_cast<Square>(accumulator.black_king_position);
            en_passant = static_cast<Square>(accumulator.en_passant);
        }
        template<Color clr>
        inline void remove_king(){
            if constexpr(clr)
                table[white_king_position] = No_Piece;
            else
                table[black_king_position] = No_Piece;
        }

        template<Color clr>
        inline void restore_king(){
            if constexpr(clr)
                table[white_king_position] = W_King;
            else
                table[black_king_position] = B_King;
        }




































        inline std::pair<int, int> eval_square(int id){
            if(table[id] == No_Piece)
                return {0, 0};
            Piece piece = table[id];
            
            if(piece_color(piece))
                return {square_eval_midlegame[piece][id ^ 56], square_eval_endgame[piece][id ^ 56]};
            
            return {-square_eval_midlegame[piece - 6][id], -square_eval_endgame[piece - 6][id]};
        }
        

        
        inline int eval_position(){
            int eval_midlegame = 0, eval_endgame = 0, midlegame_phase = 0;
            
            for(int i = 0; i < 64; ++i){
                std::pair<int, int> square_eval{eval_square(i)};

                eval_midlegame += material_midlegame[table[i]] + square_eval.first;
                eval_endgame += material_endgame[table[i]] + square_eval.second;

                midlegame_phase += phase_table[table[i]];
            }

            if(midlegame_phase > 24)
                midlegame_phase = 24;

            int endgame_phase = 24 - midlegame_phase;
            
            return (midlegame_phase * eval_midlegame + endgame_phase * eval_endgame) / 24;
        }

        template<Color color>
        inline int eval(){
            if constexpr(color)
                return eval_position();
            else
                return -eval_position();
        }




    
        u64 get_hash(){
            u64 hash = 0;

            for(int i = 0; i < 64; ++i){
                if(table[i] != No_Piece)
                    hash ^= zobrist_hashtable[i][table[i]];
            }
            if(contains(castl_rights, White_OO))
                hash ^= White_OO_hash;
            if(contains(castl_rights, White_OOO))
                hash ^= White_OOO_hash;

            if(contains(castl_rights, Black_OO))
                hash ^= Black_OO_hash;
            if(contains(castl_rights, Black_OOO))
                hash ^= Black_OOO_hash;
            
            if(!turn)
                hash ^= black_side_to_move_hash;
            if(en_passant != No_Square)
                hash ^= en_passant_files_hash[en_passant % 8];
            
            return hash;
        }




        template<Color color>
        inline void do_rook_hashing(const int id, u64 &hash){
            if(id == short_castling_square<color>()){
                hash ^= zobrist_hashtable[short_castling_rook_from_square<color>()][Rook_with_color<color>()] ^ 
                zobrist_hashtable[short_castling_rook_to_square<color>()][Rook_with_color<color>()];
            }   
            else{
                hash ^= zobrist_hashtable[long_castling_rook_from_square<color>()][Rook_with_color<color>()] ^ 
                zobrist_hashtable[long_castling_rook_to_square<color>()][Rook_with_color<color>()];
            }
        }

        template<Color color>
        inline void do_en_passant_hashing(const int id, u64 &hash){
            if constexpr(color)
                hash ^= zobrist_hashtable[id - 8][Pawn_with_color<change_color(color)>()];
            else
                hash ^= zobrist_hashtable[id + 8][Pawn_with_color<change_color(color)>()];
        }




        template<Color color>
        u64 get_hash(u64 hash, Move_full_info move){
            hash ^= black_side_to_move_hash;
            if(table[move.to_square] != No_Piece)
                hash ^= zobrist_hashtable[move.to_square][table[move.to_square]];

            if(table[move.from_square] == King_with_color<color>()){
                if(contains(castl_rights, short_castling_with_color<color>() ))
                    hash ^= short_castling_hash_with_color<color>();
                if(contains(castl_rights, long_castling_with_color<color>()))
                    hash ^= long_castling_hash_with_color<color>();
            }

            hash ^= zobrist_hashtable[move.from_square][table[move.from_square]];
            
            if(contains(castl_rights, short_castling_with_color<color>()) && (move.from_square == short_castling_rook_from_square<color>()) 
            && (table[short_castling_rook_from_square<color>()] == Rook_with_color<color>()))
                hash ^= short_castling_hash_with_color<color>();

            if(contains(castl_rights, long_castling_with_color<color>()) && (move.from_square == long_castling_rook_from_square<color>()) 
            && (table[long_castling_rook_from_square<color>()] == Rook_with_color<color>()))
                hash ^= long_castling_hash_with_color<color>();


            if(contains(castl_rights, short_castling_with_color<change_color(color)>()) && (move.to_square == short_castling_rook_from_square<change_color(color)>()) 
            && (table[short_castling_rook_from_square<change_color(color)>()] == Rook_with_color<change_color(color)>()))
                hash ^= short_castling_hash_with_color<change_color(color)>();

            if(contains(castl_rights, long_castling_with_color<change_color(color)>()) && (move.to_square == long_castling_rook_from_square<change_color(color)>()) 
            && (table[long_castling_rook_from_square<change_color(color)>()] == Rook_with_color<change_color(color)>()))
                hash ^= long_castling_hash_with_color<change_color(color)>();
            
            
            
            if(en_passant != No_Square)
                hash ^= en_passant_files_hash[en_passant % 8];

            if((table[move.from_square] == Pawn_with_color<color>()) && 
                ((move.to_square - move.from_square) == pawn_double_move_distance<color>()))
                hash ^= en_passant_files_hash[move.to_square % 8];

            

            switch (move.special)
            {
            case No_special:
                hash ^= zobrist_hashtable[move.to_square][table[move.from_square]];
                break;
            case SP_Promotion:
                hash ^= zobrist_hashtable[move.to_square][move.promotion + Knight_with_color<color>()];
                break;
            case SP_castling:
                do_rook_hashing<color>(move.to_square, hash);
                hash ^= zobrist_hashtable[move.to_square][King_with_color<color>()];
                break;
            case SP_en_passant:
                do_en_passant_hashing<color>(move.to_square, hash);
                hash ^= zobrist_hashtable[move.to_square][Pawn_with_color<color>()];
            }
            
            return hash;
        }

    };







 
}