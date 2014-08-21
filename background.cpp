#include "background.h"
#include "render.h"
#include "world.h"

using namespace std;

Background_Layer::Background_Layer(double get_width,double get_height,double get_parallax_speed_x,double get_parallax_speed_y){
    width=get_width;
    height=get_height;

    parallax_speed_x=get_parallax_speed_x;
    parallax_speed_y=get_parallax_speed_y;

    position_x_a=0.0;
    position_x_b=0.0;
    position_y_a=0.0;
    position_y_b=0.0;
}

void Background_Layer::generate_texture_background(int layer_number){
    layer_image.w=width;
    layer_image.h=height;

    SDL_Surface* surface=0;

    Uint32 rmask,gmask,bmask,amask;
    engine_interface.get_rgba_masks(&rmask,&gmask,&bmask,&amask);

    surface=SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32,rmask,gmask,bmask,amask);

    if(SDL_MUSTLOCK(surface)){
        SDL_LockSurface(surface);
    }

    Color pixel_color;

    //Set the background color.
    pixel_color.set_rgb(0,0,0,0);

    //Apply the background color.
    for(int x=0;x<surface->w;x++){
        for(int y=0;y<surface->h;y++){
            surface_put_pixel(surface,x,y,pixel_color);
        }
    }

    //Create the stars:

    //The number of star attempts to make.
    int base_stars=50;
    int multiplier_stars=10;

    int max_star_attempts=base_stars-layer_number*multiplier_stars;

    int star_size=game.option_background_count-(int)ceil((double)layer_number*2.0);
    if(star_size>MAX_BACKGROUND_STAR_SIZE){
        star_size=MAX_BACKGROUND_STAR_SIZE;
    }
    if(star_size<1){
        star_size=1;
    }

    for(int star_attempts=0;star_attempts<max_star_attempts;star_attempts++){
        //Set the star color.
        Color* color=0;

        int random=game.rng.random_range(0,99);

        if(random>=0 && random<4){
            color=engine_interface.get_color("star_blue");
        }
        else if(random>=4 && random<9){
            color=engine_interface.get_color("star_white");
        }
        else if(random>=9 && random<17){
            color=engine_interface.get_color("star_yellow");
        }
        else if(random>=17 && random<29){
            color=engine_interface.get_color("star_orange");
        }
        else if(random>=29 && random<100){
            color=engine_interface.get_color("star_red");
        }

        pixel_color.set_rgb(color->get_red_short(),color->get_green_short(),color->get_blue_short(),255);

        int x=game.rng.random_range(1,surface->w-1-star_size);
        int y=game.rng.random_range(1,surface->h-1-star_size);

        for(int int_x=x;int_x<x+star_size;int_x++){
            for(int int_y=y;int_y<y+star_size;int_y++){
                surface_put_pixel(surface,int_x,int_y,pixel_color);
            }
        }
    }

    if(SDL_MUSTLOCK(surface)){
        SDL_UnlockSurface(surface);
    }

    layer_image.load_image(surface);

    //We have copied the image data into the new texture, so delete the image data.
    SDL_FreeSurface(surface);
}

void Background_Layer::update(double camera_delta_x,double camera_delta_y){
    //Update the x position.

    position_x_a-=camera_delta_x/parallax_speed_x;

    if(position_x_a+width<0){
        position_x_a=0;
    }
    else if(position_x_a>0){
        position_x_a=-width;
    }

    position_x_b=position_x_a+width;

    //Update the y position.

    position_y_a-=camera_delta_y/parallax_speed_y;

    if(position_y_a+height<0){
        position_y_a=0;
    }
    else if(position_y_a>0){
        position_y_a=-height;
    }

    position_y_b=position_y_a+height;
}

void Background_Layer::render(double opacity){
    render_texture((int)position_x_a,(int)position_y_a,layer_image,opacity);
    render_texture((int)position_x_b,(int)position_y_a,layer_image,opacity);
    render_texture((int)position_x_a,(int)position_y_b,layer_image,opacity);
    render_texture((int)position_x_b,(int)position_y_b,layer_image,opacity);
}

Background::Background(){
    opacity=1.0;

    for(int i=0;i<game.option_background_count;i++){
        background_layers.push_back(Background_Layer(main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,2.0+2.0*(double)i,2.0+2.0*(double)i));
    }

    //Generate the background layer textures.
    for(int i=0;i<background_layers.size();i++){
        background_layers[i].generate_texture_background(i);
    }
}

void Background::unload(){
    for(int i=0;i<background_layers.size();i++){
        background_layers[i].layer_image.unload_image();
    }
    background_layers.clear();
}

void Background::update(double camera_delta_x,double camera_delta_y){
    for(int i=0;i<background_layers.size();i++){
        background_layers[i].update(camera_delta_x,camera_delta_y);
    }
}

void Background::render(){
    for(int i=0;i<background_layers.size();i++){
        background_layers[i].render(opacity);
    }
}
