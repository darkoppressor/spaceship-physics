#include "engine_interface.h"
#include "world.h"
#include "render.h"

#include <boost/algorithm/string.hpp>

using namespace std;

void Engine_Interface::load_data_game(){
    load_data("ship_type");
}

void Engine_Interface::load_data_script_game(string script,File_IO_Load* load){
    if(script=="ship_type"){
        load_ship_type(load);
    }
}

void Engine_Interface::unload_data_game(){
    ship_types.clear();
}

void Engine_Interface::load_ship_type(File_IO_Load* load){
    ship_types.push_back(Ship_Type());

    bool multi_line_comment=false;

    while(!load->eof()){
        string line="";

        string str_name="name:";
        string str_sprite="sprite:";
        string str_sprite_moving="sprite_moving:";
        string str_health="health:";
        string str_armor="armor:";
        string str_thrust="thrust:";
        string str_angular_thrust="angular_thrust:";
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
        //sprite_moving
        else if(!multi_line_comment && boost::algorithm::starts_with(line,str_sprite_moving)){
            line.erase(0,str_sprite_moving.length());

            ship_types[ship_types.size()-1].sprite_moving=line;
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
