#ifndef RAY_CHESS_GAME_H
#define RAY_CHESS_GAME_H

#include <string>
#include <map>

#include "pieces/Piece.h"
#include "Board.h"
#include "raylib.h"
#include "Move.h"

enum GAME_STATE {
    S_RUNNING,
    S_PROMOTION,
    S_WHITE_WINS,
    S_BLACK_WINS,
    S_STALEMATE
};

class Game {
public:
    const static int INFO_BAR_HEIGHT = 32;
    const static int WINDOW_WIDTH = 640;
    const static int WINDOW_HEIGHT = WINDOW_WIDTH + INFO_BAR_HEIGHT;
    const static int CELL_SIZE = WINDOW_WIDTH / 8;

    const static std::string ASSETS_PATH;
    const static std::string TEXTURES_PATH;
    const static std::string SOUNDS_PATH;

    const static Color LIGHT_SHADE;
    const static Color DARK_SHADE;

    Game();
    ~Game();

    void Run();
    void SwapTurns();

private:
    void LoadTextures();
    void LoadSounds();

    void HandleInput();
    void HandleInputPromotion();
    Move* GetMoveAtPosition(const Position& position);
    void DoMoveOnBoard(const Move& move);

    void CalculateAllPossibleMovements();
    void CheckForEndOfGame();
    void FilterMovesThatAttackOppositeKing();
    void FilterMovesThatLeadToCheck();
    bool IsAnyMovePossible();

    // Assets.
    std::map<std::string, Texture> textures;
    std::map<std::string, Sound> sounds;

    // Game state.
    Board board;
    PIECE_COLOR turn = PIECE_COLOR::C_WHITE;
    GAME_STATE state = GAME_STATE::S_RUNNING;

    // Selected piece/possible moves state.
    Piece* selectedPiece = nullptr;
    std::map<Piece*, std::vector<Move>> possibleMovesPerPiece;

    // Game information (current round and time).
    int round = 1;
    double time = 0;
};

#endif //RAY_CHESS_GAME_H
