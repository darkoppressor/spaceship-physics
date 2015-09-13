/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_world_h
#define game_world_h

#include "background.h"
#include "effect_transient.h"
#include "ship.h"
#include "collision.h"
#include "projectile.h"

#include <vector>

class Game_World{
public:

    std::vector<Background> backgrounds;

    std::vector<Effect_Transient> effects_transient;

    std::vector<Ship> ships;
    std::vector<Projectile> projectiles;

    std::vector<Ship> new_ships;
    std::vector<Projectile> new_projectiles;

    std::vector<Collision> collisions_ship_on_ship;
    std::vector<Collision> collisions_projectile_on_ship;

    void clear_world();
    void generate_world();

    void spawn_ship(std::string type,double x,double y,Vector velocity,double angular_velocity,std::string faction,std::vector<std::string> weapons);
    void spawn_projectile(std::string type,double x,double y,Vector velocity,double angular_velocity,Vector force);

    void tick();
    void ai();
    void movement();
    void events();
    void animate();
    void render();

    void update_background();
    void render_background();

    Ship* get_player();
};

#endif
