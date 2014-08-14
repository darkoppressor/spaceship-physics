#include "object.h"
#include "world.h"

using namespace std;

Object::Object(double get_mass,Collision_Circ get_circle,double get_angle,Vector get_velocity,string get_sprite){
    mass=get_mass;
    circle=get_circle;
    angle=get_angle;

    velocity=get_velocity;

    reset_thrust_input();

    sprite.set_name(get_sprite);
}

void Object::reset_thrust_input(){
    thrust_left=false;
    thrust_up=false;
    thrust_right=false;
    thrust_down=false;
}

void Object::apply_thrust(){
    if(thrust_left){
        net_force+=Vector(500.0,180.0);
    }
    if(thrust_up){
        net_force+=Vector(500.0,90.0);
    }
    if(thrust_right){
        net_force+=Vector(500.0,0.0);
    }
    if(thrust_down){
        net_force+=Vector(500.0,270.0);
    }
}

void Object::accelerate(){
    apply_thrust();

    Vector acceleration=net_force/mass;

    velocity+=acceleration;

    net_force*=0.0;
}

void Object::movement(uint32_t index){
    Vector_Components vc=velocity.get_components();

    circle.x+=vc.a/UPDATE_RATE;
    circle.y+=vc.b/UPDATE_RATE;

    for(uint32_t i=0;i<game.world.objects.size();i++){
        Object* object=&game.world.objects[i];

        if(this!=object){
            uint32_t object1=0;
            uint32_t object2=0;
            if(index<=i){
                object1=index;
                object2=i;
            }
            else{
                object1=i;
                object2=index;
            }

            /**if(true){
                Collision gravitation(object1,object2);

                bool interaction_is_old=false;
                for(uint32_t j=0;j<game.world.gravitations.size();j++){
                    if(gravitation==game.world.gravitations[j]){
                        interaction_is_old=true;
                        break;
                    }
                }

                if(!interaction_is_old){
                    game.world.gravitations.push_back(gravitation);
                }
            }*/

            if(collision_check_circ(circle,object->circle)){
                Collision collision(object1,object2);

                bool interaction_is_old=false;
                for(uint32_t j=0;j<game.world.collisions.size();j++){
                    if(collision==game.world.collisions[j]){
                        interaction_is_old=true;
                        break;
                    }
                }

                if(!interaction_is_old){
                    game.world.collisions.push_back(collision);
                }
            }
        }
    }
}

void Object::collide(uint32_t index){
    Object* object=&game.world.objects[index];

    if(collision_check_circ(circle,object->circle)){
        Vector_Components vc_collision=Vector(object->circle.r+circle.r,get_angle_to_circ(object->circle,circle,game.camera)).get_components();

        circle.x=object->circle.x+vc_collision.a;
        circle.y=object->circle.y+vc_collision.b;
    }

    Vector force_1(velocity.magnitude*mass,velocity.direction);
    Vector force_2(object->velocity.magnitude*object->mass,object->velocity.direction);

    net_force+=force_1.opposite()+force_2;
    object->net_force+=force_1+force_2.opposite();
}

void Object::gravitate(uint32_t index){
    Object* object=&game.world.objects[index];

    double distance=distance_between_points(circle.x,circle.y,object->circle.x,object->circle.y);

    double force_magnitude=(GRAVITY*mass*object->mass)/pow(distance,2.0);

    net_force+=Vector(force_magnitude,get_angle_to_circ(circle,object->circle,game.camera));
    object->net_force+=Vector(force_magnitude,get_angle_to_circ(object->circle,circle,game.camera));
}

void Object::animate(){
    if(collision_check_circ_rect(circle*game.camera_zoom,game.camera)){
        sprite.animate();
    }
}

void Object::render(){
    if(collision_check_circ_rect(circle*game.camera_zoom,game.camera)){
        sprite.render((circle.x-circle.r)*game.camera_zoom-game.camera.x,(circle.y-circle.r)*game.camera_zoom-game.camera.y,1.0,game.camera_zoom,game.camera_zoom,angle);
    }
}
