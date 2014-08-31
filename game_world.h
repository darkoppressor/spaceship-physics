#ifndef game_world_h
#define game_world_h

#include "background.h"
#include "effect_transient.h"
#include "ship.h"
#include "collision.h"

#include <vector>

class Game_World{
public:

    std::vector<Background> backgrounds;

    std::vector<Effect_Transient> effects_transient;

    std::vector<Ship> ships;

    std::vector<Collision> collisions_ship_on_ship;

    void clear_world();
    void generate_world();

    void generate_ship(std::string type,double x,double y,Vector velocity,double angular_velocity,std::string faction);

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
