#include "Input_controller.h"

Input_controller::Input_controller() {
}

void Input_controller::init() {
    this->transform = &parent_entity->get_component<Transform_component>();
}

void Input_controller::update() {

    if (Game_controler::event.type == SDL_KEYDOWN) {
        switch (Game_controler::event.key.keysym.sym) {
            case SDLK_w:
                transform->velocity.y = -1;
                break;
            case SDLK_s:
                transform->velocity.y = 1;
                break;
            case SDLK_a:
                transform->velocity.x = -1;
                break;
            case SDLK_d:
                transform->velocity.x = 1;
                break;
            default:
                break;
        }
    }

    if (Game_controler::event.type == SDL_KEYUP) {
        switch (Game_controler::event.key.keysym.sym) {
            case SDLK_w:
                transform->velocity.y = 0;
                break;
            case SDLK_s:
                transform->velocity.y = 0;
                break;
            case SDLK_a:
                transform->velocity.x = 0;
                break;
            case SDLK_d:
                transform->velocity.x = 0;
                break;
            default:
                break;
        }
    }
}