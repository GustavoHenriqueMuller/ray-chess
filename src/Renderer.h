#ifndef RAY_CHESS_RENDERER_H
#define RAY_CHESS_RENDERER_H

#include "raylib.h"
#include "pieces/Piece.h"

class Renderer {
public:
    static void RenderBackground();
    static void RenderPieces(const Board& board);
    static void RenderMovesSelectedPiece(const std::vector<Move>& possibleMoves);
    static void RenderGuideText();
    static void RenderPromotionScreen(const std::map<std::string, Texture>& textures, Piece::COLOR colorOfPeonBeingPromoted);

private:
    static Color GetShadeColor(Piece::COLOR color);
    static Piece::COLOR GetColorOfCell(const Position& cellPosition);
};

#endif //RAY_CHESS_RENDERER_H
