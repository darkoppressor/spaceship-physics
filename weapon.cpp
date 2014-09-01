#include "weapon.h"
#include "world.h"

using namespace std;

Weapon::Weapon(string get_type){
    type=get_type;

    ///message_log.add_error("loading: "+string_stuff.num_to_string(type.length())+", "+type);

    cooldown=0;
}

double Weapon::get_targeting_radius(){
    ///message_log.add_error("get_targeting_radius: "+string_stuff.num_to_string(type.length())+", "+type);
    return engine_interface.get_weapon_type(type)->targeting_radius;
    ///return 0.0;
}

void Weapon::cool(){
    if(cooldown>0){
        cooldown--;
    }
}

bool Weapon::cooled(){
    if(cooldown==0){
        return true;
    }
    else{
        return false;
    }
}

void Weapon::fire(string projectile,Collision_Circ parent_circle,double angle,Vector parent_velocity,double parent_angular_velocity){
    ///message_log.add_error("fire: "+string_stuff.num_to_string(type.length())+", "+type);

    Weapon_Type* weapon_type=engine_interface.get_weapon_type(type);

    cooldown=weapon_type->cooldown*UPDATE_RATE;

    Sprite sprite;
    sprite.set_name(engine_interface.get_projectile_type(projectile)->sprite);
    double sprite_radius=(sprite.get_width()+sprite.get_height())/2.0;

    Vector position(parent_circle.r+sprite_radius,angle);
    Vector_Components vc_position=position.get_components();
    double x=parent_circle.x+vc_position.a;
    double y=parent_circle.y+vc_position.b;

    game.world.spawn_projectile(projectile,x,y,parent_velocity,parent_angular_velocity,Vector(weapon_type->force,angle));

    sound_system.play_sound(weapon_type->sound,x,y);
}
