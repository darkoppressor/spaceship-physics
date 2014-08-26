#include "ship.h"
#include "world.h"

using namespace std;

Ship::Ship(string get_type,double get_mass,Collision_Circ get_circle,Vector get_velocity,double get_angular_velocity,string get_faction){
    type=get_type;

    Object::setup(get_mass,get_circle,get_velocity,get_angular_velocity,get_health_max(),engine_interface.get_ship_type(type)->sprite);

    reset_thrust_input();

    armor=get_armor_max();

    faction=get_faction;

    sprite_thrust.set_name("thrust");
}

double Ship::get_health_max(){
    return engine_interface.get_ship_type(type)->health;
}

double Ship::get_armor_max(){
    return engine_interface.get_ship_type(type)->armor;
}

double Ship::get_thrust(){
    return engine_interface.get_ship_type(type)->thrust;
}

double Ship::get_angular_thrust(){
    return engine_interface.get_ship_type(type)->angular_thrust;
}

double Ship::get_brake(){
    return engine_interface.get_ship_type(type)->brake;
}

double Ship::get_stabilizer(){
    return engine_interface.get_ship_type(type)->stabilizer;
}

double Ship::get_angular_stabilizer(){
    return engine_interface.get_ship_type(type)->angular_stabilizer;
}

void Ship::reset_thrust_input(){
    thrust_left=false;
    thrust_up=false;
    thrust_right=false;
    thrust_down=false;
    braking=false;
}

void Ship::brake(double stabilizer){
    double stabilizer_acceleration=stabilizer/mass;

    if(abs(velocity.magnitude)<stabilizer_acceleration){
        stabilizer_acceleration=velocity.magnitude;
        stabilizer=stabilizer_acceleration*mass;
    }

    net_force+=Vector(stabilizer,velocity.opposite().direction);
}

void Ship::apply_thrust(){
    if(thrust_left){
        net_angular_force+=get_angular_thrust()/UPDATE_RATE;
    }
    if(thrust_up){
        net_force+=Vector(get_thrust()/UPDATE_RATE,angle+0.0);
    }
    if(thrust_right){
        net_angular_force-=get_angular_thrust()/UPDATE_RATE;
    }
    if(thrust_down){
        net_force+=Vector(get_thrust()/UPDATE_RATE,angle+180.0);
    }

    if(braking){
        brake(get_brake()/UPDATE_RATE);
    }

    reduce_angle(angle);

    if(!thrust_right && !thrust_left){
        double angular_stabilizer=get_angular_stabilizer()/UPDATE_RATE;
        if(braking){
            angular_stabilizer+=get_brake()/UPDATE_RATE;
        }

        double angular_stabilizer_acceleration=angular_stabilizer/mass;

        if(abs(angular_velocity)<angular_stabilizer_acceleration){
            angular_stabilizer_acceleration=-angular_velocity;
            angular_stabilizer=angular_stabilizer_acceleration*mass;
        }
        else if(angular_velocity>0.0){
            angular_stabilizer*=-1.0;
        }

        net_angular_force+=angular_stabilizer;
    }

    if(!thrust_down && !thrust_up){
        brake(get_stabilizer()/UPDATE_RATE);
    }
}

void Ship::ai(){
    if(is_alive()){
        reset_thrust_input();

        if(faction=="hostile"){
            Ship* ship=&game.world.ships[0];

            double target_angle=get_angle_to_circ(circle,ship->circle,game.camera);

            reduce_angle(angle);
            reduce_angle(velocity.direction);

            double angle_differential=target_angle-angle;
            if(angle_differential>180.0){
                angle_differential-=360.0;
            }
            else if(angle_differential<-180.0){
                angle_differential+=360.0;
            }

            double velocity_differential=0.0;
            if(velocity.magnitude>=32.0){
                velocity_differential=target_angle-velocity.direction;
                if(velocity_differential>180.0){
                    velocity_differential-=360.0;
                }
                else if(velocity_differential<-180.0){
                    velocity_differential+=360.0;
                }
            }

            double velocity_cap=0.0;
            double distance=distance_between_points(circle.x,circle.y,ship->circle.x,ship->circle.y);
            if(distance>128.0){
                velocity_cap=128.0;
            }
            else if(distance<=128.0 && distance>64.0){
                velocity_cap=64.0;
            }
            else if(distance<=64.0 && distance>32.0){
                velocity_cap=32.0;
            }
            else if(distance<=32.0){
                velocity_cap=0.0;
            }

            if(abs(angle_differential)>15.0 && abs(angular_velocity)<200.0){
                if(angle_differential>0.0){
                    thrust_left=true;
                }
                else{
                    thrust_right=true;
                }
            }
            else if(abs(angle_differential)<=15.0 && abs(velocity_differential)<=15.0 && abs(velocity.magnitude)<velocity_cap){
                thrust_up=true;
            }
            else{
                braking=true;
            }
        }
    }
}

void Ship::accelerate(){
    if(is_alive()){
        apply_thrust();

        Object::accelerate();
    }
}

void Ship::movement(uint32_t index){
    if(is_alive()){
        Object::movement(index);

        for(uint32_t i=0;i<game.world.ships.size();i++){
            Ship* ship=&game.world.ships[i];

            if(this!=ship && ship->is_alive()){
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

                if(collision_check_circ(circle,ship->circle)){
                    Collision collision(object1,object2);

                    bool interaction_is_old=false;
                    for(uint32_t j=0;j<game.world.collisions_ship_on_ship.size();j++){
                        if(collision==game.world.collisions_ship_on_ship[j]){
                            interaction_is_old=true;
                            break;
                        }
                    }

                    if(!interaction_is_old){
                        game.world.collisions_ship_on_ship.push_back(collision);
                    }
                }
            }
        }
    }
}

void Ship::collide_with_ship(uint32_t index){
    Ship* ship=&game.world.ships[index];

    if(is_alive() && ship->is_alive() && collision_check_circ(circle,ship->circle)){
        Vector_Components vc_collision=Vector(ship->circle.r+circle.r,get_angle_to_circ(ship->circle,circle,game.camera)).get_components();

        circle.x=ship->circle.x+vc_collision.a;
        circle.y=ship->circle.y+vc_collision.b;

        Vector force_1(velocity.magnitude*mass,velocity.direction);
        Vector force_2(ship->velocity.magnitude*ship->mass,ship->velocity.direction);

        double angular_force_1=angular_velocity*mass;
        double angular_force_2=ship->angular_velocity*ship->mass;

        double adjusted_armor_self=armor*2.0;
        if(adjusted_armor_self>0.99){
            adjusted_armor_self=0.99;
        }
        double adjusted_armor_ship=ship->armor*2.0;
        if(adjusted_armor_ship>0.99){
            adjusted_armor_ship=0.99;
        }

        double damage_ratio_self=1.0-adjusted_armor_self;
        double damage_ratio_ship=1.0-adjusted_armor_ship;

        Vector force_self=force_1.opposite()+force_2;
        Vector force_ship=force_1+force_2.opposite();

        double angular_force_self=-angular_force_1+angular_force_2;
        double angular_force_ship=angular_force_1-angular_force_2;

        Vector damage_self=force_self*damage_ratio_self;
        Vector damage_ship=force_ship*damage_ratio_ship;

        double angular_damage_self=angular_force_self*damage_ratio_self;
        double angular_damage_ship=angular_force_ship*damage_ratio_ship;

        net_force+=force_self-damage_self;
        ship->net_force+=force_ship-damage_ship;

        net_angular_force+=angular_force_self-angular_damage_self;
        ship->net_angular_force+=angular_force_ship-angular_damage_ship;

        Vector damage_total_self(damage_self.magnitude+angular_damage_self,damage_self.direction);
        Vector damage_total_ship(damage_ship.magnitude+angular_damage_ship,damage_ship.direction);

        take_damage(damage_total_self);
        ship->take_damage(damage_total_ship);
    }
}

void Ship::take_damage(Vector damage_force){
    if(is_alive()){
        Vector damage=damage_force/mass;

        double armor_ratio=0.3;

        Vector armor_damage=damage*armor_ratio;
        damage-=armor_damage;

        health-=damage.magnitude;

        armor-=armor_damage.magnitude*0.01;
        if(armor<0.01){
            armor=0.01;
        }
    }
}

void Ship::animate(){
    if(is_alive()){
        if(collision_check_circ_rect(circle*game.camera_zoom,game.camera)){
            Object::animate();

            sprite_thrust.animate();
        }
    }
}

void Ship::render(){
    if(is_alive()){
        if(collision_check_circ_rect(circle*game.camera_zoom,game.camera)){
            if(thrust_right){
                ///sprite_thrust.render((circle.x)*game.camera_zoom-game.camera.x,(circle.y-circle.r/2.0-sprite_thrust.get_height())*game.camera_zoom-game.camera.y,1.0,game.camera_zoom,game.camera_zoom,angle);
            }

            Object::render();
        }
    }
}
