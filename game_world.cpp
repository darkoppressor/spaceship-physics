#include "game_world.h"
#include "world.h"
#include "render.h"

using namespace std;

void Game_World::clear_world(){
    for(int i=0;i<backgrounds.size();i++){
        backgrounds[i].unload();
    }
    backgrounds.clear();

    effects_transient.clear();

    ships.clear();

    collisions_ship_on_ship.clear();
}

void Game_World::generate_world(){
    RNG rng;

    clear_world();

    backgrounds.push_back(Background());

    Vector position_player((double)rng.random_range(0,1000000000)*0.000000001*144.0,(double)rng.random_range(0,359));
    Vector_Components vc_player=position_player.get_components_absolute();

    generate_ship("1",vc_player.a,vc_player.b,Vector(0.0,0.0),0.0,"player");

    /**for(int i=0;i<10;i++){
        Vector position((double)rng.random_range(0,1000000000)*0.000000001*144.0,(double)rng.random_range(0,359));
        Vector_Components vc=position.get_components_absolute();

        generate_ship("1",vc.a,vc.b,Vector(0.0,0.0),0.0,"hostile");
    }*/

    /**double universe_mass=SOLAR_MASS*1.5;
    double universe_density=(SOLAR_MASS*0.004)/pow(9460528400000000.0,3.0);
    double universe_volume=universe_mass/universe_density;
    double universe_radius=pow(universe_volume*(3.0/(4.0*M_PI)),1.0/3.0);*/

    /**double universe_mass=9000.0;
    double universe_radius=144.0;

    for(double mass_to_create=universe_mass;mass_to_create>0.0;){
        double mass=0.0;
        double radius=0.0;
        string sprite="";

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

        ships.push_back(Ship(mass,Collision_Circ(vc.a,vc.b,radius),0.0,Vector(0.0,0.0),0.0,sprite));

        mass_to_create-=mass;
    }*/
}

void Game_World::generate_ship(string type,double x,double y,Vector velocity,double angular_velocity,string faction){
    RNG rng;

    Sprite sprite_ship;
    sprite_ship.set_name(engine_interface.get_ship_type(type)->sprite);

    double radius=sprite_ship.get_width()/2.0;
    double density=(double)rng.random_range(900,1100)*0.001*SHIP_DENSITY;
    double volume=(4.0/3.0)*M_PI*pow(radius,3.0);
    double mass=volume*density;

    ships.push_back(Ship(type,mass,Collision_Circ(x,y,radius),velocity,angular_velocity,faction));
}

void Game_World::tick(){
}

void Game_World::ai(){
    for(uint32_t i=1;i<ships.size();i++){
        ships[i].ai();
    }
}

void Game_World::movement(){
    for(uint32_t i=0;i<effects_transient.size();i++){
        effects_transient[i].movement();
    }

    for(uint32_t i=0;i<ships.size();i++){
        ships[i].accelerate();
    }

    for(uint32_t i=0;i<ships.size();i++){
        ships[i].movement(i);
    }

    for(uint32_t i=0;i<collisions_ship_on_ship.size();i++){
        ships[collisions_ship_on_ship[i].object1].collide_with_ship(collisions_ship_on_ship[i].object2);
    }
    collisions_ship_on_ship.clear();
}

void Game_World::events(){
    for(int i=0;i<effects_transient.size();i++){
        if(!effects_transient[i].exists){
            effects_transient.erase(effects_transient.begin()+i);
            i--;
        }
    }
}

void Game_World::animate(){
    for(uint32_t i=0;i<effects_transient.size();i++){
        effects_transient[i].animate();
    }

    for(uint32_t i=0;i<ships.size();i++){
        ships[i].animate();
    }
}

void Game_World::render(){
    for(uint32_t i=0;i<effects_transient.size();i++){
        effects_transient[i].render();
    }

    for(uint32_t i=0;i<ships.size();i++){
        ships[i].render();
    }
}

void Game_World::update_background(){
    for(int i=0;i<backgrounds.size();i++){
        backgrounds[i].update(game.camera_delta_x,game.camera_delta_y);
    }
}

void Game_World::render_background(){
    render_rectangle(0,0,main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,1.0,"ui_black");

    for(int i=0;i<backgrounds.size();i++){
        if(backgrounds[i].opacity>0.0){
            backgrounds[i].render();
        }
    }
}

Ship* Game_World::get_player(){
    Ship* player=0;

    if(ships.size()>=1){
        player=&ships[0];
    }

    return player;
}
