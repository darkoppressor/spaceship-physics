/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "window.h"
#include "world.h"

using namespace std;

void Window::exec_close_function(){
    if(close_function.length()>0){
        if(close_function=="configure_command"){
            engine_interface.configure_command=-1;
        }
        else{
            message_log.add_error("Invalid close function: '"+close_function+"'");
        }
    }
}
