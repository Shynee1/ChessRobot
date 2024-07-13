#include "gamemanager.hpp"

std::shared_ptr<GameManager> GameManager::instance = NULL;

std::shared_ptr<GameManager> GameManager::Instance() {
    if (instance == NULL)
        instance = std::make_shared<GameManager>();
    return instance;
}
    
void GameManager::start_game(std::string fen){
    this->isGameRunning = true;
    this->board = std::make_shared<chess::Board>(fen);
    this->openingBook = std::make_shared<OpeningBook>(board);

    for (const auto& component : components)
        component->start();
}

void GameManager::end_game(){
    this->isGameRunning = false;
}

void GameManager::update_components() {
    if (!isGameRunning) return;

    for (const auto& component : components)
        component->update();
}

void GameManager::draw_components() {
    if (!isGameRunning) return;
    
    for (const auto& component : components)
        component->graphics();
}

std::shared_ptr<chess::Board> GameManager::get_board() {
    return board;
}

std::shared_ptr<OpeningBook> GameManager::get_opening_book() {
    return openingBook;
}

bool GameManager::is_game_running(){
    return isGameRunning;
}