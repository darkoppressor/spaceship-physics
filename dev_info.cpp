#include "engine_interface.h"
#include "world.h"
#include "render.h"

using namespace std;

void Engine_Interface::render_dev_info(){
    string msg="";

    if(game.in_progress){
        /**msg+="Camera Position: "+string_stuff.num_to_string(game.camera.x*game.camera_zoom)+","+string_stuff.num_to_string(game.camera.y*game.camera_zoom)+"\n";
        msg+="Camera Size: "+string_stuff.num_to_string(game.camera.w/game.camera_zoom)+","+string_stuff.num_to_string(game.camera.h/game.camera_zoom)+"\n";
        msg+="Camera Zoom: "+string_stuff.num_to_string(game.camera_zoom)+"\n";
        msg+="\n";*/

        msg+="\n";

        msg+="Ships: "+string_stuff.num_to_string(game.world.ships.size())+"\n";
        msg+="\n";
        msg+="Mass: "+string_stuff.num_to_string(game.world.ships[0].mass)+" kg"+"\n";
        msg+="Radius: "+string_stuff.num_to_string(game.world.ships[0].circle.r)+" m"+"\n";
        msg+="Density: "+string_stuff.num_to_string(game.world.ships[0].get_density())+" kg/m^3"+"\n";
        msg+="Position: "+string_stuff.num_to_string(game.world.ships[0].circle.x)+","+string_stuff.num_to_string(game.world.ships[0].circle.y)+"\n";
        msg+="Velocity: "+string_stuff.num_to_string(game.world.ships[0].velocity.magnitude)+" m/s "+string_stuff.num_to_string(game.world.ships[0].velocity.direction)+string(1,(unsigned char)167)+"\n";
        msg+="Momentum: "+string_stuff.num_to_string(game.world.ships[0].get_momentum().magnitude)+" N s "+string_stuff.num_to_string(game.world.ships[0].get_momentum().direction)+string(1,(unsigned char)167)+"\n";
        msg+="Acceleration: "+string_stuff.num_to_string(game.world.ships[0].acceleration.magnitude)+" m/s"+string(1,(unsigned char)253)+" "+string_stuff.num_to_string(game.world.ships[0].velocity.direction)+string(1,(unsigned char)167)+"\n";
        msg+="Angle: "+string_stuff.num_to_string(game.world.ships[0].angle)+string(1,(unsigned char)167)+"\n";
        msg+="Angular Velocity: "+string_stuff.num_to_string(game.world.ships[0].angular_velocity)+" degrees/s "+"\n";
        msg+="Angular Momentum: "+string_stuff.num_to_string(game.world.ships[0].get_angular_momentum())+" N s "+"\n";
        msg+="Angular Acceleration: "+string_stuff.num_to_string(game.world.ships[0].angular_acceleration)+" degrees/s"+string(1,(unsigned char)253)+"\n";
        msg+="Hull: "+string_stuff.num_to_string(game.world.ships[0].health)+"/"+string_stuff.num_to_string(game.world.ships[0].get_health_max())+"\n";
        msg+="Armor: "+string_stuff.num_to_string(game.world.ships[0].armor)+"/"+string_stuff.num_to_string(game.world.ships[0].get_armor_max())+"\n";

        double angle_differential=330.0-45.0;
        if(angle_differential>180.0){
            angle_differential-=360.0;
        }
        else if(angle_differential<180.0){
            angle_differential+=360.0;
        }
        msg+="\n";
        msg+="lol: "+string_stuff.num_to_string(angle_differential)+"\n";
    }

    if(msg.length()>0){
        Bitmap_Font* font=engine_interface.get_font("small");

        render_rectangle(2,2,string_stuff.longest_line(msg)*font->spacing_x,string_stuff.newline_count(msg)*font->spacing_y,0.75,"ui_black");
        font->show(2,2,msg,"red");
    }
}
