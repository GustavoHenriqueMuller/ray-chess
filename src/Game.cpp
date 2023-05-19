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
const std::string Game::TEXTURES_PATH = Game::ASSETS_PATH + "/textures";
const std::string Game::SOUNDS_PATH = Game::ASSETS_PATH + "/sounds";

const Color Game::LIGHT_SHADE = Color{240, 217, 181, 255};
const Color Game::DARK_SHADE = Color{181, 136, 99, 255};

Game::Game() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "RayChess");
    InitAudioDevice();

    SetTargetFPS(60);

    LoadTextures();
    LoadSounds();

    // Init the board and calculate the initial movements for the white player.
    board.Init();
    CalculateAllPossibleMovements();
}

void Game::LoadTextures() {
    for (const auto & entry : std::filesystem::directory_iterator(TEXTURES_PATH)) {
        // Load and resize image.
        Image image = LoadImage(entry.path().string().c_str());
        ImageResize(&image, CELL_SIZE, CELL_SIZE);

        Texture texture = LoadTextureFromImage(image);

        // Add texture to map of textures.
        std::string fileName = entry.path().filename().string();
        size_t dotIndex = fileName.find('.');

        std::string fileNameWithoutExtension = entry.path().filename().string().substr(0, dotIndex);
        textures[fileNameWithoutExtension] = texture;

        // Free image data.
        UnloadImage(image);
    }
}

void Game::LoadSounds() {
    for (const auto & entry : std::filesystem::directory_iterator(SOUNDS_PATH)) {
        // Load sound.
        Sound sound = LoadSound(entry.path().string().c_str());

        // Add sound to map of sounds.
        std::string fileName = entry.path().filename().string();
        size_t dotIndex = fileName.find('.');

        std::string fileNameWithoutExtension = entry.path().filename().string().substr(0, dotIndex);
        sounds[fileNameWithoutExtension] = sound;
    }
}

Game::~Game() {
    // Free textures.
    for (auto const& kv : textures) {
        UnloadTexture(kv.second);
    }

    // Free sounds.
    for (auto const& kv : sounds) {
        UnloadSound(kv.second);
    }

    board.Clear();

    CloseAudioDevice();
    CloseWindow();
}

void Game::Run() {
    while (!WindowShouldClose()){
        // Input.
        if (state == GAME_STATE::S_RUNNING) {
            HandleInput();

            // Getting new time.
            time += GetFrameTime();
        }

        if (state == GAME_STATE::S_PROMOTION) {
            HandlePromotionInput();
        }

        // Render.
        BeginDrawing();
        {
            std::vector<Move> movesOfSelectedPiece;

            if (selectedPiece) {
                movesOfSelectedPiece = possibleMovesPerPiece.at(selectedPiece);
            }

            Renderer::ChangeMouseCursor(board, movesOfSelectedPiece, turn, state == GAME_STATE::S_PROMOTION);
            Renderer::Clear();
            Renderer::RenderBackground();
            Renderer::RenderPieces(board, textures);

            if (state != GAME_STATE::S_PROMOTION) {
                Renderer::RenderMovesSelectedPiece(textures, movesOfSelectedPiece);
            }

            Renderer::RenderGuideText();
            Renderer::RenderInfoBar(round, time);

            // Render promotion screen.
            if (state == GAME_STATE::S_PROMOTION) {
                Renderer::RenderPromotionScreen(textures, selectedPiece->color);
            }

            // Render end-game screen.
            if (state == GAME_STATE::S_WHITE_WINS || state == GAME_STATE::S_BLACK_WINS) {
                Renderer::RenderEndScreen(state);
            }
        }
        EndDrawing();
    }
}

void Game::HandleInput() {
    if (IsMouseButtonPressed(0)) {
        Vector2 mousePosition = GetMousePosition();
        mousePosition.y -= Game::INFO_BAR_HEIGHT;

        Position clickedPosition = {int(mousePosition.y) / CELL_SIZE, int(mousePosition.x) / CELL_SIZE};
        Piece* clickedPiece = board.At(clickedPosition);

        // Select piece.
        if (clickedPiece != nullptr && clickedPiece->color == turn) {
            PlaySound(sounds["click"]);
            selectedPiece = clickedPiece;
        } else {
            // Do movement.
            Move* desiredMove = GetMoveAtPosition(clickedPosition);

            if (desiredMove && selectedPiece != nullptr) {
                DoMoveOnBoard(board, *desiredMove, true, true);
            } else {
                PlaySound(sounds["clickCancel"]);
            }

            // Piece must still be selected to render promotion screen.
            if (!desiredMove ||
               (desiredMove->type != MOVE_TYPE::PROMOTION &&
                desiredMove->type != MOVE_TYPE::ATTACK_AND_PROMOTION)
            ) {
                selectedPiece = nullptr;
            }
        }
    }
}

void Game::HandlePromotionInput() {
    if (IsMouseButtonPressed(0)) {
        Vector2 mousePosition = GetMousePosition();
        mousePosition.y -= Game::INFO_BAR_HEIGHT;

        Position clickedPosition = {int(mousePosition.y) / CELL_SIZE, int(mousePosition.x) / CELL_SIZE};

        if (clickedPosition.i == 3 && clickedPosition.j >= 2 && clickedPosition.j <= 5) {
            Piece* newPiece;

            if (clickedPosition.j == 2) { // Clicked queen.
                newPiece = new Queen(selectedPiece->GetPosition(), selectedPiece->color);
            } else if (clickedPosition.j == 3) { // Clicked rook.
                newPiece = new Rook(selectedPiece->GetPosition(), selectedPiece->color);
            } else if (clickedPosition.j == 4) { // Clicked bishop.
                newPiece = new Bishop(selectedPiece->GetPosition(), selectedPiece->color);
            } else { // Clicked knight.
                newPiece = new Knight(selectedPiece->GetPosition(), selectedPiece->color);
            }

            // Destroy peon, create new piece at same position.
            board.Destroy(selectedPiece->GetPosition());
            board.Add(newPiece);

            // Quit promotion, deselect peon and swap turns.
            state = GAME_STATE::S_RUNNING;

            selectedPiece = nullptr;
            possibleMovesPerPiece.clear();

            SwapTurns();
        }
    }
}

Move* Game::GetMoveAtPosition(const Position& position) {
    for (auto& [piece, moves] : possibleMovesPerPiece) {
        if (piece == selectedPiece) {
            for (Move& move : moves) {
                if (move.position.i == position.i && move.position.j == position.j) {
                    return &move;
                }
            }
        }
    }
    
    return nullptr;
}

void Game::DoMoveOnBoard(Board& targetBoard, const Move& move, bool doPromotion, bool swapTurns) {
    targetBoard.DoMove(selectedPiece, move);

    // If the move was a promotion move (and we should do promotion), show the promotion screen.
    if (doPromotion && (move.type == MOVE_TYPE::PROMOTION || move.type == MOVE_TYPE::ATTACK_AND_PROMOTION)) {
        state = GAME_STATE::S_PROMOTION;
    } else {
        // Swap turns.
        if (swapTurns) {
            SwapTurns();
        }
    }
}

void Game::SwapTurns() {
    turn = Piece::GetInverseColor(turn);

    // Advance round.
    if (turn == PIECE_COLOR::C_WHITE) {
        round++;
    }

    // Calculate all possible movements for the current pieces.
    CalculateAllPossibleMovements();

    // Check for stalemates or checkmates. If so, ends the game.
    CheckForEndOfGame();
}

void Game::CalculateAllPossibleMovements() {
    possibleMovesPerPiece.clear();

    for (Piece* piece : board.GetPiecesByColor(turn)) {
        possibleMovesPerPiece[piece] = piece->GetPossibleMoves(board);
    }

    // Remove the moves that could destroy the opponent's king.
    FilterMovesThatAttackOppositeKing();

    // Remove the moves that cause the player to get in check.
    FilterMovesThatLeadToCheck();
}

void Game::CheckForEndOfGame() {
    std::vector<Piece*> piecesOfCurrentTurn = board.GetPiecesByColor(turn);

    if (board.IsInCheck(turn)) {
        // Filter castling moves.
        FilterCastlingMoves();

        // If there are no moves possible and in check, declare checkmate.
        if (!IsAnyMovePossible()) {
            state = (turn == PIECE_COLOR::C_WHITE ? GAME_STATE::S_BLACK_WINS : GAME_STATE::S_WHITE_WINS);
        }
    } else if (!IsAnyMovePossible()) {
        // If not in check and there is not any move possible, declare stalemate.
        state = GAME_STATE::S_STALEMATE;
    }
}

void Game::FilterMovesThatAttackOppositeKing() {
    for (auto& [piece, possibleMoves] : possibleMovesPerPiece) {
        for (int i = possibleMoves.size() - 1; i >= 0; i--) {
            Move& move = possibleMoves[i];

            // Remove moves that attack the opponent's king.
            bool isAttackMove = move.type == MOVE_TYPE::ATTACK || move.type == MOVE_TYPE::ATTACK_AND_PROMOTION;

            if (isAttackMove) {
                Piece* attackedPiece = board.At(move.position);

                if (attackedPiece->type == PIECE_TYPE::KING && attackedPiece->color != turn) {
                    possibleMoves.erase(possibleMoves.begin() + i);
                }
            }
        }
    }
}

void Game::FilterMovesThatLeadToCheck() {
    for (auto& [piece, possibleMoves] : possibleMovesPerPiece) {
        for (int i = possibleMoves.size() - 1; i >= 0; i--) {
            // Copy current board and current selected piece.
            Board boardCopy = board;
            Piece* currentSelectedPiece = selectedPiece;

            // Perform the move.
            Move& move = possibleMoves[i];
            selectedPiece = boardCopy.At(piece->GetPosition());

            DoMoveOnBoard(boardCopy, move, false, false);

            if (boardCopy.IsInCheck(turn)) {
                possibleMoves.erase(possibleMoves.begin() + i);
            }

            // Restore old board.
            selectedPiece = currentSelectedPiece;
        }
    }
}

void Game::FilterCastlingMoves() {
    for (auto& [piece, possibleMoves] : possibleMovesPerPiece) {
        for (int i = possibleMoves.size() - 1; i >= 0; i--) {
            MOVE_TYPE type = possibleMoves[i].type;

            if (type == MOVE_TYPE::SHORT_CASTLING || type == MOVE_TYPE::LONG_CASTLING) {
                possibleMoves.erase(possibleMoves.begin() + i);
            }
        }
    }
}

bool Game::IsAnyMovePossible() {
    for (const auto& [pieceName, possibleMoves] : possibleMovesPerPiece) {
        if (!possibleMoves.empty()) {
            return true;
        }
    }

    return false;
}










