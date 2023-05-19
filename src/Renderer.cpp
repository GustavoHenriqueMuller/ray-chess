#include <algorithm>
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

                DrawTexture(textures.at(piece->GetName()), x, y, WHITE);
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
        int y = Game::WINDOW_HEIGHT - characterSize * 1.75 - padding;

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

void Renderer::RenderInfoBar(int round, double time) {
    DrawRectangle(0, 0, Game::WINDOW_WIDTH, Game::INFO_BAR_HEIGHT, BLACK);

    std::string roundText = "Round: " + std::to_string(round);
    std::string timeText = "Time: " + std::to_string((int) time) + "s";

    int timeTextWidth = MeasureText(timeText.c_str(), 20);
    int padding = 5;

    DrawText(roundText.c_str(), padding, Game::INFO_BAR_HEIGHT / 2 - 10, 20, WHITE);
    DrawText(timeText.c_str(), Game::WINDOW_WIDTH - timeTextWidth - padding, Game::INFO_BAR_HEIGHT / 2 - 10, 20, WHITE);
}

void Renderer::RenderEndScreen(GAME_STATE state) {
    DrawRectangle(0, 0, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, Color{0, 0, 0, 127});

    const char* text;

    if (state == GAME_STATE::S_WHITE_WINS) {
        text = "White wins";
    } else if (state == GAME_STATE::S_BLACK_WINS) {
        text = "Black wins";
    } else if (state == GAME_STATE::S_STALEMATE) {
        text = "Stalemate";
    }

    int textLength = MeasureText(text, 40);
    DrawText(text, Game::WINDOW_WIDTH / 2 - textLength / 2, Game::WINDOW_HEIGHT / 2, 40, WHITE);
}

void Renderer::ChangeMouseCursor(const Board& board, const std::vector<Move>& possibleMoves, PIECE_COLOR turn, bool inPromotion) {
    Vector2 mousePosition = GetMousePosition();
    mousePosition.y -= Game::INFO_BAR_HEIGHT;

    Position hoverPosition = {int(mousePosition.y) / Game::CELL_SIZE, int(mousePosition.x) / Game::CELL_SIZE};

    if (!inPromotion) {
        bool isHoveringOverPiece = board.At(hoverPosition) && board.At(hoverPosition)->color == turn;
        auto it = std::find_if(possibleMoves.begin(), possibleMoves.end(), [hoverPosition](const Move& m) {
            return m.position.i == hoverPosition.i && m.position.j == hoverPosition.j;
        });

        bool isHoveringOverMove = it != possibleMoves.end();

        // Set mouse to pointer if hovering over piece or hovering over move.
        if (isHoveringOverPiece || isHoveringOverMove) {
            SetMouseCursor(4);
        } else {
            SetMouseCursor(0);
        }
    } else {
        // If in promotion screen, also set mouse to pointer if hovering over the options.
        if (hoverPosition.i == 3 && hoverPosition.j >= 2 && hoverPosition.j <= 5) {
            SetMouseCursor(4);
        }
    }
}

std::string Renderer::GetTextureNameFromMoveType(MOVE_TYPE moveType) {
    switch (moveType) {
        case MOVE_TYPE::WALK:
        case MOVE_TYPE::DOUBLE_WALK:
        case MOVE_TYPE::ATTACK:
            return "move";

        case MOVE_TYPE::SHORT_CASTLING:
        case MOVE_TYPE::LONG_CASTLING:
            return "castling";

        case MOVE_TYPE::EN_PASSANT:
            return "enpassant";

        case MOVE_TYPE::PROMOTION:
        case MOVE_TYPE::ATTACK_AND_PROMOTION:
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









