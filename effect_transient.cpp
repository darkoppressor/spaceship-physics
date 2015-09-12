/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "effect_transient.h"
#include "world.h"

using namespace std;

Effect_Transient::Effect_Transient(double x,double y,Vector get_velocity,double get_angular_velocity,double get_scale,double get_angle,string sprite_name){
    sprite.set_name(sprite_name);

    circle.x=x;
    circle.y=y;
    circle.r=(sprite.get_width()+sprite.get_height())/2.0;

    velocity=get_velocity;
    angular_velocity=get_angular_velocity;

    scale=get_scale;
    angle=get_angle;

    exists=true;
}

void Effect_Transient::movement(){
    if(exists){
        Vector_Components vc=velocity.get_components();

        circle.x+=vc.a/UPDATE_RATE;
        circle.y+=vc.b/UPDATE_RATE;

        angle+=angular_velocity/UPDATE_RATE;
    }
}

void Effect_Transient::animate(){
    if(exists){
        sprite.animate();

        if(!sprite.animating){
            exists=false;
        }
    }
}

void Effect_Transient::render(){
    if(exists && collision_check_circ_rect(circle*game.camera_zoom,game.camera)){
        sprite.render((circle.x-(sprite.get_width()*scale)/2.0)*game.camera_zoom-game.camera.x,(circle.y-(sprite.get_height()*scale)/2.0)*game.camera_zoom-game.camera.y,1.0,scale*game.camera_zoom,scale*game.camera_zoom,angle);
    }
}
