/*
 * pyrrole: a basic audio synthesizer
 * Software Design by Benjamin Wagley
 * Hardware Design by Elijah Mt. Castle
 * CSM HASS327 Synthesizer Project
*/
#include <alsa/asoundlib.h>
#include <iostream>
#include <string>

#include "freq_gen.hpp"

int main(int argc, char* argv[]){
	freq_gen f;

	f.init();
	f.play_sound(std::stoi(argv[1]));


	return 0;
}
