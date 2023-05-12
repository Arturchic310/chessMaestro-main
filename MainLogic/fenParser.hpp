#pragma once
#include "../Board/board.hpp"

namespace chess
{

    struct fenParser
    {
        types::map<char, Piece> fenToMap;
        types::map<Piece, char> mapToFen;

    public:
        fenParser()
        {
            init();
        }

        void parse_from_FEN(const types::string& FEN, Board &brd){
            brd.clear_board();
            std::istringstream FEN_in(FEN);
            types::string command;
            FEN_in >> command;
            parse_position(command, brd);
            //std::cout << "Position" << "\n";
            
            FEN_in >> command;
            parse_turn(command, brd);
            //std::cout << "Turn" << "\n";
            
            FEN_in >> command;
            parse_castle(command, brd);
            //std::cout << "Castle" << "\n";

            FEN_in >> command;
            parse_en_passant(command, brd);
            //std::cout << "an Puasant" << "\n";

            FEN_in >> command;
            parse_fifty_moves_rule(command, brd);
            //std::cout << "Fifty Moves" << "\n";
            
            FEN_in >> command;
            parse_total_moves(command, brd);
            //std::cout << "Total Moves" << "\n";

            brd.find_kings();
        }
        types::string parse_to_Fen(Board &brd){
            types::string result_fen;
            result_fen.reserve(95);
            result_fen += position_to_fen(brd);
            result_fen += " ";
            result_fen += turn_to_fen(brd.get_turn());
            result_fen += " ";
            result_fen += castle_to_fen(brd);
            result_fen += " ";
            result_fen += en_passant_to_fen(brd.get_en_passant());
            result_fen += " ";
            result_fen += fifty_moves_to_fen(brd.get_fifty_rule());
            result_fen += " ";
            result_fen += total_moves_to_fen(brd.get_total_moves());

            result_fen.shrink_to_fit();
            return result_fen;
        }

     private:
        void parse_position(const types::string& FEN_position, Board &brd)
        {
            int iterator = 0;
            int noPieceIterator = 0;
            for (int i = 0; i < FEN_position.size(); i++)
            {
                char currentChar = FEN_position[i];
                if (currentChar == '/')
                    continue;
                if (isalpha(currentChar))
                {
                    brd[real_board_id(iterator)] = fenToMap.at(currentChar);
                    iterator++;
                }
                else
                {
                    noPieceIterator = currentChar - '0';
                    for (int j = 0; j < noPieceIterator; j++)
                    {
                        brd[real_board_id(iterator)] = Piece::No_Piece;
                        iterator++;
                    }
                }
            }
        }
        void parse_turn(const types::string& FEN_turn, Board &brd)
        {
            bool isWhite = FEN_turn.find('w') != -1 ? true : false;

            brd.set_turn(isWhite ? White : Black);
        }
        void parse_castle(const types::string& FEN_castle, Board &brd)
        {
            brd.remove_all_castle<White>();
            brd.remove_all_castle<Black>();
            if(FEN_castle.find("Q") != -1){
                brd.add_castling(White_OOO);
            }
            if(FEN_castle.find("K") != -1){
                brd.add_castling(White_OO);            
            }
            if(FEN_castle.find("q") != -1){
                brd.add_castling(Black_OOO);
            }
            if(FEN_castle.find("k") != -1){
                brd.add_castling(Black_OO);            
            }
        }
        void parse_en_passant(const types::string& FEN_an_pausant, Board &brd)
        {
            if (FEN_an_pausant.find('-') != -1)
            {
                return;
            }

            int rank = stoi(FEN_an_pausant.substr(1)) - 1;
            int file = get_offset_from_file(FEN_an_pausant[0]);

            brd.set_en_passant(static_cast<Square>(8 * rank + file));
        }
        void parse_fifty_moves_rule(const types::string& FEN_moves, Board &brd)
        {
            types::string fifty_moves = FEN_moves.substr(0, FEN_moves.find(" "));

            int moves_fifty = stoi(fifty_moves);

            brd.set_fifty_rule(moves_fifty);
        }
        void parse_total_moves(const types::string& FEN_moves, Board &brd)
        {

            int moves_total = stoi(FEN_moves);

            brd.set_total_moves(moves_total);
        }

        types::string position_to_fen(const Board& brd){
            types::string position_to_fen = "";
            position_to_fen.reserve(85);
            constexpr int board_height = 8;
            constexpr int board_width = 8;

            for (int height = 0; height < board_height; ++height)
            {
                int empty_squares_counter = 0;
                for (int width = 0; width < board_width; ++width)
                {

                    const int real_id = real_board_id(width + board_width * height); 
                    char currentPiece = mapToFen.at(brd[real_id]);
                    if (currentPiece != '*')
                    {
                        if (empty_squares_counter != 0)
                        {
                            position_to_fen += empty_squares_counter;
                            empty_squares_counter = 0;
                        }
                        position_to_fen += currentPiece;
                    }
                    else
                    {
                        empty_squares_counter++;

                        if (empty_squares_counter == board_width)
                        {
                            position_to_fen += empty_squares_counter;
                            empty_squares_counter = 0;
                        }
                    }
                }
                if (height == 7)
                    break;
                position_to_fen += '/';
            }

            return position_to_fen;
        }
        types::string turn_to_fen(Color clr)
        {
            if (clr)
            {
                return "w";
            }
            return "b";
        }
        types::string castle_to_fen(const Board& brd){
            types::string castl_FEN;
            if(brd.has_castling(White_OO)){
                castl_FEN += 'K';
            }
            if(brd.has_castling(White_OOO)){
                castl_FEN += 'Q';
            }
            if(brd.has_castling(Black_OO)){
                castl_FEN += 'k';
            }
            if(brd.has_castling(Black_OOO)){
                castl_FEN += 'q';
            }
            return castl_FEN;
        }
        types::string en_passant_to_fen(Square en_passant)
        {
            if(en_passant == No_Square)
                return "-";

            types::string en_passant_square = "";

            int number_of_square = en_passant / 8 + 1;

            en_passant_square = get_file_from_offest(en_passant % 8);
            en_passant_square += number_of_square;

            return en_passant_square;
        }
        types::string fifty_moves_to_fen(int move)
        {
            return std::to_string(move);
        }
        types::string total_moves_to_fen(int move)
        {
            return std::to_string(move);
        }

        void init()
        {
            fenToMap.insert(std::pair<char, Piece>('r', Piece::B_Rook));
            fenToMap.insert(std::pair<char, Piece>('n', Piece::B_Knight));
            fenToMap.insert(std::pair<char, Piece>('b', Piece::B_Bishop));
            fenToMap.insert(std::pair<char, Piece>('q', Piece::B_Queen));
            fenToMap.insert(std::pair<char, Piece>('k', Piece::B_King));
            fenToMap.insert(std::pair<char, Piece>('p', Piece::B_Pawn));
            fenToMap.insert(std::pair<char, Piece>('R', Piece::W_Rook));
            fenToMap.insert(std::pair<char, Piece>('N', Piece::W_Knight));
            fenToMap.insert(std::pair<char, Piece>('B', Piece::W_Bishop));
            fenToMap.insert(std::pair<char, Piece>('Q', Piece::W_Queen));
            fenToMap.insert(std::pair<char, Piece>('K', Piece::W_King));
            fenToMap.insert(std::pair<char, Piece>('P', Piece::W_Pawn));

            mapToFen.insert(std::pair<Piece, char>(Piece::B_Rook, 'r'));
            mapToFen.insert(std::pair<Piece, char>(Piece::B_Knight, 'n'));
            mapToFen.insert(std::pair<Piece, char>(Piece::B_Bishop, 'b'));
            mapToFen.insert(std::pair<Piece, char>(Piece::B_Queen, 'q'));
            mapToFen.insert(std::pair<Piece, char>(Piece::B_King, 'k'));
            mapToFen.insert(std::pair<Piece, char>(Piece::B_Pawn, 'p'));
            mapToFen.insert(std::pair<Piece, char>(Piece::W_Rook, 'R'));
            mapToFen.insert(std::pair<Piece, char>(Piece::W_Knight, 'N'));
            mapToFen.insert(std::pair<Piece, char>(Piece::W_Bishop, 'B'));
            mapToFen.insert(std::pair<Piece, char>(Piece::W_Queen, 'Q'));
            mapToFen.insert(std::pair<Piece, char>(Piece::W_King, 'K'));
            mapToFen.insert(std::pair<Piece, char>(Piece::W_Pawn, 'P'));
            mapToFen.insert(std::pair<Piece, char>(Piece::No_Piece, '*'));
        }

        int get_offset_from_file(char file)
        {
            int id = file - 'a';
            #ifdef MAESTRO_DEBUG
            if((id < 0) || (id > 7))
                throw std::invalid_argument("Invalid file: " + file);
            #endif
            return id;
        }

        char get_file_from_offest(int offset)
        {
            #ifdef MAESTRO_DEBUG
            if((offset < 0) || (offset > 7))
                throw std::invalid_argument("Invalid offset: " + offset);
            #endif
            return offset + 'a';
        }
    };

    

    

    

    

    

    

    

   



    

    

    

    

}