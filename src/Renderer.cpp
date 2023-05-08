#include "Renderer.h"
#include "Game.h"

void Renderer::Clear() {
    ClearBackground(WHITE);
}

void Renderer::RenderBackground() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int x = j * Game::CELL_SIZE;
            int y = i * Game::CELL_SIZE + Game::INFO_BAR_HEIGHT;

            Color cellColor = GetShadeColor(GetColorOfCell({i, j}));
            DrawRectangle(x, y, Game::CELL_SIZE, Game::CELL_SIZE, cellColor);
        }
    }
}

void Renderer::RenderPieces(const Board& board, const std::map<std::string, Texture>& textures) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = board.At({i, j});

            if (piece != nullptr) {
                int x = j * Game::CELL_SIZE;
                int y = i * Game::CELL_SIZE + Game::INFO_BAR_HEIGHT;

                DrawTexture(textures.at(piece->GetTextureName()), x, y, WHITE);
            }
        }
    }
}

void Renderer::RenderMovesSelectedPiece(const std::map<std::string, Texture>& textures, const std::vector<Move>& possibleMoves) {
    for (const Move& move : possibleMoves) {
        DrawTexture(
            textures.at(GetTextureNameFromMoveType(move.type)),
            move.position.j * Game::CELL_SIZE,
            move.position.i * Game::CELL_SIZE + Game::INFO_BAR_HEIGHT,
            WHITE
        );
    }
}

void Renderer::RenderGuideText() {
    int padding = 3;
    int characterSize = 10;

    // Render 1-8 numbers (rows).
    for (int i = 0; i < 8; i++) {
        Color textColor = GetShadeColor(Piece::GetInverseColor(GetColorOfCell({i, 0})));

        // Render text.
        int x = padding;
        int y = i * Game::CELL_SIZE + padding + Game::INFO_BAR_HEIGHT;

        char text[2];
        text[0] = 49 + i;
        text[1] = 0;

        DrawText(text, x, y, 20, textColor);
    }

    // Render h-a characters (columns).
    for (int j = 0; j < 8; j++) {
        Color textColor = GetShadeColor(Piece::GetInverseColor(GetColorOfCell({7, j})));

        // Render text.
        int x = (j + 1) * Game::CELL_SIZE - characterSize - padding;
        int y = Game::WINDOW_HEIGHT - characterSize * 1.75 - padding + Game::INFO_BAR_HEIGHT;

        char text[2];
        text[0] = 97 + (7 - j);
        text[1] = 0;

        DrawText(text, x, y, 20, textColor);
    }
}

void Renderer::RenderPromotionScreen(const std::map<std::string, Texture>& textures, PIECE_COLOR colorOfPeon) {
    DrawRectangle(0, 0, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, Color{0, 0, 0, 127});
    DrawText("Promotion", Game::WINDOW_WIDTH / 2 - 98, Game::WINDOW_HEIGHT / 4, 40, WHITE);

    std::string prefix = colorOfPeon == PIECE_COLOR::C_WHITE ? "w" : "b";

    int textureY = Game::CELL_SIZE * 3 + Game::INFO_BAR_HEIGHT;
    int textY = Game::CELL_SIZE * 4 + 5 + Game::INFO_BAR_HEIGHT;

    // Draw queen.
    {
        DrawTexture(textures.at(prefix + "q"), Game::CELL_SIZE * 2, textureY, WHITE);
        DrawText("Queen", Game::CELL_SIZE * 2 + 9, textY, 20, WHITE);
    }

    // Draw rook.
    {
        DrawTexture(textures.at(prefix + "r"), Game::CELL_SIZE * 3, textureY, WHITE);
        DrawText("Rook", Game::CELL_SIZE * 3 + 14, textY, 20, WHITE);
    }

    // Draw bishop.
    {
        DrawTexture(textures.at(prefix + "b"), Game::CELL_SIZE * 4, textureY, WHITE);
        DrawText("Bishop", Game::CELL_SIZE * 4 + 7, textY, 20, WHITE);
    }

    // Draw knight.
    {
        DrawTexture(textures.at(prefix + "n"), Game::CELL_SIZE * 5, textureY, WHITE);
        DrawText("Knight", Game::CELL_SIZE * 5 + 9, textY, 20, WHITE);
    }
}

std::string Renderer::GetTextureNameFromMoveType(Move::TYPE moveType) {
    switch (moveType) {
        case Move::TYPE::WALK:
        case Move::TYPE::DOUBLE_WALK:
        case Move::TYPE::ATTACK:
            return "move";

        case Move::TYPE::SHORT_CASTLING:
        case Move::TYPE::LONG_CASTLING:
            return "castling";

        case Move::TYPE::EN_PASSANT:
            return "enpassant";

        case Move::TYPE::PROMOTION:
        case Move::TYPE::ATTACK_AND_PROMOTION:
            return "promotion";
    }
}

Color Renderer::GetShadeColor(PIECE_COLOR color) {
    return color == PIECE_COLOR::C_WHITE ? Game::LIGHT_SHADE : Game::DARK_SHADE;
}

PIECE_COLOR Renderer::GetColorOfCell(const Position& cellPosition) {
    int startingColorInRow = cellPosition.i % 2 == 0 ? 0 : 1;
    int colorIndex = (startingColorInRow + cellPosition.j) % 2;

    return colorIndex == 0 ? PIECE_COLOR::C_WHITE : PIECE_COLOR::C_BLACK;
}

void Renderer::RenderInfoBar(int round, double time) {
    DrawRectangle(0, 0, Game::WINDOW_WIDTH, Game::INFO_BAR_HEIGHT, BLACK);

    int padding = 5;
    std::string roundText = "Round: " + std::to_string(round);
    std::string timeText = "Time: " + std::to_string((int) time) + "s";
    int timeTextWidth = MeasureText(timeText.c_str(), 20);

    DrawText(roundText.c_str(), padding, Game::INFO_BAR_HEIGHT / 2 - 10, 20, WHITE);
    DrawText(timeText.c_str(), Game::WINDOW_WIDTH - timeTextWidth - padding, Game::INFO_BAR_HEIGHT / 2 - 10, 20, WHITE);
}






