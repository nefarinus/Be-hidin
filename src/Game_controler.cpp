#include "Game_controler.h"
#include "Collision.h"
#include "EC/Components.h"
#include "EC/EC.h"
#include "Texture_manager.h"
#include "Tile_map.h"
#include "Vector_2D.h"

std::unique_ptr<Tile_map> map;

std::unique_ptr<SDL_Renderer, SDL_renderer_destroyer> Game_controler::renderer =
    nullptr;

Entity_manager manager;
auto&          new_player(manager.add_entity());
auto&          tile_0(manager.add_entity());
auto&          tile_1(manager.add_entity());
auto&          tile_2(manager.add_entity());

SDL_Event Game_controler::event;

std::vector<Collider_component*> Game_controler::colliders;

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

    new_player.add_component<Transform_component>(320, 0, 32, 32, 2);
    new_player.add_component<Sprite_component>("assets/player0.png");
    new_player.add_component<Input_controller>();
    new_player.add_component<Collider_component>("player");
    // new_player.get_component<Transform_component>().velocity.y = 1;

    tile_0.add_component<Tile_component>(20, 20, 32, 32, 1);
    tile_0.add_component<Collider_component>("wall");

    tile_1.add_component<Tile_component>(250, 250, 32, 32, 2);
    tile_1.add_component<Collider_component>("platform");

    tile_2.add_component<Tile_component>(300, 300, 32, 32, 3);
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

    // call update methods for all objects
}

void Game_controler::render() {

    SDL_RenderClear(renderer.get());
    // stuff to render
    // map->draw_map();
    manager.draw();

    SDL_RenderPresent(renderer.get());
}

void Game_controler::clear() {

    SDL_Quit();

    std::cout << "Game cleaned" << std::endl;
}

bool Game_controler::is_running() {
    return running;
}
