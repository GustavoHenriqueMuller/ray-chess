#ifndef RAY_CHESS_RENDERER_H
#define RAY_CHESS_RENDERER_H

#include "raylib.h"
#include "pieces/Piece.h"

class Renderer {
public:
    static void Clear();
    static void RenderBackground();
    static void RenderPieces(const Board& board, const std::map<std::string, Texture>& textures);
    static void RenderMovesSelectedPiece(const std::map<std::string, Texture>& textures, const std::vector<Move>& possibleMoves);
    static void RenderGuideText();
    static void RenderPromotionScreen(const std::map<std::string, Texture>& textures, PIECE_COLOR colorOfPeonBeingPromoted);
    static void RenderInfoBar(int round, double time);

private:
    static std::string GetTextureNameFromMoveType(Move::TYPE moveType);
    static Color GetShadeColor(PIECE_COLOR color);
    static PIECE_COLOR GetColorOfCell(const Position& cellPosition);
};

#endif //RAY_CHESS_RENDERER_H
