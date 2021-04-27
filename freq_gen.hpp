#include <alsa/asoundlib.h>
#include <cmath>
#include <climits>
#include <iostream>

#define PI 3.1415926

class freq_gen{
public:
	freq_gen(){
		std::cout << "Opening PCM Device" << std::endl;
		_err = snd_pcm_open(&_pcm_dev, "default", SND_PCM_STREAM_PLAYBACK, 0);
		std::cout << snd_strerror(_err) << std::endl;
	}

	void init(){
		vals = new short int(_len);
		snd_pcm_hw_params_alloca(&_hw_params);

		snd_pcm_hw_params_any(_pcm_dev, _hw_params);

		std::cout << "Setting Access" << std::endl;
		_err = snd_pcm_hw_params_set_access(_pcm_dev, _hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
		std::cout << snd_strerror(_err) << std::endl;

		std::cout << "Setting Format" << std::endl;
		_err = snd_pcm_hw_params_set_format(_pcm_dev, _hw_params, SND_PCM_FORMAT_S16_LE);
		std::cout << snd_strerror(_err) << std::endl;

		std::cout << "Setting Rate" << std::endl;
		_err = snd_pcm_hw_params_set_rate(_pcm_dev, _hw_params, _rate, (int) 0);
		std::cout << snd_strerror(_err) << std::endl;

		std::cout << "Setting Channels" << std::endl;
		_err = snd_pcm_hw_params_set_channels(_pcm_dev, _hw_params, 2);
		std::cout << snd_strerror(_err) << std::endl;

		std::cout << "Setting Periods" << std::endl;
		_err = snd_pcm_hw_params_set_periods(_pcm_dev, _hw_params, 2, 0);
		std::cout << snd_strerror(_err) << std::endl;

		std::cout << "Setting Buffer Size" << std::endl;
		_err = snd_pcm_hw_params_set_buffer_size_near(_pcm_dev, _hw_params, &_buf_size);
		std::cout << snd_strerror(_err) << std::endl;

		std::cout << "Inializing PCM Device" << std::endl;
		_err = snd_pcm_hw_params(_pcm_dev, _hw_params);
		std::cout << snd_strerror(_err) << std::endl;

	}

	void sine_wave(unsigned int freq){
		for(short int i = 0; i < _len; i++){
			vals[i] = SHRT_MAX * cos(PI*freq / _rate * i);
		}
	}

	void play_sound(unsigned int  freq){
		sine_wave(freq);
		unsigned int len = 100;
		snd_pcm_prepare(_pcm_dev);
		std::cout << "Playing Tone" << std::endl;
		_err = snd_pcm_writei(_pcm_dev, vals, len); 
		std::cout << snd_strerror(_err) << std::endl;
	}	

	int _err;
	int _rate = 48000;
	unsigned long int _buf_size=  8192*4;
	unsigned int _len = _buf_size*100;
	snd_pcm_hw_params_t *_hw_params;
	snd_pcm_t * _pcm_dev;
	signed short int* vals;

};
