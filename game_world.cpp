#include "game_world.h"
#include "world.h"

using namespace std;

void Game_World::clear_world(){
    objects.clear();

    collisions.clear();
    gravitations.clear();
}

void Game_World::generate_world(){
    RNG rng;

    clear_world();

    /**double universe_mass=SOLAR_MASS*1.5;
    double universe_density=(SOLAR_MASS*0.004)/pow(9460528400000000.0,3.0);
    double universe_volume=universe_mass/universe_density;
    double universe_radius=pow(universe_volume*(3.0/(4.0*M_PI)),1.0/3.0);*/

    double universe_mass=9000.0;
    double universe_radius=144.0;

    bool player_made=false;
    for(double mass_to_create=universe_mass;mass_to_create>0.0;){
        double mass=0.0;
        double radius=0.0;
        string sprite="";

        uint32_t object_type=rng.random_range(0,1);
        if(!player_made){
            player_made=true;
            object_type=1;
        }
        ///
        object_type=1;

        //Star
        if(object_type==0){
            mass=(double)rng.weighted_random_range(100,100000,100,"very_strong")*0.001*SOLAR_MASS;
            double density=(double)rng.weighted_random_range(9,11,10,"very_strong")*0.1*SOLAR_DENSITY;
            double volume=mass/density;
            radius=pow(volume*(3.0/(4.0*M_PI)),1.0/3.0);

            sprite="star_yellow";
        }

        //Ship
        else if(object_type==1){
            sprite="ship_"+string_stuff.num_to_string(rng.random_range(0,SHIPS-1));
            Sprite sprite_ship;
            sprite_ship.set_name(sprite);

            radius=sprite_ship.get_width()/2.0;
            double density=(double)rng.random_range(900,1100)*0.001*SHIP_DENSITY;
            double volume=(4.0/3.0)*M_PI*pow(radius,3.0);
            mass=volume*density;
        }

        Vector position((double)rng.random_range(0,1000000000)*0.000000001*universe_radius,(double)rng.random_range(0,359));
        Vector_Components vc=position.get_components_absolute();

        objects.push_back(Object(mass,Collision_Circ(vc.a,vc.b,radius),0.0,Vector(0.0,0.0),sprite));

        mass_to_create-=mass;
    }

    game.center_camera(Collision_Circ(0.0,0.0,0.0));
}

void Game_World::tick(){
}

void Game_World::ai(){
}

void Game_World::movement(){
    for(uint32_t i=0;i<objects.size();i++){
        objects[i].accelerate();
    }

    for(uint32_t i=0;i<objects.size();i++){
        objects[i].movement(i);
    }

    for(uint32_t i=0;i<gravitations.size();i++){
        objects[gravitations[i].object1].gravitate(gravitations[i].object2);
    }
    gravitations.clear();

    for(uint32_t i=0;i<collisions.size();i++){
        objects[collisions[i].object1].collide(collisions[i].object2);
    }
    collisions.clear();
}

void Game_World::events(){
}

void Game_World::animate(){
    for(uint32_t i=0;i<objects.size();i++){
        objects[i].animate();
    }
}

void Game_World::render(){
    for(uint32_t i=0;i<objects.size();i++){
        objects[i].render();
    }
}
