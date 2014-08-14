#ifndef game_constants_h
#define game_constants_h

#include <string>
#include <stdint.h>

extern double ZOOM_RATE;
extern double ZOOM_MIN;
extern double ZOOM_MAX;

extern double SPEED_OF_LIGHT;
extern double GRAVITY;

extern double SOLAR_MASS;
extern double SOLAR_DENSITY;
extern double EARTH_MASS;
extern double EARTH_DENSITY;
extern double MOON_MASS;
extern double MOON_DENSITY;
extern double SHIP_DENSITY;
extern double HUMAN_MASS;
extern double HUMAN_DENSITY;

extern uint32_t SHIPS;

void set_game_constant(std::string name,std::string value);

#endif
