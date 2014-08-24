#include "object.h"
#include "world.h"

using namespace std;

void Object::setup(double get_mass,Collision_Circ get_circle,Vector get_velocity,double get_angular_velocity,double get_health,string get_sprite,string get_sprite_moving){
    mass=get_mass;
    circle=get_circle;

    velocity=get_velocity;
    angle=velocity.direction;
    angular_velocity=get_angular_velocity;

    net_angular_force=0.0;
    angular_acceleration=0.0;

    health=get_health;

    sprite.set_name(get_sprite);
    sprite_moving.set_name(get_sprite_moving);
}

double Object::get_volume(){
    return (4.0/3.0)*M_PI*pow(circle.r,3.0);
}

double Object::get_density(){
    return mass/get_volume();
}

Vector Object::get_momentum(){
    return velocity*mass;
}

double Object::get_angular_momentum(){
    return angular_velocity*mass;
}

bool Object::is_alive(){
    return health>0.0;
}

void Object::accelerate(){
    if(is_alive()){
        acceleration=net_force/mass;
        angular_acceleration=net_angular_force/mass;

        velocity+=acceleration;
        angular_velocity+=angular_acceleration;

        net_force*=0.0;
        net_angular_force=0.0;
    }
}

void Object::movement(uint32_t index){
    if(is_alive()){
        Vector_Components vc=velocity.get_components();

        circle.x+=vc.a/UPDATE_RATE;
        circle.y+=vc.b/UPDATE_RATE;

        angle+=angular_velocity/UPDATE_RATE;
    }
}

void Object::gravitate(uint32_t index){
    /**Object* object=&game.world.objects[index];

    if(is_alive() && object->is_alive()){
        double distance=distance_between_points(circle.x,circle.y,object->circle.x,object->circle.y);

        double force_magnitude=(GRAVITY*get_relativistic_mass()*object->get_relativistic_mass())/pow(distance,2.0);

        net_force+=Vector(force_magnitude,get_angle_to_circ(circle,object->circle,game.camera));
        object->net_force+=Vector(force_magnitude,get_angle_to_circ(object->circle,circle,game.camera));
    }*/
}

void Object::animate(){
    if(is_alive()){
        if(collision_check_circ_rect(circle*game.camera_zoom,game.camera)){
            sprite_moving.animate();
        }
    }
}

void Object::render(Sprite* ptr_sprite){
    if(is_alive()){
        if(collision_check_circ_rect(circle*game.camera_zoom,game.camera)){
            ptr_sprite->render((circle.x-circle.r)*game.camera_zoom-game.camera.x,(circle.y-circle.r)*game.camera_zoom-game.camera.y,1.0,game.camera_zoom,game.camera_zoom,angle);

            ///render_circle(circle.x*game.camera_zoom-game.camera.x,circle.y*game.camera_zoom-game.camera.y,circle.r*game.camera_zoom,0.5,"red");
        }
    }
}
