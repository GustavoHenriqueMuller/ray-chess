#include "Game.h"
#include "Position.h"
#include "raylib.h"

#include "pieces/Peon.h"

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
        board.Set({1, j}, new Peon({1, j}, Piece::PIECE_COLOR::PIECE_BLACK, textures.at("bp")));
    }

    //board[0][0] = new Piece {{0, 0}, Piece::PIECE_BLACK, textures.at("br")};
    //board[0][7] = new Piece {{0, 7}, Piece::PIECE_BLACK, textures.at("br")};

    //board[0][1] = new Piece {{0, 1}, Piece::PIECE_BLACK, textures.at("bn")};
    //board[0][6] = new Piece {{0, 6}, Piece::PIECE_BLACK, textures.at("bn")};

    //board[0][2] = new Piece {{0, 2}, Piece::PIECE_BLACK, textures.at("bb")};
    //board[0][5] = new Piece {{0, 3}, Piece::PIECE_BLACK, textures.at("bb")};

    //board[0][3] = new Piece {{0, 3}, Piece::PIECE_BLACK, textures.at("bq")};
    //board[0][4] = new Piece {{0, 3}, Piece::PIECE_BLACK, textures.at("bk")};

    // Init white pieces (player).
    for (int j = 0; j < 8; j++) {
        board.Set({6, j}, new Peon({6, j}, Piece::PIECE_COLOR::PIECE_WHITE, textures.at("wp")));
    }

    //board[7][0] = new Piece {{0, 0}, Piece::PIECE_WHITE, textures.at("wr")};
    //board[7][7] = new Piece {{0, 7}, Piece::PIECE_WHITE, textures.at("wr")};

    //board[7][1] = new Piece {{0, 1}, Piece::PIECE_WHITE, textures.at("wn")};
    //board[7][6] = new Piece {{0, 6}, Piece::PIECE_WHITE, textures.at("wn")};

    //board[7][2] = new Piece {{0, 2}, Piece::PIECE_WHITE, textures.at("wb")};
    //board[7][5] = new Piece {{0, 3}, Piece::PIECE_WHITE, textures.at("wb")};

    //board[7][3] = new Piece {{0, 3}, Piece::PIECE_WHITE, textures.at("wq")};
    //board[7][4] = new Piece {{0, 3}, Piece::PIECE_WHITE, textures.at("wk")};
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
        EndDrawing();
    }
}

void Game::HandleInput() {
    if (IsMouseButtonPressed(0)) {
        Vector2 mousePosition = GetMousePosition();

        Position clickedPosition = {int(mousePosition.y) / CELL_SIZE, int(mousePosition.x) / CELL_SIZE};
        Piece* clickedPiece = board.At(clickedPosition);

        // Select piece.
        if (clickedPiece != nullptr && clickedPiece->color == turnColor) {
            selectedPiece = clickedPiece;
            possibleMoves = selectedPiece->GetPossibleMoves(board);
        } else {
            // Do movement.
            if (selectedPiece != nullptr && IsValidMove(clickedPosition)) {
                DoMove(clickedPosition);
            }
            
            selectedPiece = nullptr;
            possibleMoves.clear();
        }
    }
}

bool Game::IsValidMove(const Position& move) {
    for (const Position& possibleMove : possibleMoves) {
        if (move.i == possibleMove.i && move.j == possibleMove.j) {
            return true;
        }
    }
    
    return false;
}

void Game::DoMove(const Position& move) {
    // Delete piece, if any.
    if (board.At(move) && board.At(move)->color != selectedPiece->color) {
        board.Destroy(move);
    }

    // Swap positions.
    board.Set(move, selectedPiece);
    board.Set(selectedPiece->position, nullptr);

    selectedPiece->position = move;

    // Swap turns.
    this->turnColor = this->turnColor == Piece::PIECE_WHITE ? Piece::PIECE_BLACK : Piece::PIECE_WHITE;
}

void Game::RenderBackground() {
    // TODO: Fazer com que seja estático e constante.
    Color lightColor = Color{235, 236, 208, 255};
    Color darkColor = Color{119, 149, 86, 255};

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int x = j * Game::CELL_SIZE;
            int y = i * Game::CELL_SIZE;

            int startingColorInRow = i % 2 == 0 ? 0 : 1;
            int colorIndex = (j + startingColorInRow) % 2;

            Color color = colorIndex == 0 ? lightColor : darkColor;

            DrawRectangle(x, y, Game::CELL_SIZE, Game::CELL_SIZE, color);
        }
    }
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




