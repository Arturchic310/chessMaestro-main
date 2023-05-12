#pragma once
#include<cstdint>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <tuple>

#ifdef MAESTRO_DEBUG
    #include<debug/string>
    #include<debug/array>
    #include<debug/map>
#else
    #include<string>
    #include<array>
    #include<map>
#endif



namespace chess
{
    namespace types{
        #ifdef MAESTRO_DEBUG
            using string = __gnu_debug::string;
            template<typename _Tp, std::size_t _Nm>
            using array = std::__debug::array<_Tp, _Nm>;
            template<typename _Key, typename _Tp, typename _Compare = std::less<_Key>,
	   typename _Allocator = std::allocator<std::pair<const _Key, _Tp> > >
            using map = std::__debug::map<_Key, _Tp, _Compare, _Allocator>;
        #else
            using string = std::string;
            template<typename _Tp, std::size_t _Nm>
            using array = std::array<_Tp, _Nm>;
            template<typename _Key, typename _Tp, typename _Compare = std::less<_Key>,
	   typename _Allocator = std::allocator<std::pair<const _Key, _Tp> > >
            using map = std::map<_Key, _Tp, _Compare, _Allocator>;
        #endif
    }

    using uint = unsigned int;
    using u8 = uint8_t;
    using i8 = int8_t;
    using u64 = uint64_t;
    using Move = uint16_t;


    enum Color : bool
    {
        White = true,
        Black = false
    };

    enum Direction : int
    {
        Up = 8,
        Down = -8,
        Left = -1,
        Right = 1,
        LeftUp = 7,
        RightUp = 9,
        LeftDown = -9,
        RightDown = -7,

        No_Direction = 0
    };

    constexpr Direction operator-(const Direction direction){
        return static_cast<Direction>(-static_cast<int>(direction));
    }

    enum PositionState : int{
        quite = 0,
        check = 1,
        double_check = 2    
    };
    enum CastlingRights : int
    {
        NO_Castling = 0,
        White_OO = 1,
        White_OOO = 2,
        Black_OO = 4,
        Black_OOO = 8,
        
        White_OO_OOO = White_OO | White_OOO,
        Black_OO_OOO = Black_OO | Black_OOO
    };

    template<Color clr>
    consteval CastlingRights short_castling_with_color(){
        if constexpr(clr)
            return White_OO;
        else
            return Black_OO;
    }
    
    template<Color clr>
    consteval CastlingRights long_castling_with_color(){
        if constexpr(clr)
            return White_OOO;
        else
            return Black_OOO;
    }

    constexpr CastlingRights& operator|=(CastlingRights& rights, const CastlingRights& add){
        rights = static_cast<CastlingRights>(rights | add);
        return rights;
    }
    constexpr CastlingRights& operator&=(CastlingRights& rights, const CastlingRights& add){
        rights = static_cast<CastlingRights>(rights & add);
        return rights;
    }

    

    constexpr CastlingRights operator|(const CastlingRights& right, const CastlingRights& left){
        return static_cast<CastlingRights>(static_cast<int>(right) | static_cast<int>(left));
    }

    constexpr int real_board_id(const int id){
        return ((7 - (id / 8)) * 8) + (id % 8);  
    }
    
    constexpr int vector_to_id(const int rank, const int file){
        return (rank << 3) | file;
    }

    constexpr bool contains_mask(const int num, const int mask){
        return (num & mask) == mask;
    }

    

    

    enum Piece : int
    {
        W_Pawn = 0,
        W_Knight = 1,
        W_Bishop = 2,
        W_Rook = 3,
        W_Queen = 4,
        W_King = 5,

        B_Pawn = 6,
        B_Knight = 7,
        B_Bishop = 8,
        B_Rook = 9,
        B_Queen = 10,
        B_King = 11,

        No_Piece = 12
    };

    enum PieceType: int{
        Pawn = 0,
        Knight,
        Bishop,
        Rook,
        Quuen,
        King,

    };


    enum Square : int
    {
        SQ_A1 = 0,SQ_B1,SQ_C1,SQ_D1,SQ_E1,SQ_F1,SQ_G1,SQ_H1,
        SQ_A2,SQ_B2,SQ_C2,SQ_D2,SQ_E2,SQ_F2,SQ_G2,SQ_H2,
        SQ_A3,SQ_B3,SQ_C3,SQ_D3,SQ_E3,SQ_F3,SQ_G3,SQ_H3,
        SQ_A4,SQ_B4,SQ_C4,SQ_D4,SQ_E4,SQ_F4,SQ_G4,SQ_H4,
        SQ_A5,SQ_B5,SQ_C5,SQ_D5,SQ_E5,SQ_F5,SQ_G5,SQ_H5,
        SQ_A6,SQ_B6,SQ_C6,SQ_D6,SQ_E6,SQ_F6,SQ_G6,SQ_H6,
        SQ_A7,SQ_B7,SQ_C7,SQ_D7,SQ_E7,SQ_F7,SQ_G7,SQ_H7,
        SQ_A8,SQ_B8,SQ_C8,SQ_D8,SQ_E8,SQ_F8,SQ_G8,SQ_H8,

        No_Square
    };

    enum Promotion : u8
    {
        No_promotion = 0,
        promote_to_knight = 0,
        promote_to_bishop,
        promote_to_rook,
        promote_to_queen
    };

    enum Special : u8
    {
        No_special = 0,
        SP_Promotion,
        SP_en_passant,
        SP_castling
    };


    template<Color color>
    consteval Square short_castling_square(){
        if constexpr(color)
            return SQ_G1;
        else 
            return SQ_G8;
    }

    template<Color color>
    consteval Square long_castling_square(){
        if constexpr(color)
            return SQ_C1;
        else 
            return SQ_C8;
    }

    template<Color color>
    consteval Square long_castling_rook_from_square(){
        if constexpr(color)
            return SQ_A1;
        else 
            return SQ_A8;
    }

    template<Color color>
    consteval Square short_castling_rook_from_square(){
        if constexpr(color)
            return SQ_H1;
        else 
            return SQ_H8;
    }


    template<Color color>
    consteval Square long_castling_rook_to_square(){
        if constexpr(color)
            return SQ_D1;
        else 
            return SQ_D8;
    }

    template<Color color>
    consteval Square short_castling_rook_to_square(){
        if constexpr(color)
            return SQ_F1;
        else 
            return SQ_F8;
    }

    template<Color color> 
    consteval Square castling_king_from_square(){
        if constexpr(color)
            return SQ_E1;
        else
            return SQ_E8;
    }

    template<Color color> 
    consteval Square short_castling_king_to_square(){
        if constexpr(color)
            return SQ_G1;
        else
            return SQ_G8;
    }

    template<Color color> 
    consteval Square long_castling_king_to_square(){
        if constexpr(color)
            return SQ_C1;
        else
            return SQ_C8;
    }


    template <Color color>
    consteval Direction pawn_move_dir() 
    {
        if constexpr (color)
            return Up;
        else
            return Down;
    }

    constexpr uint64_t bit_at(const uint i)
    {
        return static_cast<uint64_t>(1) << i;
    }

    constexpr bool contains(const auto source, const auto mask)
    {
        return (source & mask);
    }

    inline types::string square_to_str(const Square sq){
        types::string str; 
        str += static_cast<char>((sq % 8) + 'a');
        str += static_cast<char>((sq / 8) + '1');
        return str;
    }

    
    

    consteval Color change_color(Color color){
        return static_cast<Color>(!color);
    }

    template<Color color>
    consteval Piece Pawn_with_color(){
        if constexpr(color){
            return W_Pawn;
        }
        else{
            return B_Pawn;
        }
    }
    
    template<Color color>
    consteval Piece Knight_with_color(){
        if constexpr(color){
            return W_Knight;
        }
        else{
            return B_Knight;
        }
    }

    template<Color color>
    consteval Piece Bishop_with_color(){
        if constexpr(color){
            return W_Bishop;
        }
        else{
            return B_Bishop;
        }
    }

    template<Color color>
    consteval Piece Rook_with_color(){
        if constexpr(color){
            return W_Rook;
        }
        else{
            return B_Rook;
        }
    }

    template<Color color>
    consteval Piece Queen_with_color(){
        if constexpr(color){
            return W_Queen;
        }
        else{
            return B_Queen;
        }
    }

    template<Color color>
    consteval Piece King_with_color(){
        if constexpr(color){
            return W_King;
        }
        else{
            return B_King; 
        }
    }

    template<Color color>
    consteval int pawn_double_move_distance(){
        if constexpr(color)
            return 16;
        else 
            return -16;
    }
    

    /// @brief get en_passant direction by color from from_square
    template<Color color>
    consteval int pawn_en_passant_direction_from(){
        if constexpr(color)
            return 8;
        else 
            return -8;
    }

    template<Color color>
    consteval int pawn_en_passant_direction_to(){
        if constexpr(color)
            return -8;
        else 
            return 8;
    }
    inline Color piece_color(const Piece piece)
    {
        return static_cast<Color>(piece < 6);
    }
    


    struct Move_full_info{
        u8 from_square, to_square;
        Promotion promotion;
        Special special;
        

        constexpr Move_full_info():from_square(No_Square), to_square(No_Square), promotion(No_promotion), special(No_special){}
        constexpr Move_full_info(u8 _from_square, u8 _to_square, Promotion _promotion, Special _special)noexcept : 
            from_square(_from_square), 
            to_square(_to_square),
            promotion(_promotion),
            special(_special){}

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

        friend constexpr bool operator<(const Move_full_info& left, const Move_full_info& right){
            return false;
        }

        friend constexpr bool operator==(const Move_full_info& left, const Move_full_info& right){
            return (left.from_square == right.from_square) && (left.to_square == right.to_square)
                && (left.promotion == right.promotion) && (left.special == right.special);
        }

        constexpr bool is_no_move(){
            return from_square == No_Square; 
        }
    };

    constexpr Move_full_info No_Move{};

    template<uint _size>
    struct Movelist{
        public: 
        types::array<Move_full_info, _size> arr;
        int count;
        constexpr Movelist(): count(0) {}
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

    struct Accumulator{
        u8 castling_rights, white_king_position, black_king_position, en_passant, piece_to_revive;
    };

    struct Movelist_ref{
        public: 
        Move_full_info *begin;
        Move_full_info *end;
        constexpr Movelist_ref(Move_full_info *arr): begin(arr), end(arr) {}
        template<uint _size>
        constexpr Movelist_ref(Movelist<_size>& _list):begin(_list.arr.begin()), end(_list.arr.begin()){}

        constexpr const Move_full_info& operator[](int i)const{
            return begin[i];
        }

        constexpr  Move_full_info& operator[](int i){
            return begin[i];
        }


        constexpr void add(const Move_full_info& move){
            *(end++) = move;
        }
        constexpr Move_full_info peek()const{
            return *end;
        }
        constexpr void pop(){
            --end;
        }
        constexpr void pop(Move_full_info &move){
            --end;
            move = *end;
        }
        //constexpr void
        constexpr bool no_moves()const{
            return begin == end;
        }
        constexpr Movelist_ref get_ref(){
            return Movelist_ref(end);
        }

        constexpr int count(){
            return end - begin;
        }

        constexpr void clear_moves(){
            begin = end;
        }
    };
}