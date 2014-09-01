#include "engine_interface.h"
#include "world.h"
#include "render.h"

#include <boost/algorithm/string.hpp>

using namespace std;

void Engine_Interface::load_data_game(){
    load_data("ship_type");
    load_data("projectile_type");
    load_data("weapon_type");
}

void Engine_Interface::load_data_script_game(string script,File_IO_Load* load){
    if(script=="ship_type"){
        load_ship_type(load);
    }
    else if(script=="projectile_type"){
        load_projectile_type(load);
    }
    else if(script=="weapon_type"){
        load_weapon_type(load);
    }
}

void Engine_Interface::unload_data_game(){
    ship_types.clear();
    projectile_types.clear();
    weapon_types.clear();
}

void Engine_Interface::load_ship_type(File_IO_Load* load){
    ship_types.push_back(Ship_Type());

    bool multi_line_comment=false;

    while(!load->eof()){
        string line="";

        string str_name="name:";
        string str_sprite="sprite:";
        string str_thruster_up="thruster_up:";
        string str_thruster_down="thruster_down:";
        string str_thruster_left="thruster_left:";
        string str_thruster_right="thruster_right:";
        string str_health="health:";
        string str_armor="armor:";
        string str_density="density:";
        string str_thrust="thrust:";
        string str_angular_thrust="angular_thrust:";
        string str_brake="brake:";
        string str_stabilizer="stabilizer:";
        string str_angular_stabilizer="angular_stabilizer:";

        load->getline(&line);
        boost::algorithm::trim(line);

        if(boost::algorithm::contains(line,"*/")){
            multi_line_comment=false;
        }
        if(!multi_line_comment && boost::algorithm::starts_with(line,"/*")){
            multi_line_comment=true;
        }
        else if(!multi_line_comment && boost::algorithm::starts_with(line,"//")){
        }

        //name
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_name)){
            line.erase(0,str_name.length());

            ship_types[ship_types.size()-1].name=line;
        }
        //sprite
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_sprite)){
            line.erase(0,str_sprite.length());

            ship_types[ship_types.size()-1].sprite=line;
        }
        //thruster_up
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_thruster_up)){
            line.erase(0,str_thruster_up.length());

            vector<string> coords;
            boost::algorithm::split(coords,line,boost::algorithm::is_any_of(","));

            if(coords.size()>=2){
                ship_types[ship_types.size()-1].thruster_up.x=string_stuff.string_to_long(coords[0]);
                ship_types[ship_types.size()-1].thruster_up.y=string_stuff.string_to_long(coords[1]);
            }
        }
        //thruster_down
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_thruster_down)){
            line.erase(0,str_thruster_down.length());

            vector<string> coords;
            boost::algorithm::split(coords,line,boost::algorithm::is_any_of(","));

            if(coords.size()>=2){
                ship_types[ship_types.size()-1].thruster_down.x=string_stuff.string_to_long(coords[0]);
                ship_types[ship_types.size()-1].thruster_down.y=string_stuff.string_to_long(coords[1]);
            }
        }
        //thruster_left
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_thruster_left)){
            line.erase(0,str_thruster_left.length());

            vector<string> coords;
            boost::algorithm::split(coords,line,boost::algorithm::is_any_of(","));

            if(coords.size()>=2){
                ship_types[ship_types.size()-1].thruster_left.x=string_stuff.string_to_long(coords[0]);
                ship_types[ship_types.size()-1].thruster_left.y=string_stuff.string_to_long(coords[1]);
            }
        }
        //thruster_right
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_thruster_right)){
            line.erase(0,str_thruster_right.length());

            vector<string> coords;
            boost::algorithm::split(coords,line,boost::algorithm::is_any_of(","));

            if(coords.size()>=2){
                ship_types[ship_types.size()-1].thruster_right.x=string_stuff.string_to_long(coords[0]);
                ship_types[ship_types.size()-1].thruster_right.y=string_stuff.string_to_long(coords[1]);
            }
        }
        //health
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_health)){
            line.erase(0,str_health.length());

            ship_types[ship_types.size()-1].health=string_stuff.string_to_double(line);
        }
        //armor
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_armor)){
            line.erase(0,str_armor.length());

            ship_types[ship_types.size()-1].armor=string_stuff.string_to_double(line);
        }
        //density
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_density)){
            line.erase(0,str_density.length());

            ship_types[ship_types.size()-1].density=string_stuff.string_to_double(line);
        }
        //thrust
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_thrust)){
            line.erase(0,str_thrust.length());

            ship_types[ship_types.size()-1].thrust=string_stuff.string_to_double(line);
        }
        //angular_thrust
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_angular_thrust)){
            line.erase(0,str_angular_thrust.length());

            ship_types[ship_types.size()-1].angular_thrust=string_stuff.string_to_double(line);
        }
        //brake
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_brake)){
            line.erase(0,str_brake.length());

            ship_types[ship_types.size()-1].brake=string_stuff.string_to_double(line);
        }
        //stabilizer
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_stabilizer)){
            line.erase(0,str_stabilizer.length());

            ship_types[ship_types.size()-1].stabilizer=string_stuff.string_to_double(line);
        }
        //angular_stabilizer
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_angular_stabilizer)){
            line.erase(0,str_angular_stabilizer.length());

            ship_types[ship_types.size()-1].angular_stabilizer=string_stuff.string_to_double(line);
        }

        else if(!multi_line_comment && boost::algorithm::starts_with(line,"</ship_type>")){
            return;
        }
    }
}

void Engine_Interface::load_projectile_type(File_IO_Load* load){
    projectile_types.push_back(Projectile_Type());

    bool multi_line_comment=false;

    while(!load->eof()){
        string line="";

        string str_name="name:";
        string str_sprite="sprite:";
        string str_sound_thrust="sound_thrust:";
        string str_health="health:";
        string str_density="density:";
        string str_thrust="thrust:";

        load->getline(&line);
        boost::algorithm::trim(line);

        if(boost::algorithm::contains(line,"*/")){
            multi_line_comment=false;
        }
        if(!multi_line_comment && boost::algorithm::starts_with(line,"/*")){
            multi_line_comment=true;
        }
        else if(!multi_line_comment && boost::algorithm::starts_with(line,"//")){
        }

        //name
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_name)){
            line.erase(0,str_name.length());

            projectile_types[projectile_types.size()-1].name=line;
        }
        //sprite
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_sprite)){
            line.erase(0,str_sprite.length());

            projectile_types[projectile_types.size()-1].sprite=line;
        }
        //sound_thrust
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_sound_thrust)){
            line.erase(0,str_sound_thrust.length());

            projectile_types[projectile_types.size()-1].sound_thrust=line;
        }
        //health
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_health)){
            line.erase(0,str_health.length());

            projectile_types[projectile_types.size()-1].health=string_stuff.string_to_double(line);
        }
        //density
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_density)){
            line.erase(0,str_density.length());

            projectile_types[projectile_types.size()-1].density=string_stuff.string_to_double(line);
        }
        //thrust
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_thrust)){
            line.erase(0,str_thrust.length());

            projectile_types[projectile_types.size()-1].thrust=string_stuff.string_to_double(line);
        }

        else if(!multi_line_comment && boost::algorithm::starts_with(line,"</projectile_type>")){
            return;
        }
    }
}

void Engine_Interface::load_weapon_type(File_IO_Load* load){
    weapon_types.push_back(Weapon_Type());

    bool multi_line_comment=false;

    while(!load->eof()){
        string line="";

        string str_name="name:";
        string str_sound="sound:";
        string str_cooldown="cooldown:";
        string str_targeting_radius="targeting_radius:";
        string str_force="force:";

        load->getline(&line);
        boost::algorithm::trim(line);

        if(boost::algorithm::contains(line,"*/")){
            multi_line_comment=false;
        }
        if(!multi_line_comment && boost::algorithm::starts_with(line,"/*")){
            multi_line_comment=true;
        }
        else if(!multi_line_comment && boost::algorithm::starts_with(line,"//")){
        }

        //name
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_name)){
            line.erase(0,str_name.length());

            weapon_types[weapon_types.size()-1].name=line;
        }
        //sound
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_sound)){
            line.erase(0,str_sound.length());

            weapon_types[weapon_types.size()-1].sound=line;
        }
        //cooldown
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_cooldown)){
            line.erase(0,str_cooldown.length());

            weapon_types[weapon_types.size()-1].cooldown=string_stuff.string_to_double(line);
        }
        //targeting_radius
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_targeting_radius)){
            line.erase(0,str_targeting_radius.length());

            weapon_types[weapon_types.size()-1].targeting_radius=string_stuff.string_to_double(line);
        }
        //force
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_force)){
            line.erase(0,str_force.length());

            weapon_types[weapon_types.size()-1].force=string_stuff.string_to_double(line);
        }

        else if(!multi_line_comment && boost::algorithm::starts_with(line,"</weapon_type>")){
            return;
        }
    }
}

Ship_Type* Engine_Interface::get_ship_type(string name){
    Ship_Type* ptr_object=0;

    for(int i=0;i<ship_types.size();i++){
        if(ship_types[i].name==name){
            ptr_object=&ship_types[i];

            break;
        }
    }

    if(ptr_object==0){
        message_log.add_error("Error accessing ship type '"+name+"'");
    }

    return ptr_object;
}

Projectile_Type* Engine_Interface::get_projectile_type(string name){
    Projectile_Type* ptr_object=0;

    for(int i=0;i<projectile_types.size();i++){
        if(projectile_types[i].name==name){
            ptr_object=&projectile_types[i];

            break;
        }
    }

    if(ptr_object==0){
        message_log.add_error("Error accessing projectile type '"+name+"'");
    }

    return ptr_object;
}

Weapon_Type* Engine_Interface::get_weapon_type(string name){
    Weapon_Type* ptr_object=0;

    for(int i=0;i<weapon_types.size();i++){
        if(weapon_types[i].name==name){
            ptr_object=&weapon_types[i];

            break;
        }
    }

    if(ptr_object==0){
        message_log.add_error("Error accessing weapon type '"+name+"'");
    }

    return ptr_object;
}

void Engine_Interface::open_window(Window* window){
    if(!is_window_open(window)){
        window_z_order.insert(window_z_order.begin(),window);

        reset_gui_selected_object();
    }
}

void Engine_Interface::close_window(Window* window){
    for(int i=0;i<window_z_order.size();i++){
        if(window_z_order[i]==window){
            window_z_order.erase(window_z_order.begin()+i);

            reset_gui_selected_object();

            break;
        }
    }
}

void Engine_Interface::render_title_background(){
    Bitmap_Font* font=get_font("small");

    render_rectangle(0,0,main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,1.0,"ui_black");

    font->show(0,main_window.SCREEN_HEIGHT-font->spacing_y*2.0,"Version "+get_version()+"\nChecksum "+CHECKSUM,"ui_white");

    Image_Data* logo=image.get_image("logo");

    render_texture(main_window.SCREEN_WIDTH-logo->w*0.25,main_window.SCREEN_HEIGHT-logo->h*0.25,*logo,1.0,0.25,0.25);
}

void Engine_Interface::render_pause(){
    Bitmap_Font* font=get_font("standard");

    string msg="Paused";
    font->show((main_window.SCREEN_WIDTH-(font->spacing_x*msg.length()))/2,(main_window.SCREEN_HEIGHT-font->spacing_y)/2,msg,"ui_white");
}

void Engine_Interface::render_fps(int render_rate,double ms_per_frame,int logic_frame_rate){
    get_font("small")->show(2,2,"FPS: "+string_stuff.num_to_string(render_rate)+"\n"+network.get_stats(),"ui_white");
}

void Engine_Interface::render_loading_screen(double percentage,string load_message){
    Bitmap_Font* font=get_font("standard");

    SDL_SetRenderDrawColor(main_window.renderer,0,0,0,255);
    SDL_RenderClear(main_window.renderer);

    ///If images are loaded
    /**render_texture(0,0,image.loading_screen_main,0.25);
    render_texture((main_window.SCREEN_WIDTH-image.logo_hubert.w)/2.0,30,image.logo_hubert);*/

    render_rectangle(0,0,main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,1.0,"ui_black");

    double bar_width=240.0*percentage;
    double max_bar_width=240.0*1.0;
    render_rectangle(main_window.SCREEN_WIDTH/2.0-120-2,main_window.SCREEN_HEIGHT-75-2,max_bar_width+4,30+4,1.0,"ui_3");
    render_rectangle(main_window.SCREEN_WIDTH/2.0-120,main_window.SCREEN_HEIGHT-75,bar_width,30,1.0,"ui_1");

    string msg=string_stuff.num_to_string((int)(percentage*100.0))+"%";

    font->show(main_window.SCREEN_WIDTH/2.0-120+(max_bar_width-msg.length()*font->spacing_x)/2.0,main_window.SCREEN_HEIGHT-75+font->spacing_y/4.0,msg,"ui_0");

    font->show((main_window.SCREEN_WIDTH-load_message.length()*font->spacing_x)/2.0,main_window.SCREEN_HEIGHT-75+font->spacing_y*2.0,load_message,"ui_0");

    SDL_RenderPresent(main_window.renderer);
}

string Engine_Interface::get_game_window_caption(){
    string msg="";

    return msg;
}
