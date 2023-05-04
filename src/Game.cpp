#include "Game.h"
#include "Position.h"
#include "raylib.h"

#include "pieces/Peon.h"
#include "pieces/Rook.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"

#include <filesystem>
#include <iostream>

Game::Game() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "RayChess");
    SetTargetFPS(60);

    LoadTextures();
    InitBoard();
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

void Game::InitBoard() {
    // Init black pieces (computer).
    for (int j = 0; j < 8; j++) {
        board.Set({1, j}, new Peon({1, j}, Piece::TYPE::TYPE_BLACK, textures.at("bp")));
    }

    board.Set({0, 0}, new Rook({0, 0}, Piece::TYPE::TYPE_BLACK, textures.at("br")));
    board.Set({0, 7}, new Rook({0, 7}, Piece::TYPE::TYPE_BLACK, textures.at("br")));

    board.Set({0, 1}, new Knight({0, 1}, Piece::TYPE::TYPE_BLACK, textures.at("bn")));
    board.Set({0, 6}, new Knight({0, 6}, Piece::TYPE::TYPE_BLACK, textures.at("bn")));

    board.Set({0, 2}, new Bishop({0, 2}, Piece::TYPE::TYPE_BLACK, textures.at("bb")));
    board.Set({0, 5}, new Bishop({0, 2}, Piece::TYPE::TYPE_BLACK, textures.at("bb")));

    //board[0][3] = new Piece {{0, 3}, PIECE_COLOR::::PIECE_BLACK, textures.at("bq")};
    //board[0][4] = new Piece {{0, 3}, PIECE_COLOR::::PIECE_BLACK, textures.at("bk")};

    // Init white pieces (player).
    for (int j = 0; j < 8; j++) {
        board.Set({6, j}, new Peon({6, j}, Piece::TYPE::TYPE_WHITE, textures.at("wp")));
    }

    board.Set({7, 0}, new Rook({7, 0}, Piece::TYPE::TYPE_WHITE, textures.at("wr")));
    board.Set({7, 7}, new Rook({7, 7}, Piece::TYPE::TYPE_WHITE, textures.at("wr")));

    board.Set({7, 1}, new Knight({7, 1}, Piece::TYPE::TYPE_WHITE, textures.at("wn")));
    board.Set({7, 6}, new Knight({7, 6}, Piece::TYPE::TYPE_WHITE, textures.at("wn")));

    board.Set({7, 2}, new Bishop({7, 2}, Piece::TYPE::TYPE_WHITE, textures.at("wb")));
    board.Set({7, 5}, new Bishop({7, 5}, Piece::TYPE::TYPE_WHITE, textures.at("wb")));

    //board[7][3] = new Piece {{0, 3}, PIECE_COLOR::::PIECE_WHITE, textures.at("wq")};
    //board[7][4] = new Piece {{0, 3}, PIECE_COLOR::::PIECE_WHITE, textures.at("wk")};
}

Game::~Game() {
    // Free textures.
    for (auto const& kv : textures) {
        UnloadTexture(kv.second);
    }

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
        if (clickedPiece != nullptr && clickedPiece->type == turn) {
            selectedPiece = clickedPiece;
            possibleMoves = selectedPiece->GetPossibleMoves(board);
        } else {
            // Do movement.
            if (selectedPiece != nullptr && IsPossibleMove(clickedPosition)) {
                DoMove(clickedPosition);
            }
            
            selectedPiece = nullptr;
            possibleMoves.clear();
        }
    }
}

bool Game::IsPossibleMove(const Position& move) {
    for (const Position& possibleMove : possibleMoves) {
        if (move.i == possibleMove.i && move.j == possibleMove.j) {
            return true;
        }
    }
    
    return false;
}

void Game::DoMove(const Position& move) {
    // Delete piece, if any.
    if (board.At(move) && board.At(move)->type != selectedPiece->type) {
        // TODO: CHECAR POR REI, CHEQUE, ETC
        board.Destroy(move);
    }

    // Swap positions.
    board.Set(move, selectedPiece);
    board.Set(selectedPiece->GetPosition(), nullptr);

    selectedPiece->Move(move);

    // Swap turns.
    this->turn = GetInverseType(this->turn);
}

void Game::RenderBackground() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int x = j * Game::CELL_SIZE;
            int y = i * Game::CELL_SIZE;

            Color cellColor = GetTypeOfCell({i, j}) == Piece::TYPE::TYPE_WHITE ? LIGHT_COLOR : DARK_COLOR;
            DrawRectangle(x, y, Game::CELL_SIZE, Game::CELL_SIZE, cellColor);
        }
    }
}

Color Game::GetColorOfType(Piece::TYPE type) {
    return type == Piece::TYPE::TYPE_WHITE ? LIGHT_COLOR : DARK_COLOR;
}

Piece::TYPE Game::GetTypeOfCell(const Position& cellPosition) {
    int startingColorInRow = cellPosition.i % 2 == 0 ? 0 : 1;
    int colorIndex = (startingColorInRow + cellPosition.j) % 2;

    return colorIndex == 0 ? Piece::TYPE::TYPE_WHITE : Piece::TYPE::TYPE_BLACK;
}

Piece::TYPE Game::GetInverseType(Piece::TYPE type) {
    return type == Piece::TYPE::TYPE_WHITE ? Piece::TYPE::TYPE_BLACK : Piece::TYPE::TYPE_WHITE;
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
    for (const Position& move : possibleMoves) {
        int radius = 17;

        DrawCircle(
            move.j * CELL_SIZE + CELL_SIZE / 2,
            move.i * CELL_SIZE + CELL_SIZE / 2,
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
        Color textColor = GetColorOfType(GetInverseType(GetTypeOfCell({i, 0})));

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
        Color textColor = GetColorOfType(GetInverseType(GetTypeOfCell({7, j})));

        // Render text.
        int x = (j + 1) * CELL_SIZE - characterSize - padding;
        int y = WINDOW_HEIGHT - characterSize * 1.75 - padding;

        char text[2];
        text[0] = 97 + (7 - j);
        text[1] = 0;

        DrawText(text, x, y, 20, textColor);
    }
}




