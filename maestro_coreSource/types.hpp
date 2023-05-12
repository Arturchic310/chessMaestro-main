#pragma once
#include <bits/stdint-uintn.h>
#include <iostream>
#include <bitset>
#include <array>
#include <bit>
#include <immintrin.h>
#include <tuple>

namespace maestro{
    using u8  = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;
    
    using i8  = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;
    
    enum CastlingRights : int {
        NO_Castling = 0,
        White_OO = 1,
        White_OOO = 2,
        Black_OO = 4,
        Black_OOO = 8
    };

    


    enum Piece : int{
        W_Pawn = 0,
        W_Knight,
        W_Bishop, 
        W_Rook, 
        W_Queen,
        W_King,
        
        B_Pawn = W_Pawn + 6,
        B_Knight,
        B_Bishop, 
        B_Rook, 
        B_Queen,
        B_King,
        
        No_Piece
    };
    
    enum PieceType : int{
        Type_Pawn = 0,
        Type_Knight,
        Type_Bishop, 
        Type_Rook, 
        Type_Queen,
        Type_King,
        
        No_PieceType
    };

    enum Color : bool{
        Color_Black = false,
        Color_White = true
    };

    /*enum Piece : int{
        W_Pawn = 0,
        W_Knight,
        W_Bishop, 
        W_Rook, 
        W_Queen,
        W_King,
        
        B_Pawn = W_Pawn + 6,
        B_Knight,
        B_Bishop, 
        B_Rook, 
        B_Queen,
        B_King,
        
        No_Piece
    };*/

    

    template<Color clr>
    consteval Piece Pawn_with_color(){
        if constexpr(clr){
            return W_Pawn;
        }
        else{
            return B_Pawn;
        }
    }
    
    template<Color clr>
    consteval Piece Knight_with_color(){
        if constexpr(clr){
            return W_Knight;
        }
        else{
            return B_Knight;
        }
    }

    template<Color clr>
    consteval Piece Bishop_with_color(){
        if constexpr(clr){
            return W_Bishop;
        }
        else{
            return B_Bishop;
        }
    }

    template<Color clr>
    consteval Piece Rook_with_color(){
        if constexpr(clr){
            return W_Rook;
        }
        else{
            return B_Rook;
        }
    }

    template<Color clr>
    consteval Piece Queen_with_color(){
        if constexpr(clr){
            return W_Queen;
        }
        else{
            return B_Queen;
        }
    }

    template<Color clr>
    consteval Piece King_with_color(){
        if constexpr(clr){
            return W_King;
        }
        else{
            return B_King; 
        }
    }

    

    

    // guaranteed that it's not No_PieceType
    template<Color clr, PieceType piece_type>
    consteval Piece type_to_piece()noexcept{
        if constexpr(piece_type == No_PieceType){
            return No_Piece;
        }
        if constexpr (clr){
            return static_cast<Piece>(piece_type);
        }
        else{
            return static_cast<Piece>(static_cast<int>(piece_type) + 6);
        }
    }

    template<Color clr>
    consteval Piece My_piece(const PieceType piece_type)noexcept{
        if constexpr (clr){
            return static_cast<Piece>(piece_type);
        }
        else{
            return static_cast<Piece>(static_cast<int>(piece_type) + 6);
        }
    }

    template<Color clr>
    consteval Piece Enemy_piece(const PieceType piece_type)noexcept{
        if constexpr (clr){
            return static_cast<Piece>(static_cast<int>(piece_type) + 6);
        }
        else{
            return static_cast<Piece>(piece_type);
        }
    }
    
    enum Square : int{
        SQ_A1 = 0, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1,
        SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2,
        SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3,
        SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4,
        SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5,
        SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6,
        SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7,
        SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8,
        
        No_Square
    };
    
    template<typename T>
    constexpr Square operator+(const Square square, const T value)noexcept{
        return static_cast<Square>(static_cast<T>(square) + value);
    }
    
    template<typename T>
    constexpr Square operator-(const Square square, const T value)noexcept{
        return static_cast<Square>(static_cast<T>(square) - value);
    }

    template<typename T>
    constexpr Square operator+(const PieceType piece_type, const T value)noexcept{
        return static_cast<PieceType>(static_cast<T>(piece_type) + value);
    }
    
    template<typename T>
    constexpr Square operator-(const PieceType piece_type, const T value)noexcept{
        return static_cast<PieceType>(static_cast<T>(piece_type) - value);
    }
    
    enum Promotion : int{
        No_promotion = 0,
        promote_to_knight = 0,
        promote_to_bishop,
        promote_to_rook,
        promote_to_queen
    };
    
    enum Special : int{
        No_special = 0,
        SP_Promotion,
        SP_en_passant,
        SP_castling
    };
    
    enum GameState : int{
        No_state = 0,
        ST_Check,
        ST_Checkmate
    };
    
    
    using Move = u16;
    
    struct Move_full_info{
        u8 from_square, to_square;
        u8 promotion;
        u8 special;
        
        constexpr Move_full_info(Move move)noexcept : 
            from_square(static_cast<u8>(move & 0b111'111)), 
            to_square(static_cast<u8>((move >>= 6) & 0b111'111)),
            promotion(static_cast<u8>((move >>= 6) & 0b11)),
            special(static_cast<u8>(move >> 2)){}
        
        constexpr Move_full_info(u8 _from_square, u8 _to_square, u8 _promotion, u8 _special)noexcept : 
            from_square(_from_square), 
            to_square(_to_square),
            promotion(_promotion),
            special(_special){}

        constexpr void set_all(Move move)noexcept{
            from_square = static_cast<u8>(move & 0b111'111);
            to_square = static_cast<u8>((move >>= 6) & 0b111'111);
            promotion = static_cast<u8>((move >>= 6) & 0b11);
            special = static_cast<u8>(move >> 2);
        }
        constexpr Move to_move()const noexcept{
            Move move = static_cast<Move>(special);
            move <<= 2;
            move |= static_cast<Move>(promotion);
            move <<= 2;
            move |= static_cast<Move>(to_square);
            move <<= 6;
            move |= static_cast<Move>(from_square);
            return move;
        }
    };

    constexpr Move Create_move(const Square from_square, const Square to_square, const Promotion promotion, const Special special){
        Move move = static_cast<Move>(special);
        move <<= 2;
        move |= static_cast<Move>(promotion);
        move <<= 2;
        move |= static_cast<Move>(to_square);
        move <<= 6;
        move |= static_cast<Move>(from_square);
        return move;
    }

    constexpr Move_full_info Create_move_full_info(const Square from_square, const Square to_square, const Promotion promotion, const Special special){
        return Move_full_info(static_cast<u8>(from_square), static_cast<u8>(to_square),
                              static_cast<u8>(promotion),  static_cast<u8>(special));
    }

    template<uint _size>
    struct Movelist{
        public: 
        Move_full_info arr[_size];
        int count;
        constexpr Movelist(){}
        constexpr void push(const Move_full_info& move){
            arr[count++] = move;
        }
        constexpr Move_full_info peek(){
            return arr[count];
        }
        constexpr void pop(){
            --count;
        }
        constexpr uint size(){
            return count;
        }
    };

    inline void clear_LSB(u64 &mask)noexcept{
        //mask &= mask - 1;
        mask = _blsr_u64(mask);
    }
    
    constexpr int bitscan(const u64 mask)noexcept{
        return __builtin_ctzll(mask);
    }
    //#define bit_loop(i, given_mask) for(u64 i = __builtin_ctzll(given_mask), mask = given_mask ; mask; i = __builtin_ctzll(mask))
    /// NOTE: mask must be u64
    #define forBits(mask, source) for(u64 mask = source; mask; clear_LSB(mask))
    #define forMask(mask) for(; mask; clear_LSB(mask))

    template<Color clr>
    constexpr u64 last_rank(){
        if constexpr(clr){
            return 0xff00000000000000ull;
            /// DONE: NEED MAGIC
        }
        else{
            return 0x00000000000000ffull;
            /// DONE: NEED MAGIC
        }
    }




    struct Movelist_ref{
        public: 
        Move_full_info *begin;
        Move_full_info *end;
        constexpr Movelist_ref(Move_full_info *arr): begin(arr), end(arr) {}
        constexpr Movelist_ref(){}
        constexpr void add(const Move_full_info& move){
            *(end++) = move;
        }
        constexpr Move peek(){
            return *end;
        }
        constexpr void pop(){
            --end;
        }
        constexpr void add_plain_moves(const int from_id, u64 moves_mask){
            forMask(moves_mask){
                *(end++) = Create_move_full_info(from_id, bitscan(moves_mask), No_promotion, No_special);
            }
        }
        template<Color clr>
        constexpr void add_pawns_moves(const int from_id, u64 moves_mask){
            forMask(moves_mask){
                const int to_id = bitscan(moves_mask);
                if((to_id & last_rank<clr>()) != 0){
                    *(end++) = Create_move_full_info(from_id, to_id, promote_to_queen, SP_Promotion);
                    *(end++) = Create_move_full_info(from_id, to_id, promote_to_rook, SP_Promotion);
                    *(end++) = Create_move_full_info(from_id, to_id, promote_to_bishop, SP_Promotion);
                    *(end++) = Create_move_full_info(from_id, to_id, promote_to_knight, SP_Promotion);
                }
                else{
                    *(end++) = Create_move_full_info(from_id, bitscan(moves_mask), No_promotion, No_special);
                }
            }
        }
        constexpr void add_plain_move(const int from_id, const int to_id){
            *(end++) = Create_move_full_info(from_id, to_id, No_promotion, No_special);
        }
        template<Color clr>
        constexpr void add_en_passant(const int from_id, const int to_id){
            *(end++) = Create_move_full_info(from_id, to_id, No_promotion, SP_en_passant);
        }
        //constexpr void
        constexpr bool no_moves(){
            return begin == end;
        }
    };

    
}
