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
