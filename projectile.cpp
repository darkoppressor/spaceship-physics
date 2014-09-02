#include "projectile.h"
#include "world.h"

using namespace std;

Projectile::Projectile(string get_type,double get_mass,Collision_Circ get_circle,Vector get_velocity,double get_angular_velocity){
    type=get_type;

    Object::setup(get_mass,get_circle,get_velocity,get_angular_velocity,get_health_max(),engine_interface.get_projectile_type(type)->sprite);

    counter_sound_thrust=0;
}

double Projectile::get_health_max(){
    return engine_interface.get_projectile_type(type)->health;
}

double Projectile::get_thrust(){
    return engine_interface.get_projectile_type(type)->thrust;
}

string Projectile::get_sound_thrust(){
    return engine_interface.get_projectile_type(type)->sound_thrust;
}

void Projectile::apply_thrust(){
    net_force+=Vector(get_thrust()/UPDATE_RATE,angle);
}

void Projectile::accelerate(){
    if(is_alive()){
        apply_thrust();

        Object::accelerate();
    }
}

void Projectile::movement(uint32_t index){
    if(is_alive()){
        Object::movement();

        for(uint32_t i=0;i<game.world.ships.size();i++){
            Ship* ship=&game.world.ships[i];

            if(ship->is_alive()){
                uint32_t object1=index;
                uint32_t object2=i;

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
                    for(uint32_t j=0;j<game.world.collisions_projectile_on_ship.size();j++){
                        if(collision==game.world.collisions_projectile_on_ship[j]){
                            interaction_is_old=true;
                            break;
                        }
                    }

                    if(!interaction_is_old){
                        game.world.collisions_projectile_on_ship.push_back(collision);
                    }
                }
            }
        }
    }
}

void Projectile::collide_with_ship(uint32_t index){
    Ship* ship=&game.world.ships[index];

    if(is_alive() && ship->is_alive() && collision_check_circ(circle,ship->circle)){
        Vector_Components vc_collision=Vector(ship->circle.r+circle.r,get_angle_to_circ(ship->circle,circle,game.camera)).get_components();

        circle.x=ship->circle.x+vc_collision.a;
        circle.y=ship->circle.y+vc_collision.b;

        Vector force_1(velocity.magnitude*mass,velocity.direction);
        Vector force_2(ship->velocity.magnitude*ship->mass,ship->velocity.direction);

        double angular_force_1=angular_velocity*mass;
        double angular_force_2=ship->angular_velocity*ship->mass;

        double adjusted_armor_ship=ship->armor*2.0;
        if(adjusted_armor_ship>0.99){
            adjusted_armor_ship=0.99;
        }

        double damage_ratio_self=1.0;
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

void Projectile::take_damage(Vector damage_force){
    if(is_alive()){
        Vector damage=damage_force/mass;

        health-=damage.magnitude;

        if(!is_alive()){
            die();
        }
    }
}

void Projectile::die(){
    health=0.0;

    if(game.effect_allowed()){
        Sprite sprite_explosion;
        sprite_explosion.set_name("explosion");

        double scale=(circle.r*2.0)/((sprite_explosion.get_width()+sprite_explosion.get_height())/2.0);

        game.world.effects_transient.push_back(Effect_Transient(circle.x,circle.y,velocity,angular_velocity,scale,angle,"explosion"));
    }

    sound_system.play_sound("explosion_"+string_stuff.num_to_string(game.rng.random_range(0,5)),circle.x,circle.y);
}

void Projectile::animate(){
    if(is_alive()){
        if(collision_check_circ_rect(circle*game.camera_zoom,game.camera)){
            Object::animate();

            double thrust_effect_rate=0.2;

            string sound_thrust=get_sound_thrust();

            if(sound_thrust.length()>0){
                if(counter_sound_thrust>0){
                    counter_sound_thrust--;
                }

                if(counter_sound_thrust==0){
                    double sound_falloff=32.0;

                    counter_sound_thrust=thrust_effect_rate*UPDATE_RATE;
                    sound_system.play_sound(sound_thrust,circle.x,circle.y,sound_falloff);
                }
            }
        }
    }
}
