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
    board.Init();
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
    while (!WindowShouldClose() && !isGameOver){
        // Input.
        inPromotion ? HandlePromotionInput() : HandleInput();

        // Getting new time.
        time += GetFrameTime();

        // Render.
        BeginDrawing();
            Renderer::ChangeMouseCursor(board, possibleMoves, turn, inPromotion);

            Renderer::Clear();
            Renderer::RenderBackground();
            Renderer::RenderPieces(board, textures);

            if (!inPromotion) {
                Renderer::RenderMovesSelectedPiece(textures, possibleMoves);
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
    turn = Piece::GetInverseColor(this->turn);

    // Advance round.
    if (turn == PIECE_COLOR::C_WHITE) {
        round++;
    }

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
            possibleMoves = selectedPiece->GetPossibleMoves(board);

            if (inCheck) {
                FilterMovesThatDoNotRemoveCheck(possibleMoves);
            }
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
                possibleMoves.clear();
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
            } else if (clickedPosition.j == 5) { // Clicked knight.
                newPiece = new Knight(selectedPiece->GetPosition(), selectedPiece->color);
            }

            // Destroy peon, create new piece at same position.
            board.Destroy(selectedPiece->GetPosition());
            board.Add(newPiece);

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

void Game::DoMove(const Move& move) {
    // Delete piece, if attack or en passant.
    if (move.type == MOVE_TYPE::ATTACK || move.type == MOVE_TYPE::ATTACK_AND_PROMOTION) {
        board.Destroy(move.position);
    } else if (move.type == MOVE_TYPE::EN_PASSANT) {
        board.Destroy({selectedPiece->GetPosition().i, move.position.j});
    }

    // In case of promotion, show promotion dialog and stop game.
    if (move.type == MOVE_TYPE::PROMOTION || move.type == MOVE_TYPE::ATTACK_AND_PROMOTION) {
        selectedPiece->DoMove(move);
        inPromotion = true;
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
        SwapTurns();
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

void Game::CheckForEndOfGame() {
    std::vector<Piece*> piecesOfCurrentTurn = board.GetPiecesByColor(turn);
    inCheck = false;

    if (CheckForCheck(turn)) {
        std::cout << "To em cheque!" << std::endl;
        inCheck = true;

        // Check for checkmate.
        //if (CheckForCheckmate(piecesOfCurrentTurn)) {
           // FIM DE JOGO.
        //}
    } else {
        if (!IsAnyMovePossible(piecesOfCurrentTurn)) {
            // FIM DE JOGO (afogamento).
            isGameOver = true; // TODO: MELHORAR ISSO, CRIAR TELA DE ENDSCREEN.
        }
    }
}

bool Game::CheckForCheck(PIECE_COLOR player) {
    std::vector<Piece*> enemyPieces = board.GetPiecesByColor(Piece::GetInverseColor(player));

    for (Piece* piece : enemyPieces) {
        for (const Move& move : piece->GetPossibleMoves(board)) {
            Piece* pieceAtMovePosition = board.At(move.position);

            bool movePositionContainsMyKing = pieceAtMovePosition &&
                                              pieceAtMovePosition->color == turn &&
                                              pieceAtMovePosition->type == PIECE_TYPE::KING;

            bool moveIsAttack = move.type == MOVE_TYPE::ATTACK || move.type == MOVE_TYPE::ATTACK_AND_PROMOTION;

            // If the enemy piece is attacking the players king, the king is in check.
            if (movePositionContainsMyKing && moveIsAttack) {
                return true;
            }
        }
    }

    return false;
}

/*bool Game::CheckCheckmate(PIECE_COLOR color, const std::vector<Piece*>& pieces) {
    for (Piece* piece : pieces) {
        // If there are moves that remove the check, the king is not in checkmate.
        if (!FilterMovesThatDoNotRemoveCheck(piece->GetPossibleMoves(board)).empty()) {
            return false;
        }
    }

    return true;
}*/

void Game::FilterMovesThatDoNotRemoveCheck(std::vector<Move>& moves) {
    for (size_t i = moves.size(); i >= 0; i--) {
        Board boardCopy = board; // N dá pra fazer isso, tá afetando os pointer
        DoMove(boardCopy, moves[i]);

        if (CheckForCheck(boardCopy, turn)) {
            moves.erase(moves.begin() + i);
        }
    }
}

bool Game::IsAnyMovePossible(const std::vector<Piece*>& pieces) {
    for (Piece* piece : pieces) {
        if (!piece->GetPossibleMoves(board).empty()) { // TODO: FAZER CACHE AQUI!
            return true;
        }
    }

    return false;
}








