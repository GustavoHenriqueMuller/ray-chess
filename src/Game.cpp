#include "Game.h"
#include "Position.h"
#include "raylib.h"
#include "Renderer.h"

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
        Renderer::RenderAll(board, possibleMoves);
    }
}

void Game::HandleInput() {
    if (IsMouseButtonPressed(0)) {
        Vector2 mousePosition = GetMousePosition();

        Position clickedPosition = {int(mousePosition.y) / CELL_SIZE, int(mousePosition.x) / CELL_SIZE};
        Piece* clickedPiece = board.At(clickedPosition);

        // Select piece.
        if (clickedPiece != nullptr && clickedPiece->color == turn) {
            // If king, check for castling.
            /*if (selectedPiece && selectedPiece->type == Piece::TYPE::KING) {
                Move* move = GetMoveAtPosition(clickedPosition);

                if (move && (move->type == Move::TYPE::LONG_CASTLING || move->type == Move::TYPE::SHORT_CASTLING)) {
                    // Perform castling.
                    DoMove(*move);

                    selectedPiece = nullptr;
                    possibleMoves.clear();
                    return;
                }
            }*/

            selectedPiece = clickedPiece;
            possibleMoves = selectedPiece->GetPossibleMoves(board);
        } else {
            // Do movement.
            Move* desiredMove = GetMoveAtPosition(clickedPosition);

            if (selectedPiece != nullptr && desiredMove) {
                DoMove(*desiredMove);
            }
            
            selectedPiece = nullptr;
            possibleMoves.clear();
        }
    }
}

Move* Game::GetMoveAtPosition(const Position& position) {
    for (Move& move : possibleMoves) {
        if (move.position.i == position.i && move.position.j == position.j) {
            return &move;
        }
    }
    
    return nullptr;
}

// TODO: CHECAR POR REI, CHEQUE, ETC
// TODO: CHECAR SE NÃO VAI DEIXAR O REI EM CHEQUE

void Game::DoMove(const Move& move) {
    // Delete piece, if attack or en passant.
    if (move.type == Move::TYPE::ATTACK) {
        board.Destroy(move.position);
    } else if (move.type == Move::TYPE::EN_PASSANT) {
        board.Destroy({selectedPiece->GetPosition().i, move.position.j});
    }

    // Move piece. In case of castling, also move rook.
    if (move.type == Move::TYPE::SHORT_CASTLING) {
        DoShortCastling(move);
    } else if (move.type == Move::TYPE::LONG_CASTLING) {
        DoLongCastling(move);
    } else {
        // Swap positions.
        MovePiece(selectedPiece, move);
    }

    // Swap turns.
    this->turn = Piece::GetInverseColor(this->turn);
}

void Game::DoShortCastling(const Move& move) {
    Piece* rook = board.At({selectedPiece->GetPosition().i, 7});

    MovePiece(selectedPiece, move);
    MovePiece(rook, {Move::TYPE::WALK, rook->GetPosition().i, rook->GetPosition().j - 2});
}

void Game::DoLongCastling(const Move& move) {
    Piece* rook = board.At({selectedPiece->GetPosition().i, 0});

    MovePiece(selectedPiece, move);
    MovePiece(rook, {Move::TYPE::WALK, rook->GetPosition().i, rook->GetPosition().j + 3});
}

void Game::MovePiece(Piece* piece, const Move& move) {
    // Swap positions of piece.
    board.Set(move.position, piece);
    board.Set(piece->GetPosition(), nullptr);
    piece->DoMove(move);
}





