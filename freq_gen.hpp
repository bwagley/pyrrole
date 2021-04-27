#include <alsa/asoundlib.h>
#include <cmath>
#include <climits>
#include <iostream>

#define PI 3.1415926

class freq_gen{
public:
	freq_gen(){}

	void init(){
		vals = new short int(len);
    		snd_pcm_hw_params_alloca(&hwparams);

    		err = snd_pcm_open(&pcm_dev, pcm_name, stream, 0);
    		std::cout << "Opening: " << snd_strerror(err) << std::endl;

    		err = snd_pcm_hw_params_any(pcm_dev, hwparams);
		std::cout << "Initializing hwparams structure: " << snd_strerror(err) << std::endl;   
	
    		err = snd_pcm_hw_params_set_access(pcm_dev, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED);
    		std::cout << "Setting access: " << snd_strerror(err) << std::endl;

    		err = snd_pcm_hw_params_set_format(pcm_dev, hwparams, SND_PCM_FORMAT_S16_LE);
    		std::cout << "Setting format: " << snd_strerror(err) << std::endl;

    		err = snd_pcm_hw_params_set_rate(pcm_dev, hwparams, rate, (int)0);
    		std::cout << "Setting rate: " << snd_strerror(err) << std::endl;

    		err = snd_pcm_hw_params_set_channels(pcm_dev, hwparams, 2); 
    		std::cout << "Setting channels: " << snd_strerror(err) << std::endl;

		err = snd_pcm_hw_params_set_periods(pcm_dev, hwparams, 2, 0);
		std::cout << "Setting periods: " << snd_strerror(err) << std::endl;

    		err = snd_pcm_hw_params_set_buffer_size_near(pcm_dev, hwparams, &bufferSize);
		std::cout << "Setting buffer size: " << snd_strerror(err) << std::endl;

 	         err = snd_pcm_hw_params(pcm_dev, hwparams);
   		 std::cout << "Applying parameters: " << snd_strerror(err) << std::endl;

		 std::cout << std::endl << std::endl;
	}

	void sine_wav(uint16_t freq){
		for(int i = 0; i < len; i++){
			vals[i] = SHRT_MAX * cos(3.14159 * freq / rate * i);
		}
	}

	void play_sound(uint16_t freq){
		int err;
		while(true){
			err = snd_pcm_writei(pcm_dev, vals, len);
		}
	}

    	int err;
	
    	snd_pcm_t* pcm_dev;
    	snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
    	snd_pcm_hw_params_t* hwparams;  // hardware information
    	char* pcm_name = strdup("default");  // on-board audio jack
    	int rate = 48000;

    	long unsigned int bufferSize = 8192*4;
    	uint32_t len = bufferSize*100;
    	short int *vals;
};
