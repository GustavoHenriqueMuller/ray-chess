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

    // Init the board with the pieces and calculate the initial movements for the white player.
    board.Init();
    CalculateAllPossibleMovements();
}

void Game::LoadTextures() {
    for (const auto & entry : std::filesystem::directory_iterator(ASSETS_PATH)) {
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

Game::~Game() {
    // Free textures.
    for (auto const& kv : textures) {
        UnloadTexture(kv.second);
    }

    board.Clear();
    CloseWindow();
}

void Game::Run() {
    while (!WindowShouldClose() && state == GAME_STATE::RUNNING){
        // Input.
        inPromotion ? HandlePromotionInput() : HandleInput();

        // Getting new time.
        time += GetFrameTime();

        // Render.
        BeginDrawing();
            std::vector<Move> movesOfSelectedPiece;

            if (selectedPiece) {
                movesOfSelectedPiece = possibleMovesPerPiece.at(selectedPiece);
            }

            Renderer::ChangeMouseCursor(board, movesOfSelectedPiece, turn, inPromotion);
            Renderer::Clear();
            Renderer::RenderBackground();
            Renderer::RenderPieces(board, textures);

            if (!inPromotion) {
                Renderer::RenderMovesSelectedPiece(textures, movesOfSelectedPiece);
            }

            Renderer::RenderGuideText();

            if (inPromotion) {
                Renderer::RenderPromotionScreen(textures, selectedPiece->color);
            }

            Renderer::RenderInfoBar(round, time);
        EndDrawing();
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

void Game::HandleInput() {
    if (IsMouseButtonPressed(0)) {
        Vector2 mousePosition = GetMousePosition();
        mousePosition.y -= Game::INFO_BAR_HEIGHT;

        Position clickedPosition = {int(mousePosition.y) / CELL_SIZE, int(mousePosition.x) / CELL_SIZE};
        Piece* clickedPiece = board.At(clickedPosition);

        // Select piece.
        if (clickedPiece != nullptr && clickedPiece->color == turn) {
            selectedPiece = clickedPiece;
        } else {
            // Do movement.
            Move* desiredMove = GetMoveAtPosition(clickedPosition);

            if (desiredMove && selectedPiece != nullptr) {
                DoMove(*desiredMove);
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
            inPromotion = false;

            selectedPiece = nullptr;
            possibleMovesPerPiece.clear();

            SwapTurns();
        }
    }
}

Move* Game::GetMoveAtPosition(const Position& position) {
    for (auto& [name, moves] : possibleMovesPerPiece) {
        for (Move& move : moves) {
            if (move.position.i == position.i && move.position.j == position.j) {
                return &move;
            }
        }
    }
    
    return nullptr;
}

void Game::DoMove(const Move& move, bool doPromotion, bool swapTurns) {
    // Delete piece, if attack or en passant.
    if (move.type == MOVE_TYPE::ATTACK || move.type == MOVE_TYPE::ATTACK_AND_PROMOTION) {
        board.Destroy(move.position);
    } else if (move.type == MOVE_TYPE::EN_PASSANT) {
        board.Destroy({selectedPiece->GetPosition().i, move.position.j});
    }

    // In case of promotion, show promotion dialog and stop game.
    if (move.type == MOVE_TYPE::PROMOTION || move.type == MOVE_TYPE::ATTACK_AND_PROMOTION) {
        selectedPiece->DoMove(move);

        if (doPromotion) {
            inPromotion = true;
        }
    } else {
        // Move piece. In case of castling, also move rook.
        if (move.type == MOVE_TYPE::SHORT_CASTLING) {
            DoShortCastling(move);
        } else if (move.type == MOVE_TYPE::LONG_CASTLING) {
            DoLongCastling(move);
        } else {
            // Swap positions.
            selectedPiece->DoMove(move);
        }

        // Swap turns.
        if (swapTurns) {
            SwapTurns();
        }
    }
}

void Game::DoShortCastling(const Move& move) {
    Piece* rook = board.At({selectedPiece->GetPosition().i, 7});

    selectedPiece->DoMove(move);
    rook->DoMove({MOVE_TYPE::WALK, rook->GetPosition().i, rook->GetPosition().j - 2});
}

void Game::DoLongCastling(const Move& move) {
    Piece* rook = board.At({selectedPiece->GetPosition().i, 0});

    selectedPiece->DoMove(move);
    rook->DoMove({MOVE_TYPE::WALK, rook->GetPosition().i, rook->GetPosition().j + 3});
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

    if (CheckForCheck()) {
        // If there are no moves possible and in check, declare checkmate.
        if (!IsAnyMovePossible()) {
            state = (turn == PIECE_COLOR::C_WHITE ? GAME_STATE::BLACK_WINS : GAME_STATE::WHITE_WINS);
        }
    } else if (!IsAnyMovePossible()) {
        // If not in check and there is not any move possible, declare stalemate.
        state = GAME_STATE::STALEMATE;
    }
}

bool Game::CheckForCheck() {
    std::vector<Piece*> enemyPieces = board.GetPiecesByColor(Piece::GetInverseColor(turn));

    for (Piece* piece : enemyPieces) {
        for (const Move& move : piece->GetPossibleMoves(board)) {
            Piece* pieceAtMovePosition = board.At(move.position);

            bool movePositionContainsMyKing = pieceAtMovePosition &&
                                              pieceAtMovePosition->color == turn &&
                                              pieceAtMovePosition->type == PIECE_TYPE::KING;

            bool moveIsAttack = move.type == MOVE_TYPE::ATTACK || move.type == MOVE_TYPE::ATTACK_AND_PROMOTION;

            // If the enemy piece is attacking my king, the king is in check.
            if (movePositionContainsMyKing && moveIsAttack) {
                return true;
            }
        }
    }

    return false;
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
            selectedPiece = piece;

            DoMove(move, false, false);

            // Restore old board.
            selectedPiece = currentSelectedPiece;
            board = boardCopy;

            // TODO: ARRUMAR ISSO
            /*

            Board boardCopy = board;
            Piece* selectedPieceCopy = selectedPiece;

            selectedPiece = piece;

            Move& move = possibleMoves[i];

            DoMove(move, false, false);

            // If the move does not remove the check after being done, remove it from the
            // vector of possible moves.
            // DoMove(move);

            if (CheckForCheck()) {
                possibleMoves.erase(possibleMoves.begin() + i);
            }

            board = boardCopy;
            selectedPiece = selectedPieceCopy;*/
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










