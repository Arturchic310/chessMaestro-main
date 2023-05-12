#pragma once
#include "types.hpp"
namespace maestro{
    
    template<class T>
    constexpr u64 bit_at(T i){
        return (static_cast<u64>(1) << static_cast<int>(i));
    }
    
    struct square_walker{
    public:
        int file, rank;
        constexpr friend bool operator==(const square_walker& first, const square_walker& second){
            return ((first.file == second.file) && (first.rank == second.rank));
        }
        constexpr square_walker(const int _file, const int _rank): file(_file), rank(_rank) {}
        constexpr square_walker(const int id): file(id / 8), rank(id % 8) {}
        constexpr Square get_Square(){
            return static_cast<Square>(file * 8 + rank);
        }
        constexpr u64 get_mask(){
            return bit_at(file * 8 + rank);
        }
        constexpr bool is_legal(){
            return (file >= 0) && (file <= 7) && (rank >= 0) && (rank <= 7);
        }
        constexpr bool is_legal_and_not_on_border(){
            return (file >= 1) && (file <= 6) && (rank >= 1) && (rank <= 6);
        }
        constexpr std::tuple<int, int> get_files_and_ranks(){
            return {file, rank};
        }
        constexpr void step_North(){
            ++file;
        }
        constexpr void step_South(){
            --file;
        }
        constexpr void step_West(){
            --rank;
        }
        constexpr void step_East(){
            ++rank;
        }
        constexpr void step_Northwest(){
            ++file;
            --rank;
        }
        constexpr void step_Northeast(){
            ++file;
            ++rank;
        }
        constexpr void step_Southwest(){
            --file;
            --rank;
        }
        constexpr void step_Southeast(){
            --file;
            ++rank;
        }
    };
    
    
    void print_board(u64 board){
        for(int i = 0; i < 8; ++i){
            u64 temp = board >> 56;
            for(int j = 0; j < 8; ++j){
                std::cout << ((temp & 1) ? "1" : ".");
                temp >>= 1;
            }
            board <<= 8;
            std::cout << '\n';
        }
    }
    
    
    consteval std::array<u64, 8> gen_ranks(){
        std::array<u64, 8> arr;
        arr.fill(0);
        for(int i = 0; i < 8; ++i){
            arr[i] = 0;
            for(int j = 0; j < 8; ++j){
                arr[i] |= bit_at(i * 8 + j);
            }
        }
        return arr;
    }
    consteval std::array<u64, 8> gen_files(){
        std::array<u64, 8> arr;
        arr.fill(0);
        for(int i = 0; i < 8; ++i){
            for(int j = 0; j < 8; ++j){
                arr[i] |= bit_at(i + j * 8);
            }
        }
        return arr;
    }
    
    constexpr bool is_legal(const int i, const int j){
        return (i >= 0) && (i < 8) && (j >= 0) && (j < 8);
    }
    
    consteval bool is_legal_and_not_on_border(int i, int j){
        return ((i > 0) && (i < 7) && (j > 0) && (j < 7));
    }
    
    constexpr u64 mask_at_ij(const int i, const int j){
        return (static_cast<u64>(1) << (i * 8 + j));
    }
    constexpr u64 mask_at_ij_if_legal(const int i, const int j){
        if(is_legal(i, j))return mask_at_ij(i, j);
        return 0;
    }
    constexpr int id_at_ij(const int i, const int j){
        return (i * 8 + j);
    }
    consteval u64 all_knights_masks_at_ij(const int i, const int j){
        u64 mask = mask_at_ij_if_legal(i + 2, j + 1) | mask_at_ij_if_legal(i - 2, j + 1)
        | mask_at_ij_if_legal(i + 2, j - 1) | mask_at_ij_if_legal(i - 2, j - 1)
        | mask_at_ij_if_legal(i + 1, j + 2) | mask_at_ij_if_legal(i - 1, j + 2)
        | mask_at_ij_if_legal(i + 1, j - 2) | mask_at_ij_if_legal(i - 1, j - 2);
        return mask;
    }
    consteval std::array<u64, 64> gen_knights_move_mask(){
        std::array<u64, 64> knights;
        knights.fill(0);
        for(int i = 0; i < 8; ++i){
            for(int j = 0; j < 8; ++j){
                int id = id_at_ij(i, j);
                knights[id] = all_knights_masks_at_ij(i, j); 
            }
        }
        return knights;
    }
    consteval std::array<u64, 56> gen_white_pawns_move_mask(){
        std::array<u64, 56> pawns;
        pawns.fill(0);
        for(int i = 0; i < 8; ++i){
            pawns[id_at_ij(1, i)] |= mask_at_ij(3, i);
        }
        for(int i = 1; i < 7; ++i){
            for(int j = 0; j < 8; ++j){
                pawns[id_at_ij(i, j)] |= mask_at_ij(i + 1, j);
            }
        }
        return pawns;
    }
    consteval std::array<u64, 56> gen_black_pawns_move_mask(){
        std::array<u64, 56> pawns;
        pawns.fill(0);
        for(int i = 0; i < 8; ++i){
            pawns[id_at_ij(6, i)] |= mask_at_ij(4, i);
        }
        for(int i = 1; i < 7; ++i){
            for(int j = 0; j < 8; ++j){
                pawns[id_at_ij(i, j)] |= mask_at_ij(i - 1, j);
            }
        }
        return pawns;
    }
    consteval std::array<u64, 56> gen_white_pawns_attack_mask(){
        std::array<u64, 56> pawns;
        pawns.fill(0);
        for(int i = 1; i < 7; ++i){
            for(int j = 0; j < 8; ++j){
                pawns[id_at_ij(i, j)] |= mask_at_ij_if_legal(i + 1, j - 1) | mask_at_ij_if_legal(i + 1, j + 1);
            }
        }
        return pawns;
    }
    consteval std::array<u64, 56> gen_black_pawns_attack_mask(){
        std::array<u64, 56> pawns;
        pawns.fill(0);
        for(int i = 1; i < 7; ++i){
            for(int j = 0; j < 8; ++j){
                pawns[id_at_ij(i, j)] |= mask_at_ij_if_legal(i - 1, j - 1) | mask_at_ij_if_legal(i - 1, j + 1);
            }
        }
        return pawns;
    }
    consteval u64 gen_king_move_mask_at_ij(const int i, const int j){
        u64 mask = 
        mask_at_ij_if_legal(i + 1, j + 1) | mask_at_ij_if_legal(i + 1, j) | mask_at_ij_if_legal(i + 1, j - 1)
        | mask_at_ij_if_legal(i, j + 1) | mask_at_ij_if_legal(i, j - 1)
        | mask_at_ij_if_legal(i - 1, j + 1) | mask_at_ij_if_legal(i - 1, j) | mask_at_ij_if_legal(i - 1, j - 1);
        return mask;
    }
    consteval std::array<u64, 64> gen_king_move_mask(){
        std::array<u64, 64> king;
        king.fill(0);
        for(int i = 0; i < 8; ++i){
            for(int j = 0; j < 8; ++j){
                king[id_at_ij(i, j)] = gen_king_move_mask_at_ij(i, j);
            }
        }
        return king;
    }
    consteval u64 gen_rook_vision_at_ij(const int i, const int j){
        u64 mask = 0;
        for(int _i = i + 1; _i < 7; ++_i){
            mask |= mask_at_ij(_i, j);
        }
        for(int _i = i - 1; _i > 0; --_i){
            mask |= mask_at_ij(_i, j);
        }
        for(int _j = j + 1; _j < 7; ++_j){
            mask |= mask_at_ij(i, _j);
        }
        for(int _j = j - 1; _j > 0; --_j){
            mask |= mask_at_ij(i, _j);
        }
        return mask;
    }
    consteval std::array<u64, 64> gen_rook_vision(){
        std::array<u64, 64> rooks;
        rooks.fill(0);
        for(int i = 0; i < 8; ++i){
            for(int j = 0; j < 8; ++j){
                rooks[id_at_ij(i, j)] |= gen_rook_vision_at_ij(i, j);
            }
        }
        return rooks;
    }
    
    consteval u64 gen_bishop_vision_at_ij(const int i, const int j){
        u64 mask = 0;
        for(int _i = i + 1, _j = j + 1; is_legal_and_not_on_border(_i, _j); ++_i, ++_j){
            mask |= mask_at_ij(_i, _j);
        }
        for(int _i = i + 1, _j = j - 1; is_legal_and_not_on_border(_i, _j); ++_i, --_j){
            mask |= mask_at_ij(_i, _j);
        }
        for(int _i = i - 1, _j = j + 1; is_legal_and_not_on_border(_i, _j); --_i, ++_j){
            mask |= mask_at_ij(_i, _j);
        }
        for(int _i = i - 1, _j = j - 1; is_legal_and_not_on_border(_i, _j); --_i, --_j){
            mask |= mask_at_ij(_i, _j);
        }
        return mask;
    }
    consteval std::array<u64, 64> gen_bishop_vision(){
        std::array<u64, 64> bishops;
        bishops.fill(0);
        for(int i = 0; i < 8; ++i){
            for(int j = 0; j < 8; ++j){
                bishops[id_at_ij(i, j)] |= gen_bishop_vision_at_ij(i, j);
            }
        }
        return bishops;
    }
    
    consteval std::array<u64, 64> gen_bishop_magic(){
        return {};
    }
    consteval std::array<u64, 64> gen_rook_magic(){
        return {};
    }
    
    constexpr u64 rook_xray_on_square(const int i, const int j){
        u64 mask = 0;
        for(int ii = i + 1; ii < 8; ++ii){
            mask |= mask_at_ij(ii, j);
        }
        for(int ii = i - 1; ii >= 0; --ii){
            mask |= mask_at_ij(ii, j);
        }
        for(int jj = j + 1; jj < 8; ++jj){
            mask |= mask_at_ij(i, jj);
        }
        for(int jj = j - 1; jj >= 0; --jj){
            mask |= mask_at_ij(i, jj);
        }
        return mask;
    }
    
    consteval std::array<u64, 64> gen_rook_xray(){
        std::array<u64, 64> XRay;
        XRay.fill(0);
        for(int i = 0; i < 8; ++i){
            for(int j = 0; j < 8; ++j){
                XRay[id_at_ij(i, j)] = rook_xray_on_square(i, j);
            }
        }
        return XRay;
    }
    
    constexpr u64 bishop_xray_on_square(const int i, const int j){
        u64 mask = 0;
        for(int ii = i + 1, jj = j + 1; is_legal(ii, jj); ++ii, ++jj){
            mask |= mask_at_ij(ii, jj);
        }
        for(int ii = i + 1, jj = j - 1; is_legal(ii, jj); ++ii, --jj){
            mask |= mask_at_ij(ii, jj);
        }
        for(int ii = i - 1, jj = j + 1; is_legal(ii, jj); --ii, ++jj){
            mask |= mask_at_ij(ii, jj);
        }
        for(int ii = i - 1, jj = j - 1; is_legal(ii, jj); --ii, --jj){
            mask |= mask_at_ij(ii, jj);
        }
        return mask;
    }
    
    consteval std::array<u64, 64> gen_bishop_xray(){
        std::array<u64, 64> XRay;
        XRay.fill(0);
        for(int i = 0; i < 8; ++i){
            for(int j = 0; j < 8; ++j){
                XRay[id_at_ij(i, j)] = bishop_xray_on_square(i, j);
            }
        }
        return XRay;
    }
    
    constexpr std::tuple<int, int> get_i_j_of_square(const int id){
        return {id / 8, id % 8};  
    }
    
    
    constexpr u64 gen_check_pin_mask_aggressor_defender_North(square_walker aggressor, square_walker defender){
        u64 mask = 0;
        while(aggressor.is_legal()){
            mask |= aggressor.get_mask();
            aggressor.step_North();
            if(aggressor == defender){
                return mask;
            }
        }
        return 0;
    }
    
    constexpr u64 gen_check_pin_mask_aggressor_defender_South(square_walker aggressor, square_walker defender){
        u64 mask = 0;
        while(aggressor.is_legal()){
            mask |= aggressor.get_mask();
            aggressor.step_South();
            if(aggressor == defender){
                return mask;
            }
        }
        return 0;
    }
    
    constexpr u64 gen_check_pin_mask_aggressor_defender_West(square_walker aggressor, square_walker defender){
        u64 mask = 0;
        while(aggressor.is_legal()){
            mask |= aggressor.get_mask();
            aggressor.step_West();
            if(aggressor == defender){
                return mask;
            }
        }
        return 0;
    }
    
    constexpr u64 gen_check_pin_mask_aggressor_defender_East(square_walker aggressor, square_walker defender){
        u64 mask = 0;
        while(aggressor.is_legal()){
            mask |= aggressor.get_mask();
            aggressor.step_East();
            if(aggressor == defender){
                return mask;
            }
        }
        return 0;
    }
    
    
    constexpr u64 gen_check_pin_mask_aggressor_defender_Northwest(square_walker aggressor, square_walker defender){
        u64 mask = 0;
        while(aggressor.is_legal()){
            mask |= aggressor.get_mask();
            aggressor.step_Northwest();
            if(aggressor == defender){
                return mask;
            }
        }
        return 0;
    }
    
    constexpr u64 gen_check_pin_mask_aggressor_defender_Northeast(square_walker aggressor, square_walker defender){
        u64 mask = 0;
        while(aggressor.is_legal()){
            mask |= aggressor.get_mask();
            aggressor.step_Northeast();
            if(aggressor == defender){
                return mask;
            }
        }
        return 0;
    }
    
    constexpr u64 gen_check_pin_mask_aggressor_defender_Southwest(square_walker aggressor, square_walker defender){
        u64 mask = 0;
        while(aggressor.is_legal()){
            mask |= aggressor.get_mask();
            aggressor.step_Southwest();
            if(aggressor == defender){
                return mask;
            }
        }
        return 0;
    }
    
    constexpr u64 gen_check_pin_mask_aggressor_defender_Southeast(square_walker aggressor, square_walker defender){
        u64 mask = 0;
        while(aggressor.is_legal()){
            mask |= aggressor.get_mask();
            aggressor.step_Southeast();
            if(aggressor == defender){
                return mask;
            }
        }
        return 0;
    }
    
    
    
    constexpr u64 gen_check_pin_mask_aggressor_defender_on_squares(const int lion, const int prey){
        const square_walker aggressor(lion), defender(prey);
        u64 mask = 0;
        
        mask = gen_check_pin_mask_aggressor_defender_North(aggressor, defender);
        if(mask != 0)return mask; 
        
        mask = gen_check_pin_mask_aggressor_defender_South(aggressor, defender);
        if(mask != 0)return mask; 
        
        mask = gen_check_pin_mask_aggressor_defender_West(aggressor, defender);
        if(mask != 0)return mask; 
        
        mask = gen_check_pin_mask_aggressor_defender_East(aggressor, defender);
        if(mask != 0)return mask; 
        
        mask = gen_check_pin_mask_aggressor_defender_Northwest(aggressor, defender);
        if(mask != 0)return mask; 
        
        mask = gen_check_pin_mask_aggressor_defender_Northeast(aggressor, defender);
        if(mask != 0)return mask; 
        
        mask = gen_check_pin_mask_aggressor_defender_Southwest(aggressor, defender);
        if(mask != 0)return mask; 
        
        mask = gen_check_pin_mask_aggressor_defender_Southeast(aggressor, defender);
        if(mask != 0)return mask; 
        
        return bit_at(lion);
    }
    
    std::array<u64, 4096> gen_check_pin_mask_aggressor_defender(){
        std::array<u64, 4096> check_pin_mask;
        std::fill(check_pin_mask.begin(), check_pin_mask.end(), 0);
        for(int i = 0; i < 64; ++i){
            for(int j = 0; j < 64; ++j){
                check_pin_mask[(i << 6) + j] = gen_check_pin_mask_aggressor_defender_on_squares(i, j);
            }
        }
        return check_pin_mask;
    }
    
    
    constexpr std::array<u64, 8>  ranks                                 {gen_ranks()};
    constexpr std::array<u64, 8>  files                                 {gen_files()};
    constexpr std::array<u64, 64> knights_moves                         {gen_knights_move_mask()};
    constexpr std::array<u64, 56> white_pawns_moves                     {gen_white_pawns_move_mask()};
    constexpr std::array<u64, 56> black_pawns_moves                     {gen_black_pawns_move_mask()};
    constexpr std::array<u64, 56> white_pawns_attack                    {gen_white_pawns_attack_mask()};
    constexpr std::array<u64, 56> black_pawns_attack                    {gen_black_pawns_attack_mask()};
    constexpr std::array<u64, 64> king_moves                            {gen_king_move_mask()};
    constexpr std::array<u64, 64> rooks_vision                          {gen_rook_vision()};
    constexpr std::array<u64, 64> bishops_vision                        {gen_bishop_vision()};    
    constexpr std::array<u64, 64> rooks_magic                           {gen_rook_magic()};
    constexpr std::array<u64, 64> bishops_magic                         {gen_bishop_magic()};
    constexpr std::array<u64, 64> rook_xray                             {gen_rook_xray()};
    constexpr std::array<u64, 64> bishop_xray                           {gen_bishop_xray()};
    const     std::array<u64, 4096> check_pin_mask_aggressor_defender   {gen_check_pin_mask_aggressor_defender()};
    
    
    constexpr u64 get_check_pin_mask_aggressor_defender(const Square aggressor, const Square defender)noexcept{
        return check_pin_mask_aggressor_defender[ ((static_cast<int>(aggressor)) << 6) + static_cast<int>(defender)];
    }
    
    constexpr u64 get_check_pin_mask_aggressor_defender(const int aggressor, const int defender)noexcept{
        return check_pin_mask_aggressor_defender[ (aggressor << 6) + defender];
    }
    
    
    template<Color clr>
    constexpr u64 get_pawn_attack(const int id)noexcept{
        if constexpr(clr){
            return white_pawns_attack[id];
        }
        else{
            return black_pawns_attack[id];
        }
    }

    template<Color clr>
    constexpr u64 get_pawn_moves(const int id)noexcept{
        if constexpr(clr){
            return white_pawns_moves[id];
        }
        else{
            return black_pawns_moves[id];
        }
    }
    
    consteval std::array<u64, 64> gen_bishop_bit_shift(){
        std::array<u64, 64> bishops;
        bishops.fill(0);
        for(int i = 0; i < 64; ++i){
            bishops[i] = (64 - std::popcount(bishops_vision[i]));
        }
        return bishops;
    }
    consteval std::array<u64, 64> gen_rooks_bit_shift(){
        std::array<u64, 64> rooks;
        rooks.fill(0);
        for(int i = 0; i < 64; ++i){
            rooks[i] = (64 - std::popcount(rooks_vision[i]));
        }
        return rooks;
    }
    constexpr std::array<u64, 64> Bishops_bit_shift  {gen_bishop_bit_shift()};
    constexpr std::array<u64, 64> rooks_bit_shift    {gen_rooks_bit_shift()};
    //constexpr std::array<u64, 64> 
    
    inline u64 get_rook_magic(const u64 BlockedBitsMask, const u32 square)noexcept{
        return _pext_u64(BlockedBitsMask, rooks_vision[square]);
    }
    
    inline u64 get_bishop_magic(const u64 BlockedBitsMask, const u32 square)noexcept{
        return _pext_u64(BlockedBitsMask, bishops_vision[square]);
    }
    
    constexpr u64 rooks_move_mask_on_mask_id(const u64 mask, const u32 square){
        u64 retMask = 0;
        int i = square / 8;
        int j = square % 8;
        for(int newJ = j + 1; newJ < 8; ++newJ){
            retMask |= mask_at_ij(i, newJ);
            if(mask & mask_at_ij(i, newJ)) break;
        }
        for(int newJ = j - 1; newJ >= 0; --newJ){
            retMask |= mask_at_ij(i, newJ);
            if(mask & mask_at_ij(i, newJ)) break;
        }
        
        for(int newI = i + 1; newI < 8; ++newI){
            retMask |= mask_at_ij(newI, j);
            if(mask & mask_at_ij(newI, j)) break;
        }
        for(int newI = i - 1; newI >= 0; --newI){
            retMask |= mask_at_ij(newI, j);
            if(mask & mask_at_ij(newI, j)) break;
        }
        return retMask;
    }
    
    constexpr u64 bishops_move_mask_on_mask_id(u64 mask, u32 square){
        u64 retMask = 0;
        int i = square / 8;
        int j = square % 8;
        
        for(int newI = i + 1, newJ = j + 1; (newI < 8) && (newJ < 8); ++newI, ++newJ){
            retMask |= mask_at_ij(newI, newJ);
            if(mask & mask_at_ij(newI, newJ)) break;
        }
        
        for(int newI = i + 1, newJ = j - 1; (newI < 8) && (newJ >= 0); ++newI, --newJ){
            retMask |= mask_at_ij(newI, newJ);
            if(mask & mask_at_ij(newI, newJ)) break;
        }
        
        for(int newI = i - 1, newJ = j + 1; (newI >= 0) && (newJ < 8); --newI, ++newJ){
            retMask |= mask_at_ij(newI, newJ);
            if(mask & mask_at_ij(newI, newJ)) break;
        }
        
        for(int newI = i - 1, newJ = j - 1; (newI >= 0) && (newJ >= 0); --newI, --newJ){
            retMask |= mask_at_ij(newI, newJ);
            if(mask & mask_at_ij(newI, newJ)) break;
        }
        
        return retMask;
    }
    
    constexpr u64 pdep(u64 val, u64 mask) {
        u64 res = 0;
        for (u64 bb = 1; mask; bb += bb) {
            if (val & bb)
                res |= mask & -mask;
            mask &= mask - 1;
        }
        return res;
    }
    
    constexpr std::array<std::array<u64, 4096>, 64> gen_rooks_attacks(){
        std::array<std::array<u64, 4096>, 64> attaks;
        for(auto &i : attaks){
            i.fill(0);
        }
        for(int i = 0; i < 64; ++i){
            u64 end = 1 <<  std::popcount(rooks_vision[i]);
            for(u64 maskId = 0; maskId < end; ++maskId){
                u64 mask = _pdep_u64(maskId, rooks_vision[i]);
                u64 moveMask = rooks_move_mask_on_mask_id(mask, i);
                attaks[i][maskId] = moveMask;
            }
        }
        return attaks;
    }
    
    constexpr std::array<std::array<u64, 512>, 64> gen_bishops_attacks(){
        std::array<std::array<u64, 512>, 64> attaks;
        for(auto &i : attaks){
            i.fill(0);
        }
        for(int i = 0; i < 64; ++i){
            u64 end = 1 <<  std::popcount(bishops_vision[i]);
            for(u64 maskId = 0; maskId < end; ++maskId){
                u64 mask = _pdep_u64(maskId, bishops_vision[i]);
                u64 moveMask = bishops_move_mask_on_mask_id(mask, i);
                attaks[i][maskId] = moveMask;
            }
        }
        return attaks;
    }
    
    
    const std::array<std::array<u64, 4096>, 64> rooks_attacks     {gen_rooks_attacks()};
    
    const std::array<std::array<u64, 512>, 64> bishops_attacks    {gen_bishops_attacks()};
    
    
    inline u64 get_rook_attack_mask(const u64 BlockedBitsMask, const u32 square){
        return rooks_attacks[square][get_rook_magic(BlockedBitsMask & rooks_vision[square], square)];
    }
    /// DONE: rename parameters
    inline u64 get_bishop_attack_mask(const u64 BlockedBitsMask, const u32 square){
        return bishops_attacks[square][get_bishop_magic(BlockedBitsMask & bishops_vision[square], square)];
    }
    
    inline u64 get_queen_attack(const u64 BlockedBitsMask, const u32 square){
        return (get_bishop_attack_mask(BlockedBitsMask, square) | get_rook_attack_mask(BlockedBitsMask, square));
    }
    
    template<Color clr>
    constexpr Square rook_short_castling_from_sq(){
        if constexpr(clr){
            return SQ_H1;
        }
        else{
            return SQ_H8;
        }
    }

    template<Color clr>
    constexpr Square rook_short_castling_to_sq(){
        if constexpr(clr){
            return SQ_F1;
        }
        else{
            return SQ_F8;
        }
    }

    template<Color clr>
    constexpr Square rook_long_castling_from_sq(){
        if constexpr(clr){
            return SQ_A1;
        }
        else{
            return SQ_A8;
        }
    }

    template<Color clr>
    constexpr Square rook_long_castling_to_sq(){
        if constexpr(clr){
            return SQ_D1;
        }
        else{
            return SQ_D8;
        }
    }

    template<Color clr>
    constexpr Square king_short_castling_to_sq(){
        if constexpr(clr){
            return SQ_G1;
        }
        else{
            return SQ_G8;
        }
    }

    //constexpr std::array<u64, 64> BishopsMagic      {genBishopMagic()};
    template<int size>
    void parse_to_c_array(const std::array<u64, size> &arr, const std::string nameOfArr){
        std::cout << "constexpr u64" << nameOfArr << "[" << size << "] = {";
        std::cout << arr[0];
        for(int i = 1; i < size; ++i){
            std::cout << ", " << arr[i];
        }
        std::cout << "};\n";
    }
    
    
    
    
}
