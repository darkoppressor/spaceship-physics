#include "game_constants.h"
#include "world.h"

using namespace std;

double ZOOM_RATE=0.0;
double ZOOM_MIN=0.0;
double ZOOM_MAX=0.0;

uint32_t MAX_BACKGROUND_STAR_SIZE=0;

double GRAVITY=0.0;

double SOLAR_MASS=0.0;
double SOLAR_DENSITY=0.0;
double EARTH_MASS=0.0;
double EARTH_DENSITY=0.0;
double MOON_MASS=0.0;
double MOON_DENSITY=0.0;

double DESPAWN_RADIUS=0.0;

void set_game_constant(string name,string value){
    if(name=="zoom_rate"){
        ZOOM_RATE=string_stuff.string_to_double(value);
    }
    else if(name=="zoom_min"){
        ZOOM_MIN=string_stuff.string_to_double(value);
    }
    else if(name=="zoom_max"){
        ZOOM_MAX=string_stuff.string_to_double(value);
    }
    else if(name=="max_background_star_size"){
        MAX_BACKGROUND_STAR_SIZE=string_stuff.string_to_unsigned_long(value);
    }
    else if(name=="gravity"){
        GRAVITY=string_stuff.string_to_double(value);
    }
    else if(name=="solar_mass"){
        SOLAR_MASS=string_stuff.string_to_double(value);
    }
    else if(name=="solar_density"){
        SOLAR_DENSITY=string_stuff.string_to_double(value);
    }
    else if(name=="earth_mass"){
        EARTH_MASS=string_stuff.string_to_double(value);
    }
    else if(name=="earth_density"){
        EARTH_DENSITY=string_stuff.string_to_double(value);
    }
    else if(name=="moon_mass"){
        MOON_MASS=string_stuff.string_to_double(value);
    }
    else if(name=="moon_density"){
        MOON_DENSITY=string_stuff.string_to_double(value);
    }
    else if(name=="despawn_radius"){
        DESPAWN_RADIUS=string_stuff.string_to_double(value);
    }
}
