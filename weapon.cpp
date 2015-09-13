/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "weapon.h"
#include "world.h"

using namespace std;

Weapon::Weapon(string get_type){
    type=get_type;

    cooldown=0;
}

double Weapon::get_targeting_radius(){
    return engine_interface.get_weapon_type(type)->targeting_radius;
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
    Weapon_Type* weapon_type=engine_interface.get_weapon_type(type);

    cooldown=(weapon_type->cooldown/1000.0)*UPDATE_RATE;

    Sprite sprite;
    sprite.set_name(engine_interface.get_projectile_type(projectile)->sprite);
    double sprite_radius=(sprite.get_width()+sprite.get_height())/2.0;

    Vector position(parent_circle.r+sprite_radius,angle);
    Vector_Components vc_position=position.get_components();
    double x=parent_circle.x+vc_position.a;
    double y=parent_circle.y+vc_position.b;

    game.world.spawn_projectile(projectile,x,y,parent_velocity,parent_angular_velocity,Vector(weapon_type->force/UPDATE_RATE,angle));

    sound_system.play_sound(weapon_type->sound,x,y);
}
