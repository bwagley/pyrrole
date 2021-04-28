import pyaudio
import numpy as np
import sys
import pygame, pygame.midi
import os, signal

CHUNK = 1024
FORMAT = pyaudio.paInt16
CHANNELS = 2
RATE = 44100

p = pyaudio.PyAudio()
stream = p.open(format=FORMAT, channels=CHANNELS, rate=RATE, output=True)

def sineWave(freq, duration):
    period = 1 / freq
    n = duration / period
    duration = np.floor(n)* period
    t = np.linspace(0,duration, int(duration*RATE))
    data = np.sin(2*np.pi*freq*t)*127.5
    return data.astype(np.int16)

def playTone(freq):
    
    wave = sineWave(freq, 1)
    stream.write(wave)

def main():
    pygame.init()
    pygame.midi.init()

    inp = pygame.midi.Input(3)

    midiKey = []
    childKey = {}

    for i in range(127):
        midiKey.append((440 / 32) * (2 ** ((i - 9) / 12)))
        childKey[midiKey[i]] = 0;

    while True:
        if inp.poll():
            mlist = inp.read(1000)
            for i in range(len(mlist)):
                toPlay = midiKey[mlist[i][0][1]]
                #if mlist[i][0][2] == 0:
                ##   os.kill(childKey[toPlay], signal.SIGTERM)
                #    childKey[toPlay] = 0
                #    continue
                if mlist[i][0][2] != 0:
                    playTone(toPlay)
        pygame.time.wait(10)

main()
