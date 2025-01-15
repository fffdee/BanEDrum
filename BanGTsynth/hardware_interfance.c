#include "hardware_interfance.h"
#include "BanGTsynth.h"
#include "bg_read.h"
#include "play.h"


AudioPlay audioPlay = {

    .Callbaclk = PlayCallback,

};
BG_Reader BG_reader = {

    .Callback = bg_read_callback,

};
