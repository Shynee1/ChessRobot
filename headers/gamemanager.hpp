#pragma once
#include "utility.hpp"
#include "component.hpp"
#include "openingbook.hpp"

class GameManager {
private:
    static std::shared_ptr<GameManager> instance;
    std::shared_ptr<chess::Board> board;
    std::shared_ptr<OpeningBook> openingBook;
    std::vector<std::shared_ptr<Component>> components = {};
    bool isGameRunning = false;
public:
    static std::shared_ptr<GameManager> Instance();
    GameManager() = default;
    void start_game(std::string fen);
    void end_game();
    void update_components();
    void draw_components();
    template <class T>
    void add_component() {
        static_assert(std::is_base_of<Component, T>(), "T must derive from Component");
        components.push_back(std::make_shared<T>());
        std::cout << "added component " << typeid(T).name() << "\n";
    }
    template <class T>
    std::shared_ptr<T> get_component() {
        static_assert(std::is_base_of<Component, T>(), "T must derive from Component");
        for (const auto& component : components) {
            auto casted_component = std::dynamic_pointer_cast<T>(component);
            if (casted_component != NULL)
                 return casted_component;
        
        }   

        fatal_log(std::format("ERROR: Cannot get component '{}'", typeid(T).name()));
        return nullptr;
    }
    std::shared_ptr<chess::Board> get_board();
    std::shared_ptr<OpeningBook> get_opening_book();
    bool is_game_running();
};