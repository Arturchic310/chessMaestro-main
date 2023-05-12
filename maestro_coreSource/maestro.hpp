#pragma once
#include "types.hpp"
#include "tables.hpp"

namespace maestro{
        
    
    
    /// A move needs 16 bits to be stored
    ///
    /// bit  0- 5: destination square (from 0 to 63)
    /// bit  6-11: origin square (from 0 to 63)
    /// bit 12-13: promotion piece type - 2 (from KNIGHT-2 to QUEEN-2)
    /// bit 14-15: special move flag: promotion (1), en passant (2), castling (3)
    
    
    /// NOTE: en passant bit is set only when a pawn can be captured
    ///
    /// Special cases are MOVE_NONE and MOVE_NULL. We can sneak these in because in
    /// any normal move destination square is always different from origin square
    /// while MOVE_NONE and MOVE_NULL have the same origin and destination square.

        
    
    
    
    constexpr void add_castle_right(CastlingRights &rights, CastlingRights mask)noexcept{
        rights = static_cast<CastlingRights>(static_cast<int>(rights) | static_cast<int>(mask));
    }
    
    
    
    
    
    template<Color clr>
    consteval Color change_color(){
        if constexpr(clr){
            return Color_Black;
        }
        else{
            return Color_White;
        }
    }
    
    
    
    

    
    template<Color clr>
    constexpr Piece promotion_flag_to_piece(const Promotion promotion_flag)noexcept{
        return type_to_piece<clr, static_cast<PieceType>(static_cast<int>(promotion_flag) + 1)>();
    }
    
    
    constexpr const char* to_symbol(const Piece piece){
        switch(piece){
            case W_Pawn: 
                return "\U00002659";
            case W_Knight:
                return "\U00002658";
            case W_Bishop:
                return "\U00002657";
            case W_Rook:
                return "\U00002656";
            case W_Queen:
                return "\U00002655";
            case W_King:
                return "\U00002654";
                
                
            
            case B_Pawn: 
                return "\U0000265F";
            case B_Knight:
                return "\U0000265E";
            case B_Bishop:
                return "\U0000265D";
            case B_Rook:
                return "\U0000265C";
            case B_Queen:
                return "\U0000265B";
            case B_King:
                return "\U0000265A";
            
            case No_Piece:
                return "-";
        }
        return "-";
    }
    
    constexpr Piece char_to_piece(const char symb)noexcept{
        switch(symb){
            case 'p':
                return B_Pawn;
            case 'n':
                return B_Knight;
            case 'b':
                return B_Bishop;
            case 'r':
                return B_Rook;
            case 'q':
                return B_Queen;
            case 'k':
                return B_King;
                
                
            case 'P':
                return W_Pawn;
            case 'N':
                return W_Knight;
            case 'B':
                return W_Bishop;
            case 'R':
                return W_Rook;
            case 'Q':
                return W_Queen;
            case 'K':
                return W_King;
                
                
            default:
                return No_Piece;
        }
    }
    
    
    constexpr Color piece_color(const Piece piece)noexcept{
        if(static_cast<int>(piece) < 6)
            return Color_White;
        else 
            return Color_Black;
    }
    
    struct Board{
    public:
        std::array<u64, 12> bit_boards;
        
        u64 White_brd;
        u64 Black_brd;
        u64 Not_free;
        
        CastlingRights castle_rights;
        u16 half_moves;
        
        Square en_passant_take_square;
        
        Color who_to_move;
        
        std::array<Piece, 64> mailbox;
        
        u16 fifty_moves_rule;
        u16 move_number;

        template<Color clr>
        inline u64& My_Pawns()noexcept{
            if constexpr(clr){
                return bit_boards[static_cast<uint>(W_Pawn)];
            }
            else{
                return bit_boards[static_cast<uint>(B_Pawn)];
            }
        }

        template<Color clr>
        inline u64& My_Knights()noexcept{
            if constexpr(clr){
                return bit_boards[W_Knight];
            }
            else{
                return bit_boards[B_Knight];
            }
        }
        
        template<Color clr>
        inline u64& My_Bishops()noexcept{
            if constexpr(clr){
                return bit_boards[W_Bishop];
            }
            else{
                return bit_boards[B_Bishop];
            }
        }

        template<Color clr>
        inline u64& My_Rooks()noexcept{
            if constexpr(clr){
                return bit_boards[W_Rook];
            }
            else{
                return bit_boards[B_Rook];
            }
        }
        
        template<Color clr>
        inline u64& My_Queens()noexcept{
            if constexpr(clr){
                return bit_boards[W_Queen];
            }
            else{
                return bit_boards[B_Queen];
            }
        }

        template<Color clr>
        inline u64& My_King()noexcept{
            if constexpr(clr){
                return bit_boards[W_King];
            }
            else{
                return bit_boards[B_King];
            }
        }

        template<Color clr>
        inline u64& My_Board()noexcept{
            if constexpr(clr){
                return White_brd;
            }
            else{
                return Black_brd;
            }
        }


        template<Color clr>
        inline u64& Enemy_Pawns()noexcept{
            if constexpr(clr){
                return bit_boards[B_Pawn];
            }
            else{
                return bit_boards[W_Pawn];
            }
        }

        template<Color clr>
        inline u64& Enemy_Knights()noexcept{
            if constexpr(clr){
                return bit_boards[B_Knight];
            }
            else{
                return bit_boards[W_Knight];
            }
        }

        template<Color clr>
        inline u64& Enemy_Bishops()noexcept{
            if constexpr(clr){
                return bit_boards[B_Bishop];
            }
            else{
                return bit_boards[W_Bishop];
            }
        }

        template<Color clr>
        inline u64& Enemy_Rooks()noexcept{
            if constexpr(clr){
                return bit_boards[B_Rook];
            }
            else{
                return bit_boards[W_Rook];
            }
        }

        template<Color clr>
        inline u64& Enemy_Queens()noexcept{
            if constexpr(clr){
                return bit_boards[B_Queen];
            }
            else{
                return bit_boards[W_Queen];
            }
        }

        template<Color clr>
        inline u64& Enemy_King()noexcept{
            if constexpr(clr){
                return bit_boards[B_King];
            }
            else{
                return bit_boards[W_King];
            }
        }

        template<Color clr>
        inline u64& Enemy_Board()noexcept{
            if constexpr(clr){
                return Black_brd;
            }
            else{
                return White_brd;
            }
        }

        
        inline bool operator==(const Board &brd){
            return (bit_boards == brd.bit_boards) && (White_brd == brd.White_brd) && (Black_brd == brd.Black_brd) &&
            (Not_free == brd.Not_free) && (castle_rights == brd.castle_rights) && (half_moves == brd.half_moves) &&
            (en_passant_take_square == brd.en_passant_take_square) && (who_to_move == brd.who_to_move) && 
            (mailbox == brd.mailbox) && (fifty_moves_rule == brd.fifty_moves_rule) && (move_number == brd.move_number);
        }
        
        template<Color clr>
        constexpr u64 Enemy_or_Empty()const noexcept{
            if constexpr(clr){
                return ~Black_brd;
            }
            else{
                return ~White_brd;
            }
        }
        
        
        //remove a piece on the certain square, Color means the color of piece being removed
        // guaranteed that it's not No_Piece
        template<Color clr>
        inline void remove_piece(const Square square)noexcept{
            const u64 remove_mask = bit_at(square);
            bit_boards[mailbox[square]] ^= remove_mask;
            mailbox[square] = No_Piece;
            My_Board<clr>() ^= remove_mask;
        }
        
        template<Color clr>
        inline void remove_piece(const Square square, const Piece piece)noexcept{
            const u64 remove_mask = bit_at(square);
            bit_boards[piece] ^= remove_mask;
            mailbox[square] = No_Piece;
            My_Board<clr>() ^= remove_mask;
        }
        
        //put a piece on the certain square, Color means the color of piece being put
        // guaranteed that it's No_Piece
        template<Color clr>
        inline void put_piece(const Square square, const Piece piece)noexcept{
            const u64 put_mask = bit_at(square);
            bit_boards[piece] |= put_mask;
            mailbox[square] = piece;
            My_Board<clr>() |= put_mask;
        }
        
        //replace a piece on the certain square
        // guaranteed that it's not No_Piece
        inline void replace_piece(const Square square, const Piece piece)noexcept{
            const u64 remove_and_put_mask = bit_at(square);
            bit_boards[mailbox[square]] ^= remove_and_put_mask;
            bit_boards[piece] |= remove_and_put_mask;
            mailbox[square] = piece;
            White_brd ^= remove_and_put_mask;
            Black_brd ^= remove_and_put_mask;
        }
        
        template<Color clr>
        inline void replace_if_present_or_put_piece(const Square square, const Piece piece)noexcept{
            if(mailbox[square] == No_Piece){
                put_piece<clr>(square, piece);
            }
            else{
                replace_piece(square, piece);
            }
        }
        
        
        template<Color clr>
        inline void do_en_passant_remove(const Square square)noexcept{
            if constexpr(clr){
                remove_piece<change_color<clr>()>(square - 8, B_Pawn);
            }
            else{
                remove_piece<change_color<clr>()>(square + 8, W_Pawn);
            }
        }
        
        template<Color clr>
        inline void do_rook_castling(const Square to_square)noexcept{
            /// NOTE: King is already removed
            //remove_piece<clr>(move.from_square, W_King);

            put_piece<clr>(to_square, King_with_color<clr>());
            if(to_square == king_short_castling_to_sq<clr>()){
                /// DONE: castling squares
                /// TODO: move_piece method
                remove_piece<clr>(rook_short_castling_from_sq<clr>(), Rook_with_color<clr>());
                put_piece<clr>(rook_short_castling_to_sq<clr>(), Rook_with_color<clr>());
            }
            else{
                remove_piece<clr>(rook_long_castling_from_sq<clr>(), Rook_with_color<clr>());
                put_piece<clr>(rook_long_castling_to_sq<clr>(), Rook_with_color<clr>());
            }
            
        }
        
        template<Color clr>
        inline void do_promotion_put(const Move_full_info move)noexcept{
            replace_if_present_or_put_piece<clr>(move.to_square, promotion_flag_to_piece<clr>(move.promotion));
        }
        
        template<Color clr>
        inline void make_move(const Move_full_info move, const Piece piece)noexcept{
            //const Piece piece = mailbox[move.from_square];
            remove_piece<clr>(move.from_square, piece);
            switch(move.special){
                case No_special:
                    replace_if_present_or_put_piece<clr>(move.to_square, piece);
                    break;
                case SP_Promotion:
                    do_promotion_put<clr>(move);
                    break;
                case SP_castling:
                    do_rook_castling<clr>(move.to_square);
                    break;
                case SP_en_passant:
                    do_en_passant_remove<clr>(move.to_square);
                    put_piece<clr>(move.to_square, piece);
            }
            Not_free = White_brd | Black_brd;
        }
        
        
        template<Color clr>
        inline void undo_en_passant_remove(const Square square){
            if constexpr(clr){
                put_piece<change_color<clr>()>(square - 8, B_Pawn);
            }
            else{
                put_piece<change_color<clr>()>(square + 8, W_Pawn);
            }
        }
        
        template<Color clr>
        inline void undo_castling_rook_move(const Square to_square)noexcept{
            /// NOTE: King is already removed
            if constexpr(clr){
                if(to_square == SQ_G1){
                    put_piece<clr>(SQ_H1, W_Rook);
                    remove_piece<clr>(SQ_F1, W_Rook);
                }
                else{
                    put_piece<clr>(SQ_A1, W_Rook);
                    remove_piece<clr>(SQ_D1, W_Rook);
                }
            }
            else{
                if(to_square == SQ_G8){
                    put_piece<clr>(SQ_H8, B_Rook);
                    remove_piece<clr>(SQ_F8, B_Rook);
                }
                else{
                    put_piece<clr>(SQ_A8, B_Rook);
                    remove_piece<clr>(SQ_D8, B_Rook);
                }
            }
        }
        
        template<Color clr>
        inline void try_to_restore(const Square square, const Piece piece)noexcept{
            if(piece != No_Piece){
                put_piece<change_color<clr>()>(square, piece);
            }
        }
        
        template<Color clr>
        inline void unmake_move(const Move_full_info move, const Piece piece, const Piece restore_piece)noexcept{
            //const Piece piece = mailbox[move.from_square];
            remove_piece<clr>(move.to_square);
            switch(move.special){
                case SP_en_passant:
                    undo_en_passant_remove<clr>(move.to_square);
                    break;
                case SP_castling:
                    undo_castling_rook_move<clr>(move.to_square);
            }
            put_piece<clr>(move.from_square, piece);
            try_to_restore<clr>(move.to_square, restore_piece);
            Not_free = White_brd | Black_brd;
        }
        
        
        
        template<Color clr>
        inline void print()noexcept{
            if constexpr(clr){
                for(int i = 7; i >= 0; --i){
                    for(int j = 0; j < 8; ++j){
                        std::cout << to_symbol(mailbox[id_at_ij(i, j)]);
                    }
                    std::cout << '\n';
                }
            }
            else{
                for(int i = 0; i < 8; ++i){
                    for(int j = 7; j >= 0; --j){
                        std::cout << to_symbol(mailbox[id_at_ij(i, j)]);
                    }
                    std::cout << '\n';
                }
            }
        }
        
        inline void clear_boards()noexcept{
            for(int i = 0; i < 12; ++i){
                bit_boards[i] = 0;
            }
            White_brd = 0;
            Black_brd = 0;
            Not_free = 0;
            en_passant_take_square = No_Square;
            castle_rights = static_cast<CastlingRights>(0);
            half_moves = 0;
            for(int i = 0; i < 64; ++i){
                mailbox[i] = No_Piece;
            }
            fifty_moves_rule = 0;
            move_number = 0;
        }
        
        inline void parse_from_FEN(const std::string FEN)noexcept{
            clear_boards();
            int i = 7, j = 0;
            const int size = FEN.size();
            int id = 0;
            while((id < size) && (FEN[id] != ' ')) {
                const char symb = FEN[id++];
                Piece piece = char_to_piece(symb);
                if(piece != No_Piece){
                    Color clr = piece_color(piece);
                    if(clr){
                        put_piece<Color_White>(static_cast<Square>(id_at_ij(i, j)), piece);   
                    }
                    else{
                        put_piece<Color_Black>(static_cast<Square>(id_at_ij(i, j)), piece);
                    }
                    ++j;
                }
                else{
                    if(symb == '/'){
                        --i;
                        j = 0;
                    }
                    else{
                        j += symb - '0';
                    }
                }
                j %= 8;
            }
            while((id < size) && (FEN[id] == ' ')){
                ++id;
            }
            
            switch(FEN[id]){
                case 'w':
                    who_to_move = Color_White;
                    break;
                case 'b':
                    who_to_move = Color_Black;
            }
            while((id < size) && (FEN[id] == ' ')){
                ++id;
            }
            while((id < size) && (FEN[id] != ' ')){
                switch(FEN[id]){
                    case 'K':
                        add_castle_right(castle_rights, White_OO);
                        break;
                    case 'Q':
                        add_castle_right(castle_rights, White_OOO);
                        break;
                    case 'k':
                        add_castle_right(castle_rights, Black_OO);
                        break;
                    case 'q':
                        add_castle_right(castle_rights, Black_OOO);
                }
                ++id;
            }
            while((id < size) && (FEN[id] == ' ')){
                ++id;
            }
            
            if(FEN[id] == '-'){
                ++id;
                en_passant_take_square = No_Square;
            }
            else{
                en_passant_take_square = static_cast<Square>((FEN[id] - 'a') + (FEN[id + 1] - '1') * 8);
                id += 2;
            }
            
            while((id < size) && (FEN[id] == ' ')){
                ++id;
            }
            while((id < size) && (FEN[id] != ' ')){
                fifty_moves_rule *= 10;
                fifty_moves_rule += FEN[id] - '0';
                ++id;
            }
            
            
            while((id < size) && (FEN[id] == ' ')){
                ++id;
            }
            while((id < size) && (FEN[id] != ' ')){
                move_number *= 10;
                move_number += FEN[id] - '0';
                ++id;
            }
            
            
            Not_free = Black_brd | White_brd;
        }
        
        
        template<Color clr>
        inline u64 gen_check_mask(){
            /// TODO: WTF where are pawns checks
            const int id = bitscan(My_King<clr>());
            const u64 rook_attack = get_rook_attack_mask(Not_free, id);
            const u64 rook_attackers = rook_attack & (Enemy_Rooks<clr>() | Enemy_Queens<clr>());
            const u64 bishop_attack = get_bishop_attack_mask(Not_free, id);
            const u64 bishop_attackers = bishop_attack & (Enemy_Bishops<clr>() | Enemy_Queens<clr>());
            const u64 knight_attackers = knights_moves[id] & Enemy_Knights<clr>();
            const u64 pawn_attackers = get_pawn_attack<clr>(id) & Enemy_Pawns<clr>();
            const u64 attackers = rook_attackers | bishop_attackers | knight_attackers | pawn_attackers;
            if(attackers == 0)return 0xffffffffffffffffull;
            else if(std::popcount(attackers) >= 2)return 0;
            return get_check_pin_mask_aggressor_defender(bitscan(attackers), id);
        }

        template<Color clr>
        inline u64 gen_pin_HV_mask(){
            u64 HV_pin_mask = 0;
            const int King_id = bitscan(My_King<clr>());
            const u64 rook_attack = get_rook_attack_mask(Enemy_Board<clr>(), King_id);
            u64 rook_attackers = rook_attack & (Enemy_Rooks<clr>() | Enemy_Queens<clr>());


            forMask(rook_attackers){
                const u64 pseudo_pin_mask = get_check_pin_mask_aggressor_defender(bitscan(rook_attackers), King_id);
                if(std::popcount(pseudo_pin_mask & My_Board<clr>()) == 1){
                    HV_pin_mask |= pseudo_pin_mask;
                }
            }
            return HV_pin_mask;
        }

        template<Color clr>
        inline u64 gen_pin_DGL_mask(){
            u64 DGL_pin_mask = 0;
            const int King_id = bitscan(My_King<clr>());
            const u64 bishop_attack = get_bishop_attack_mask(Enemy_Board<clr>(), King_id);
            u64 bishop_attackers = bishop_attack & (Enemy_Bishops<clr>() | Enemy_Queens<clr>());


            forMask(bishop_attackers){
                const u64 pseudo_pin_mask = get_check_pin_mask_aggressor_defender(bitscan(bishop_attackers), King_id);
                if(std::popcount(pseudo_pin_mask & My_Board<clr>()) == 1){
                    DGL_pin_mask |= pseudo_pin_mask;
                }
            }
            return DGL_pin_mask;
        }
        
        template<Color clr>
        inline u64 gen_attacked_mask_by_Color(){
            u64 mask = 0;
            forBits(temp_mask, My_Knights<clr>()){
                mask |= knights_moves[bitscan(temp_mask)];
            }
            
            forBits(temp_mask, My_King<clr>()){
                mask |= king_moves[bitscan(temp_mask)];
            }
        
            forBits(temp_mask, My_Pawns<clr>()){
                mask |= get_pawn_attack<clr>(bitscan(temp_mask));
            }
            

            forBits(temp_mask, My_Rooks<clr>() | My_Queens<clr>()){
                mask |= get_rook_attack_mask(Not_free, bitscan(temp_mask));
            }
        

            forBits(temp_mask, My_Bishops<clr>() | My_Queens<clr>()){
                mask |= get_bishop_attack_mask(Not_free, bitscan(temp_mask));
            }
            
            return mask;
        }

        
        template<Color clr>
        GameState gen_moves(Movelist_ref movelist){
            const u64 attacked_mask = gen_attacked_mask_by_Color<clr>();
            /// NOTE: maybe check_mask is get by attacked_mask?  
            //To do at least working than optimizations
            const u64 check_mask = gen_check_mask<clr>();
            //0xffffffffffffffffull
            if(check_mask == 0xffffffffffffffffull){
                //No check
                const u64 HV_pin_mask = gen_pin_HV_mask<clr>();
                const u64 DGL_pin_mask = gen_pin_DGL_mask<clr>();
            }
            else if(check_mask != 0){
                //One piece check
                const u64 HV_pin_mask = gen_pin_HV_mask<clr>();
                const u64 DGL_pin_mask = gen_pin_DGL_mask<clr>();
                const u64 Not_pined = ~(HV_pin_mask | DGL_pin_mask);
                const int King_id = bitscan(My_King<clr>());


                /// NOTE: moves or captures to 1st or 8th rank?
                /// NOTE: create 2 different handling for it? 

                /// NOTE: en passant for check by a pawn? 

                if((en_passant_take_square != No_Square) && 
                  ((get_pawn_attack<clr>(King_id) & Enemy_Pawns<clr>()) != 0)){
                    forBits(mask, get_pawn_attack<change_color<clr>()>(en_passant_take_square) & My_Pawns<clr>()){
                        movelist.add_en_passant<clr>(bitscan(mask), en_passant_take_square);
                    }
                }

                //movable pawns
                forBits(mask, My_Pawns<clr>() & Not_pined){
                    const int id = bitscan(mask);
                    movelist.add_plain_moves(id, get_pawn_attack<clr>(id) & check_mask & Enemy_Board<clr>());
                    movelist.add_plain_moves(id, get_pawn_moves<clr>(id) & check_mask & (~Enemy_Board<clr>()));
                }

                //movable knights
                forBits(mask, My_Knights<clr>() & Not_pined){
                    const int id = bitscan(mask);
                    movelist.add_plain_moves(id, knights_moves[id] & check_mask);
                }

                //movable bishops
                forBits(mask, (My_Bishops<clr>() | My_Queens<clr>()) & Not_pined){
                    const int id = bitscan(mask);
                    movelist.add_plain_moves(id, get_bishop_attack_mask(Not_free, id) & check_mask);
                }

                //movable rooks
                forBits(mask, (My_Rooks<clr>() | My_Queens<clr>()) & Not_pined){
                    const int id = bitscan(mask);
                    movelist.add_plain_moves(id, get_rook_attack_mask(Not_free, id) & check_mask);
                }


                /// NOTE: en pasant never can block check until it's check by the moved pawn


                /// TODO: add all pieces help when checked


                
                const u64 possible_king_moves = king_moves[King_id] & (~attacked_mask) & Enemy_or_Empty<clr>();
                movelist.add_plain_moves(King_id, possible_king_moves);
            }
            else{
                //Double check
                const int King_id = bitscan(My_King<clr>());
                const u64 possible_king_moves = king_moves[King_id] & (~attacked_mask) & Enemy_or_Empty<clr>();
                movelist.add_plain_moves(King_id, possible_king_moves);
                /// TODO: create function gen_king_moves_from_mask(Movelist_ref, King_id, moves_mask) 
            }
            if((My_King<clr>() & attacked_mask) != 0){
                if(movelist.no_moves()){
                    return ST_Checkmate;
                }
                return ST_Check;
            }
            return No_state;
        }
        /*template<Color clr>
        inline bool isLegal()noexcept{
            const u32 kingId = clr ? Bitscan(Wking) : Bitscan(Bking);
            
            if(clr){
                if ((Wking & KingMoves[Bitscan(Bking)]) || (KnightsMoves[kingId] & Bknights) ) {
                    return false;
                }
                const u64 rookAttackMask = getRookAttackMask(NotFree, kingId);
                const u64 bishopAttackMask = getBishopAttackMask(NotFree, kingId);
                if( (rookAttackMask & Brooks) || (rookAttackMask & Bqueens) 
                   || (bishopAttackMask & Bbishops) || (bishopAttackMask & Bqueens) ){
                    return false;
                }
                return true;
                //also maybe without ifs? only bool algebra
            }
            else{
                if ((Bking & KingMoves[Bitscan(Wking)]) || (KnightsMoves[kingId] & Wknights) ) {
                    return false;
                }
                const u64 rookAttackMask = getRookAttackMask(NotFree, kingId);
                const u64 bishopAttackMask = getBishopAttackMask(NotFree, kingId);
                if( (rookAttackMask & Wrooks) || (rookAttackMask & Wqueens) 
                   || (bishopAttackMask & Wbishops) || (bishopAttackMask & Wqueens) ){
                    return false;
                }
                return true;
            }
        }*/
    };
}
