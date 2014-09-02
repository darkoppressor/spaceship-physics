#include "ship.h"
#include "world.h"

using namespace std;

Ship::Ship(string get_type,double get_mass,Collision_Circ get_circle,Vector get_velocity,double get_angular_velocity,string get_faction,vector<string> get_weapons){
    type=get_type;

    Object::setup(get_mass,get_circle,get_velocity,get_angular_velocity,get_health_max(),engine_interface.get_ship_type(type)->sprite);

    reset_thrust_input();
    reset_stabilizer_input();

    counter_thrust_left=0;
    counter_thrust_up=0;
    counter_thrust_right=0;
    counter_thrust_down=0;
    counter_stabilize_left=0;
    counter_stabilize_right=0;
    counter_stabilize_brake=0;
    counter_sound_thrust=0;
    counter_sound_stabilize=0;

    stabilizer_positional=true;
    stabilizer_angular=true;

    armor=get_armor_max();

    faction=get_faction;

    for(int i=0;i<get_weapons.size();i++){
        weapons.push_back(Weapon(get_weapons[i]));
    }
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

Coords Ship::get_thruster_up(){
    return engine_interface.get_ship_type(type)->thruster_up;
}

Coords Ship::get_thruster_down(){
    return engine_interface.get_ship_type(type)->thruster_down;
}

Coords Ship::get_thruster_left(){
    return engine_interface.get_ship_type(type)->thruster_left;
}

Coords Ship::get_thruster_right(){
    return engine_interface.get_ship_type(type)->thruster_right;
}

void Ship::reset_thrust_input(){
    thrust_left=false;
    thrust_up=false;
    thrust_right=false;
    thrust_down=false;
    braking=false;
}

void Ship::reset_stabilizer_input(){
    stabilize_left=false;
    stabilize_right=false;
    stabilize_brake=false;
    stabilize_brake_vector*=0.0;
}

void Ship::brake(double stabilizer){
    double stabilizer_acceleration=stabilizer/mass;

    if(abs(velocity.magnitude)<stabilizer_acceleration){
        stabilizer_acceleration=velocity.magnitude;
        stabilizer=stabilizer_acceleration*mass;
    }

    Vector brake_force(stabilizer,velocity.opposite().direction);

    net_force+=brake_force;

    if(brake_force.magnitude>0.0){
        stabilize_brake=true;
        stabilize_brake_vector.magnitude=stabilizer_acceleration;
        stabilize_brake_vector.direction=velocity.direction;
    }
}

void Ship::apply_thrust(){
    reset_stabilizer_input();

    if(thrust_left){
        net_angular_force+=get_angular_thrust()/UPDATE_RATE;
    }
    if(thrust_up){
        net_force+=Vector(get_thrust()/UPDATE_RATE,angle);
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

    if(stabilizer_angular && !thrust_right && !thrust_left){
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

        if(angular_stabilizer>0.0){
            stabilize_left=true;
        }
        else if(angular_stabilizer<0.0){
            stabilize_right=true;
        }
    }

    if(stabilizer_positional && !thrust_down && !thrust_up){
        brake(get_stabilizer()/UPDATE_RATE);
    }
}

void Ship::ai(){
    if(is_alive()){
        reset_thrust_input();

        if(faction=="bad"){
            Ship* ship=game.world.get_player();

            if(ship->is_alive()){
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
}

void Ship::fire_weapons(){
    if(is_alive()){
        int weapons_ready=0;

        for(int i=0;i<weapons.size();i++){
            weapons[i].cool();

            if(weapons[i].cooled()){
                weapons_ready++;
            }
        }

        for(int i=0;i<game.world.ships.size() && weapons_ready>0;i++){
            Ship* ship=&game.world.ships[i];

            if(this!=ship && ship->is_alive() && faction!=ship->faction){
                for(int n=0;n<weapons.size();n++){
                    if(weapons[n].cooled() && collision_check_circ(Collision_Circ(circle.x,circle.y,weapons[n].get_targeting_radius()),ship->circle)){
                        weapons[n].fire("0",circle,get_angle_to_circ(circle,ship->circle,game.camera),velocity,angular_velocity);
                        weapons_ready--;
                    }
                }
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
        Object::movement();

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

        if(!is_alive()){
            die();
        }
    }
}

void Ship::die(){
    health=0.0;

    if(game.effect_allowed()){
        Sprite sprite_explosion;
        sprite_explosion.set_name("explosion");

        double scale=(circle.r*2.0)/((sprite_explosion.get_width()+sprite_explosion.get_height())/2.0);

        game.world.effects_transient.push_back(Effect_Transient(circle.x,circle.y,velocity,angular_velocity,scale,angle,"explosion"));
    }

    sound_system.play_sound("explosion_"+string_stuff.num_to_string(game.rng.random_range(0,5)),circle.x,circle.y);
}

void Ship::create_thrust_effect(string direction,double scale){
    Sprite sprite_thrust;
    sprite_thrust.set_name("thrust");

    double thruster_scale=(circle.r*2.0)/(sprite_thrust.get_width()+sprite_thrust.get_height());

    Coords thruster_coords;
    double thrust_angle=0.0;
    if(direction=="left"){
        thruster_coords=get_thruster_left();
        thrust_angle=-90.0;
    }
    else if(direction=="right"){
        thruster_coords=get_thruster_right();
        thrust_angle=90.0;
    }
    else if(direction=="up"){
        thruster_coords=get_thruster_up();
        thrust_angle=180.0;
    }
    else if(direction=="down"){
        thruster_coords=get_thruster_down();
        thrust_angle=0.0;
    }
    else if(direction=="none"){
        thruster_coords=get_thruster_down();
        thrust_angle=stabilize_brake_vector.direction;
    }

    Collision_Circ circle_thruster(circle.x-circle.r+(double)thruster_coords.x,circle.y-circle.r+(double)thruster_coords.y,0.0);

    Vector thruster_position(abs(distance_between_points(circle.x,circle.y,circle_thruster.x,circle_thruster.y)),get_angle_to_circ(circle,circle_thruster,game.camera));
    if(direction!="none"){
        thruster_position.direction+=angle;
    }
    else{
        thruster_position.direction=stabilize_brake_vector.direction;
    }
    Vector_Components vc=thruster_position.get_components();

    Vector thruster_velocity=velocity;
    if(direction!="none"){
        thruster_velocity.direction+=thrust_angle;
        thruster_velocity.magnitude*=1.1;
    }
    else{
        thruster_velocity.direction=thrust_angle;
        thruster_velocity.magnitude+=stabilize_brake_vector.magnitude;
    }

    game.world.effects_transient.push_back(Effect_Transient(circle.x+vc.a,circle.y+vc.b,thruster_velocity,angular_velocity,thruster_scale*scale,angle,"thrust"));
}

void Ship::animate(){
    if(is_alive()){
        if(collision_check_circ_rect(circle*game.camera_zoom,game.camera)){
            Object::animate();

            double thrust_effect_rate=0.2;

            double sound_falloff=32.0;
            if(this==game.world.get_player()){
                sound_falloff=-1.0;
            }

            if(counter_sound_thrust>0){
                counter_sound_thrust--;
            }
            if(counter_sound_stabilize>0){
                counter_sound_stabilize--;
            }
            if(counter_thrust_left>0){
                counter_thrust_left--;
            }
            if(counter_thrust_right>0){
                counter_thrust_right--;
            }
            if(counter_thrust_up>0){
                counter_thrust_up--;
            }
            if(counter_thrust_down>0){
                counter_thrust_down--;
            }
            if(counter_stabilize_left>0){
                counter_stabilize_left--;
            }
            if(counter_stabilize_right>0){
                counter_stabilize_right--;
            }
            if(counter_stabilize_brake>0){
                counter_stabilize_brake--;
            }

            if((thrust_left || thrust_up || thrust_right || thrust_down) && counter_sound_thrust==0){
                counter_sound_thrust=thrust_effect_rate*UPDATE_RATE;
                sound_system.play_sound("thrust",circle.x,circle.y,sound_falloff);
            }

            if((stabilize_left || stabilize_right || stabilize_brake) && counter_sound_stabilize==0){
                counter_sound_stabilize=thrust_effect_rate*UPDATE_RATE;
                sound_system.play_sound("stabilize",circle.x,circle.y,sound_falloff);
            }

            if(thrust_left){
                if(game.effect_allowed() && counter_thrust_left==0){
                    counter_thrust_left=thrust_effect_rate*UPDATE_RATE;
                    create_thrust_effect("left");
                }
            }
            if(thrust_right){
                if(game.effect_allowed() && counter_thrust_right==0){
                    counter_thrust_right=thrust_effect_rate*UPDATE_RATE;
                    create_thrust_effect("right");
                }
            }
            if(thrust_up){
                if(game.effect_allowed() && counter_thrust_up==0){
                    counter_thrust_up=thrust_effect_rate*UPDATE_RATE;
                    create_thrust_effect("up");
                }
            }
            if(thrust_down){
                if(game.effect_allowed() && counter_thrust_down==0){
                    counter_thrust_down=thrust_effect_rate*UPDATE_RATE;
                    create_thrust_effect("down");
                }
            }

            if(stabilize_left){
                if(game.effect_allowed() && counter_stabilize_left==0){
                    counter_stabilize_left=thrust_effect_rate*UPDATE_RATE;
                    create_thrust_effect("left",0.5);
                }
            }
            if(stabilize_right){
                if(game.effect_allowed() && counter_stabilize_right==0){
                    counter_stabilize_right=thrust_effect_rate*UPDATE_RATE;
                    create_thrust_effect("right",0.5);
                }
            }
            if(stabilize_brake){
                if(game.effect_allowed() && counter_stabilize_brake==0){
                    counter_stabilize_brake=thrust_effect_rate*UPDATE_RATE;
                    create_thrust_effect("none",0.5);
                }
            }
        }
    }
}
