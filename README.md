# Project 2501

Project 2501 is an open-source AI assistant, written in C++.

Currently it can tell you the time, the weather, or any information it knows about from having read basically the entire internet :)

Project 2501 is a fun project to experiment with a partially-offline AI assistant.   There's lots of stuff I want to add to it:

* Store all previous conversations locally, and access them with associative memory.  This way Project 2501 can remember the details of previous conversations.

* Add more internet capabilities - being able to search the web for information etc.

* Allow time-based thinking, for example being able to set timers, or to remind the user to do something. e.g. "Hey nick, you should take in your washing, it's about to rain outside"

* Try one of the offline/local large-language models.

## How it works

I use Windows Speech API (SAPI) for Wake-word detection, as SAPI uses very little CPU, unlike Whisper.cpp.

Once the wake-word/phrase is detected ("Hey Twenty"), I use Whisper.cpp (https://github.com/ggerganov/whisper.cpp) for text-to-speech.  This works pretty well but is a bit slow.  This should be faster when some mulithreading issues in Whisper.cpp are fixed (see https://github.com/ggerganov/whisper.cpp/issues/200#issuecomment-1484025515)

Once the voice query has been converted to text, it's sent off to OpenAI's chat API (similar to ChatGPT).

The response from the chat API is then spoken with Windows speech API speech-to-text system.

Project 2501 is written in C++, and currently works only on Windows (due to use of Windows Speech API).

## Demo video

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/dVGtIcNIZr8/0.jpg)](https://www.youtube.com/embed/dVGtIcNIZr8)

## Building

### Prerequisites

Windows and Visual Studio 2022.  
"C++ ATL for latest xx build tools" should be selected/installed in the Visual Studio Installer.

You will need Ruby installed (for build_libressl.rb script)

For running the program, you will need a microphone that your computer can detect.

### Getting Glare-Core

Check out https://github.com/glaretechnologies/glare-core to a directory

### Build LibreSSL

Define the GLARE_CORE_LIBS environment variable, to something like c:\programming.
This is the directory the LibreSSL directory will be made in.

Build LibreSSL by changing into the scripts directory in glare-core (whereever you checked it out), then running build_libressl.rb:

```
 cd glare-core/trunk 
 ruby build_libressl.rb
```
This should build files into e.g. C:\programming\LibreSSL\libressl-3.5.2-x64-vs2022-install-debug and 
C:\programming\LibreSSL\libressl-3.5.2-x64-vs2022-install

### Building SDL

Get the latest Simple DirectMedia Layer (SDL) code from https://www.libsdl.org/ and build it.

### Run CMake

Make a build directory, and run cmake, using the locations where you checked out the project-2501 and 
glare-core code and where you built SDL.

```
mkdir project_2509_build_dir
cd project_2509_build_dir
cmake N:\project-2501\trunk\ -DGLARE_CORE_TRUNK=n:/glare-core/trunk -DSDL_BUILD_DIR=C:/programming/SDL/sdl_build_vs2022
```


Now open aibot.sln and build it.

## Getting Data files


### OpenAI API key

You will need an OpenAI API key, which is free and comes with some free credit.
Place in e.g. project_2509_build_dir\openai_API_key.txt.  
The program will complain if it can't find it and tell you where to put it.

### whisper.cpp weights

This file contains the parameters for the Whisper neural network.

Download ggml-base.en.bin from https://huggingface.co/ggerganov/whisper.cpp/tree/main and place in project_2509_build_dir.
