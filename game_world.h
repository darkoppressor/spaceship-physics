#ifndef game_world_h
#define game_world_h

#include "object.h"
#include "collision.h"

#include <vector>

class Game_World{
public:

    std::vector<Object> objects;

    std::vector<Collision> collisions;
    std::vector<Collision> gravitations;

    void clear_world();
    void generate_world();

    void tick();
    void ai();
    void movement();
    void events();
    void animate();
    void render();
};

#endif
