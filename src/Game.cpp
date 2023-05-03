#include "Game.h"
#include "raylib.h"

#include <filesystem>

Game::Game() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "RayChess");
    SetTargetFPS(60);

    this->LoadTextures();
    this->InitBoard();
}

Game::~Game() {
    // Free textures.
    for (auto const& kv : this->textures) {
        UnloadTexture(kv.second);
    }

    // Free board.
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] != nullptr) {
                delete board[i][j];
            }
        }
    }

    CloseWindow();
}

void Game::Run() {
    while (!WindowShouldClose()){
        BeginDrawing();
            this->RenderBackground();
            this->RenderPieces();
        EndDrawing();
    }
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
        this->textures[fileNameWithoutExtension] = texture;

        // Free image data.
        UnloadImage(image);
    }
}

void Game::InitBoard() {
    // Clear board.
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = nullptr;
        }
    }

    // Init black pieces (computer).
    for (int j = 0; j < 8; j++) {
        this->board[1][j] = new Piece {
            1,
            j,
            Piece::Color::PIECE_BLACK,
            Piece::Type::PEON,
            this->textures.at("bp")
        };
    }

    this->board[0][0] = new Piece {0, 0, Piece::PIECE_BLACK, Piece::ROOK, this->textures.at("br")};
    this->board[0][7] = new Piece {0, 7, Piece::PIECE_BLACK, Piece::ROOK, this->textures.at("br")};

    this->board[0][1] = new Piece {0, 1, Piece::PIECE_BLACK, Piece::KNIGHT, this->textures.at("bn")};
    this->board[0][6] = new Piece {0, 6, Piece::PIECE_BLACK, Piece::KNIGHT, this->textures.at("bn")};

    this->board[0][2] = new Piece {0, 2, Piece::PIECE_BLACK, Piece::BISHOP, this->textures.at("bb")};
    this->board[0][5] = new Piece {0, 3, Piece::PIECE_BLACK, Piece::BISHOP, this->textures.at("bb")};

    this->board[0][3] = new Piece {0, 3, Piece::PIECE_BLACK, Piece::QUEEN, this->textures.at("bq")};
    this->board[0][4] = new Piece {0, 3, Piece::PIECE_BLACK, Piece::KING, this->textures.at("bk")};

    // Init white pieces (player).
    for (int j = 0; j < 8; j++) {
        this->board[6][j] = new Piece {
                1,
                j,
                Piece::Color::PIECE_BLACK,
                Piece::Type::PEON,
                this->textures.at("wp")
        };
    }

    this->board[7][0] = new Piece {0, 0, Piece::PIECE_WHITE, Piece::ROOK, this->textures.at("wr")};
    this->board[7][7] = new Piece {0, 7, Piece::PIECE_WHITE, Piece::ROOK, this->textures.at("wr")};

    this->board[7][1] = new Piece {0, 1, Piece::PIECE_WHITE, Piece::KNIGHT, this->textures.at("wn")};
    this->board[7][6] = new Piece {0, 6, Piece::PIECE_WHITE, Piece::KNIGHT, this->textures.at("wn")};

    this->board[7][2] = new Piece {0, 2, Piece::PIECE_WHITE, Piece::BISHOP, this->textures.at("wb")};
    this->board[7][5] = new Piece {0, 3, Piece::PIECE_WHITE, Piece::BISHOP, this->textures.at("wb")};

    this->board[7][3] = new Piece {0, 3, Piece::PIECE_WHITE, Piece::QUEEN, this->textures.at("wq")};
    this->board[7][4] = new Piece {0, 3, Piece::PIECE_WHITE, Piece::KING, this->textures.at("wk")};
}

void Game::RenderBackground() {
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
            Piece* piece = this->board[i][j];

            if (piece != nullptr) {
                int x = j * CELL_SIZE;
                int y = i * CELL_SIZE;

                DrawTexture(piece->texture, x, y, WHITE);
            }
        }
    }
}




