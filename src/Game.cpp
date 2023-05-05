#include "Game.h"
#include "Position.h"
#include "raylib.h"
#include "Renderer.h"
#include "pieces/Queen.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Rook.h"

#include <filesystem>
#include <iostream>

const std::string Game::ASSETS_PATH = "../assets";
const Color Game::LIGHT_SHADE = Color{240, 217, 181, 255};
const Color Game::DARK_SHADE = Color{181, 136, 99, 255};

Game::Game() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "RayChess");
    SetTargetFPS(60);

    LoadTextures();
    board.Init(textures);
}

void Game::LoadTextures() {
    for (const auto & entry : std::filesystem::directory_iterator(ASSETS_PATH)) {
        // Load and resize image.
        Image image = LoadImage(entry.path().string().c_str());
        ImageResize(&image, CELL_SIZE, CELL_SIZE);

        Texture texture = LoadTextureFromImage(image);

        // Add texture to map of textures.
        std::string fileName = entry.path().filename().string();
        int dotIndex = fileName.find('.');

        std::string fileNameWithoutExtension = entry.path().filename().string().substr(0, dotIndex);
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
        inPromotion ? HandlePromotionInput() : HandleInput();
        ChangeMouseCursor();

        // Render.
        BeginDrawing();
            Renderer::RenderBackground();
            Renderer::RenderPieces(board);

            if (!inPromotion) {
                Renderer::RenderMovesSelectedPiece(textures, possibleMoves);
            }

            Renderer::RenderGuideText();

            if (inPromotion) {
                Renderer::RenderPromotionScreen(textures, selectedPiece->color);
            }
        EndDrawing();
    }
}

void Game::SwapTurns() {
    this->turn = Piece::GetInverseColor(this->turn);
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
            Move* desiredMove = GetMoveAtPosition(clickedPosition);

            if (desiredMove && selectedPiece != nullptr) {
                DoMove(*desiredMove);
            }

            // Piece must still be select to render promotion screen.
            if (!desiredMove || desiredMove->type != Move::TYPE::PROMOTION) {
                selectedPiece = nullptr;
                possibleMoves.clear();
            }
        }
    }
}

void Game::HandlePromotionInput() {
    if (IsMouseButtonPressed(0)) {
        Vector2 mousePosition = GetMousePosition();

        Position clickedPosition = {int(mousePosition.y) / CELL_SIZE, int(mousePosition.x) / CELL_SIZE};

        if (clickedPosition.i == 3 && clickedPosition.j >= 2 && clickedPosition.j <= 5) {
            Piece* newPiece;

            if (clickedPosition.j == 2) { // Clicked queen.
                newPiece = new Queen(selectedPiece->GetPosition(), selectedPiece->color, textures["wq"]);
            } else if (clickedPosition.j == 3) { // Clicked rook.
                newPiece = new Rook(selectedPiece->GetPosition(), selectedPiece->color, textures["wr"]);
            } else if (clickedPosition.j == 4) { // Clicked bishop.
                newPiece = new Bishop(selectedPiece->GetPosition(), selectedPiece->color, textures["wb"]);
            } else if (clickedPosition.j == 5) { // Clicked knight.
                newPiece = new Knight(selectedPiece->GetPosition(), selectedPiece->color, textures["wn"]);
            }

            // Destroy peon, create new piece at same position.
            board.Destroy(selectedPiece->GetPosition());
            board.Set(newPiece->GetPosition(), newPiece);

            // Quit promotion, deselect peon and swap turns.
            inPromotion = false;
            selectedPiece = nullptr;
            possibleMoves.clear();

            SwapTurns();
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

void Game::ChangeMouseCursor() {
    Vector2 mousePosition = GetMousePosition();
    Position hoverPosition = {int(mousePosition.y) / CELL_SIZE, int(mousePosition.x) / CELL_SIZE};

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

    // If in promotion screen, also set mouse to pointer if hovering over the options.
    if (inPromotion && (hoverPosition.i == 3 && hoverPosition.j >= 2 && hoverPosition.j <= 5)) {
        SetMouseCursor(4);
    }
}

void Game::DoMove(const Move& move) {
    // Delete piece, if attack or en passant.
    if (move.type == Move::TYPE::ATTACK) {
        board.Destroy(move.position);
    } else if (move.type == Move::TYPE::EN_PASSANT) {
        board.Destroy({selectedPiece->GetPosition().i, move.position.j});
    }

    // In case of promotion, show promotion dialog and stop game.
    if (move.type == Move::TYPE::PROMOTION) {
        MovePiece(selectedPiece, move);
        inPromotion = true;
    } else {
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
        SwapTurns();
    }
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







