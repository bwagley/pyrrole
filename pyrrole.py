#!/bin/python

#Pyrrole, a basic audio synthesis tool
#Benjamin Wagley - For HASS327 Synth Project
#Designed for use in conjunction with Echo Circuit designed by Elijah Mt. Castle
#Requires pyaudio for audio output, pygame for midi input.

import pyaudio
import numpy as np
import sys
import pygame, pygame.midi
import os, signal

#Set initilization parameters for pyaudio
CHUNK = 1024
FORMAT = pyaudio.paInt16
CHANNELS = 2
RATE = 44100
#AMP = 2*127.5
AMP = 2**12
#AMP = 2**16

argc = len(sys.argv)
argv = sys.argv

#Create a pyaudio output stream
p = pyaudio.PyAudio()
stream = p.open(format=FORMAT, channels=CHANNELS, rate=RATE, output=True)

#Define our waveform functions, using numpy trig functions.
def sineWave(freq, duration):
    t = np.linspace(0,duration, int(duration*RATE))
    data = np.sin(2*np.pi*freq*t)*AMP
    return data.astype(np.int16)

def triWave(freq, duration):
    t = np.linspace(0, duration, int(duration*RATE))
    data = 2*AMP / np.pi*np.arcsin(np.sin(2*np.pi*freq*t))
    return data.astype(np.int16)

def sawWave(freq, duration):
    t = np.linspace(0, duration, int(duration*RATE))
    data = -2*AMP / np.pi*np.arctan(1/np.tan(np.pi*freq*(t - np.floor(t))))
    return data.astype(np.int16)

def squareWave(freq, duration):
    data = sineWave(freq, duration)
    for i in range(len(data)):
        #We "fake" a square wave by using a given sine wave input
        if data[i] < 0:
            data[i] = -AMP
        else:
            data[i] = AMP

    return data

#Create a wave of using a wave function, or a given frequency and duration. 
def playWave(freq, duration, func):
    period = 1 / freq
    n = duration / period
    duration = np.floor(n)* period
    return func(freq, duration)


#Play a wave of a given shape (using above playWave function to build wave)
def playTone(freq, shape):
    wave = playWave(freq, 1, shape)
    stream.write(wave)

def main():
    #We have an array of wave shapes so that we can switch through them
    waveShapes = [sineWave, squareWave, triWave, sawWave]
    shape = 0
    if argc == 1:
        print("No Wave Shape Given, Setting to Sine Wave")
    else:
        print(argv[1])
        if argv[1] == "sine":
            shape = 0
        elif argv[1] == "square":
            shape = 1
        elif argv[1] == "triangle":
            shape = 2
        elif argv[1] == "saw":
            shape = 3
        else:
            print("Invalid Wave Shape Given, please select sine, square, triangle, or saw.\nDefaulting to sine.")

    #Init pygame for midi input
    pygame.init()
    pygame.midi.init()

    inp = pygame.midi.Input(3)

    midiKey = []

    #Populate an array of frequency for 128 midi keys.
    for i in range(127):
        midiKey.append((440 / 32) * (2 ** ((i - 9) / 12)))

    while True:
        if inp.poll():
            mlist = inp.read(1000)
            for i in range(len(mlist)):
                #If non-key midi input, change the wave shape
                if mlist[i][0][0] == 176:
                    shape = (shape + 1) % 4
                    print(waveShapes[shape])
                    continue
                #Otherwise, get the frequenct to play
                toPlay = midiKey[mlist[i][0][1]]
                #Make sure this is a midi keypress, not release
                if mlist[i][0][2] != 0:
                    playTone(toPlay, waveShapes[shape])
        pygame.time.wait(10)

main()
