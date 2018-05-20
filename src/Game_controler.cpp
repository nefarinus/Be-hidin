#include "Game_controler.h"
#include "Camera.h"
#include "Collision.h"
#include "EC/Components.h"
#include "EC/EC.h"
#include "Texture_manager.h"
#include "Tile_map.h"
#include "Vector_2D.h"

const char* map_tileset = "assets/map/tileset.png";

std::unique_ptr<Tile_map> map;

std::unique_ptr<SDL_Renderer, SDL_renderer_destroyer> Game_controler::renderer =
    nullptr;

Entity_manager manager;
auto&          new_player(manager.add_entity());

Camera* camera;

SDL_Event Game_controler::event;

std::vector<Collider_component*> Game_controler::colliders;

enum grup_tags : std::size_t {

    g_map,
    g_players,
    g_enemies,
    g_colliders
};

auto& map_tiles(manager.get_group(g_map));
auto& players(manager.get_group(g_players));
auto& enemies(manager.get_group(g_enemies));

Game_controler::Game_controler() {
}

Game_controler::~Game_controler() {
}

void Game_controler::initialize(const char* title,
                                int         x_pos,
                                int         y_pos,
                                int         width,
                                int         height,
                                bool        fullscreen) {
    loop_cnt  = 0;
    int flags = 0;

    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "SDL initialized" << std::endl;

        window.reset(
            SDL_CreateWindow(title, x_pos, y_pos, width, height, flags));
        if (window.get()) {
            std::cout << "window initialized" << std::endl;
        }

        renderer.reset(SDL_CreateRenderer(window.get(), -1, 0));
        if (renderer.get()) {
            std::cout << "renderer initialized" << std::endl;
            SDL_SetRenderDrawColor(renderer.get(), 0, 128, 255, 255);
        }

        running = true;
    } else {
        running = false;
    }

    map.reset(new Tile_map());

    Tile_map::load_map("assets/map/level.map", 50, 40, 0, -20);

    new_player.add_component<Transform_component>(0, 512, 500, 500, 0.1);
    new_player.add_component<Sprite_component>("assets/player_anims/player.png",
                                               true);
    new_player.add_component<Input_controller>();
    new_player.add_component<Collider_component>("player");
    new_player.add_group(g_players);

    camera =
        new Camera(&new_player.get_component<Transform_component>(), map_tiles);
}

void Game_controler::handle_events() {

    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;
        default:
            break;
    }
}

void Game_controler::update() {

    manager.remove_inactive();
    manager.update();

    camera->update();

    for (auto col : colliders) {
        Collision::aabb(new_player.get_component<Collider_component>(), *col);
    }

    // call update methods for all objects
}

void Game_controler::render() {

    SDL_RenderClear(renderer.get());

    for (auto& t : map_tiles) {
        t->draw();
    }
    for (auto& p : players) {
        p->draw();
    }
    for (auto& e : enemies) {
        e->draw();
    }

    // manager.draw();

    SDL_RenderPresent(renderer.get());
}

void Game_controler::clear() {

    SDL_Quit();

    std::cout << "Game cleaned" << std::endl;
}

bool Game_controler::is_running() {
    return running;
}

void Game_controler::addMapTile(int src_x, int src_y, int x, int y) {
    auto& tile(manager.add_entity());
    tile.add_component<Tile_component>(src_x, src_y, x, y, map_tileset);
    tile.add_group(g_map);
}
