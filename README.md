# Fresh Synth of Bel Air

A fun cross-platform synthesizer to play around with. Created for Bart Massey's CS-510 Music, Sound and Computers course at PSU in Spring 2022

## Authors

Peter Wells  
Eliza Sohn

## Features

This is a simple subtractive synthesizer made in the [JUCE](https://juce.com/) framework. It features

* Midi controller support
* Standalone and VST3 support
* 1x Monophonic Selectable Oscillator (sine, saw, square)
* 1x VCA ADSR
* 1x Switchable Ladder filter with cutoff and resonance controls. (Filters include Lowpass, Highpass, and Bandpass each with 12 or 24 db slopes)
* 1x Gain control

## Usage Troubleshooting

If you don't hear any sound coming out of the synth:

* Check the options screen located on the top left of the application window to make sure the correct midi controller and audio driver are selected

## Project Retrospective

This project ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

## Installation Windows / OSX

1. Clone this Repo
2. Download and extract [JUCE](https://juce.com/get-juce)
3. Run the Projucer and `file->open->FreshSynth.jucer` in the repo's root directory
4. You will need to point projucer to the Juce and modules folders. Do this in Projucer from the `file->global paths` menu
5. Select your exporter (supported: Visual Studio 2019, Xcode, Linux Makefile) and press the icon to open in your selected IDE
5. In the IDE build the project and it should open as a standalone application. This should also place files in the appropriate build folders

## Linux Installation Notes

You will need to have a C++ compiler and a series of dependencies in order for this project to build. 
From the [Official Documentation](https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md)
```
sudo apt update
sudo apt install libasound2-dev libjack-jackd2-dev \
    ladspa-sdk \
    libcurl4-openssl-dev  \
    libfreetype6-dev \
    libx11-dev libxcomposite-dev libxcursor-dev libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev \
    libwebkit2gtk-4.0-dev \
    libglu1-mesa-dev mesa-common-dev
```
Instead of exporting the project to an IDE and building there we must use the make file

1. Follow steps 1-4 above
2. Press `file->save` to save the project and generate a makefile
3. With the correct dependencies installed: navigate to `fresh-synth-of-bel-air/Builds/LinuxMakeFile` in the terminal and run `make`. This will build the project
4. Once complete, navigate to `fresh-synth-of-bel-air/Builds/LinuxMakefile/build` and run the `FreshSynth` file to open the synth

## References

* [JUCE: API Documentation](https://docs.juce.com/master/index.html)
* [JUCE: Handling MIDI Events](https://docs.juce.com/master/tutorial_handling_midi_events.html)
* [JUCE: Build a MIDI Synthesizer](https://docs.juce.com/master/tutorial_synth_using_midi_input.html)
* [The Audio Programmer: Building Your First Plug-In (JUCE overview)](https://youtu.be/Bw_OkHNpj1M)
* [The Audio Programmer: Let's Build a Synth with Juce Series](https://youtu.be/nQR-wtzsRhA)
