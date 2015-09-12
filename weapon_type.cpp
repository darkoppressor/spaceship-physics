/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "weapon_type.h"

using namespace std;

Weapon_Type::Weapon_Type(){
    name="";
    sound="";

    cooldown=0.0;

    targeting_radius=0.0;

    force=0.0;
}
