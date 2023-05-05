#include "Game.h"
#include "Position.h"
#include "raylib.h"

#include <filesystem>
#include <iostream>

Game::Game() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "RayChess");
    SetTargetFPS(60);

    LoadTextures();
    board.Init(textures);
}

void Game::LoadTextures() {
    std::string assetsPath = "../assets";

    for (const auto & entry : std::filesystem::directory_iterator(assetsPath)) {
        // Load and resize image.
        Image image = LoadImage(entry.path().string().c_str());
        ImageResize(&image, CELL_SIZE, CELL_SIZE);

        Texture texture = LoadTextureFromImage(image);

        // Add texture to map of textures.
        std::string fileNameWithoutExtension = entry.path().filename().string().substr(0, 2);
        textures[fileNameWithoutExtension] = texture;

        // Free image data.
        UnloadImage(image);
    }
}

Game::~Game() {
    // Free textures.
    for (auto const& kv : textures) {
        UnloadTexture(kv.second);
    }

    board.Clear();
    CloseWindow();
}

void Game::Run() {
    while (!WindowShouldClose()){
        // Input.
        HandleInput();

        // Render.
        BeginDrawing();
            RenderBackground();
            RenderPieces();
            RenderMovesSelectedPiece();
            RenderGuideText();
        EndDrawing();
    }
}

void Game::HandleInput() {
    if (IsMouseButtonPressed(0)) {
        Vector2 mousePosition = GetMousePosition();

        Position clickedPosition = {int(mousePosition.y) / CELL_SIZE, int(mousePosition.x) / CELL_SIZE};
        Piece* clickedPiece = board.At(clickedPosition);

        // Select piece.
        if (clickedPiece != nullptr && clickedPiece->color == turn) {
            selectedPiece = clickedPiece;
            possibleMoves = selectedPiece->GetPossibleMoves(board);
        } else {
            // Do movement.
            if (selectedPiece != nullptr && IsPossibleMovePosition(clickedPosition)) {
                DoMove(clickedPosition);
            }
            
            selectedPiece = nullptr;
            possibleMoves.clear();
        }
    }
}

bool Game::IsPossibleMovePosition(const Position& move) {
    for (const Move& possibleMove : possibleMoves) {
        if (move.i == possibleMove.position.i && move.j == possibleMove.position.j) {
            return true;
        }
    }
    
    return false;
}

void Game::DoMove(const Position& move) {
    // Delete piece, if any.
    if (board.At(move) && board.At(move)->color != selectedPiece->color) {
        // TODO: CHECAR POR REI, CHEQUE, ETC
        board.Destroy(move);
    }

    // Swap positions.
    // TODO: CHECAR SE NÃO VAI DEIXAR O REI EM CHEQUE

    board.Set(move, selectedPiece);
    board.Set(selectedPiece->GetPosition(), nullptr);
    selectedPiece->DoMove(move);

    // Swap turns.
    this->turn = GetInverseColor(this->turn);
}

void Game::RenderBackground() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int x = j * Game::CELL_SIZE;
            int y = i * Game::CELL_SIZE;

            Color cellColor = GetShadeColor(GetColorOfCell({i, j}));
            DrawRectangle(x, y, Game::CELL_SIZE, Game::CELL_SIZE, cellColor);
        }
    }
}

Color Game::GetShadeColor(Piece::COLOR color) {
    return color == Piece::COLOR::C_WHITE ? LIGHT_SHADE : DARK_SHADE;
}

Piece::COLOR Game::GetColorOfCell(const Position& cellPosition) {
    int startingColorInRow = cellPosition.i % 2 == 0 ? 0 : 1;
    int colorIndex = (startingColorInRow + cellPosition.j) % 2;

    return colorIndex == 0 ? Piece::COLOR::C_WHITE : Piece::COLOR::C_BLACK;
}

Piece::COLOR Game::GetInverseColor(Piece::COLOR color) {
    return color == Piece::COLOR::C_WHITE ? Piece::COLOR::C_BLACK : Piece::COLOR::C_WHITE;
}

void Game::RenderPieces() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = board.At({i, j});

            if (piece != nullptr) {
                int x = j * CELL_SIZE;
                int y = i * CELL_SIZE;

                DrawTexture(piece->texture, x, y, WHITE);
            }
        }
    }
}

void Game::RenderMovesSelectedPiece() {
    for (const Move& move : possibleMoves) {
        int radius = 17;

        DrawCircle(
            move.position.j * CELL_SIZE + CELL_SIZE / 2,
            move.position.i * CELL_SIZE + CELL_SIZE / 2,
            radius,
            Color{0, 0, 0, 127}
        );
    }
}

void Game::RenderGuideText() {
    int padding = 3;
    int characterSize = 10;

    // Render 1-8 numbers (rows).
    for (int i = 0; i < 8; i++) {
        Color textColor = GetShadeColor(GetInverseColor(GetColorOfCell({i, 0})));

        // Render text.
        int x = padding;
        int y = i * CELL_SIZE + padding;

        char text[2];
        text[0] = 49 + i;
        text[1] = 0;

        DrawText(text, x, y, 20, textColor);
    }

    // Render h-a characters (columns).
    for (int j = 0; j < 8; j++) {
        Color textColor = GetShadeColor(GetInverseColor(GetColorOfCell({7, j})));

        // Render text.
        int x = (j + 1) * CELL_SIZE - characterSize - padding;
        int y = WINDOW_HEIGHT - characterSize * 1.75 - padding;

        char text[2];
        text[0] = 97 + (7 - j);
        text[1] = 0;

        DrawText(text, x, y, 20, textColor);
    }
}




