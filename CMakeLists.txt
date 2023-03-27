cmake_minimum_required(VERSION 3.0)

project(aibot)


add_definitions(-DUSING_LIBRESSL=1)
add_definitions(-DBUILD_TESTS=1)

if(WIN32)
	set(AIBOT_VS_VER 2022)
		
	add_definitions(/MP) # Enable multi-processor compilation.
	add_definitions(/W4)
	add_definitions(-DUNICODE -D_UNICODE)
	add_definitions(-D__SSE3__ -D__SSSE3__ -D__SSE4_1__)
	
	add_definitions(-D__WINDOWS_MM__=1)
else()
	
endif()



SET(GLARE_CORE_TRUNK $ENV{GLARE_CORE_TRUNK_DIR})

# Directory where LibreSSL and SDL are built
SET(LIBS_DIR "c:/programming")

set(SDL_DIR       "${LIBS_DIR}/SDL/SDL-release-2.26.2" CACHE FILEPATH "SDL source directory")
set(SDL_BUILD_DIR "${LIBS_DIR}/SDL/sdl_build_vs2022"   CACHE FILEPATH "SDL build directory")


set(ilmbasedir	"${GLARE_CORE_TRUNK}/IlmBase")
set(imathdir	"${GLARE_CORE_TRUNK}/Imath") # OpenEXR's math library
set(openexrdir	"${GLARE_CORE_TRUNK}/OpenEXR")
set(zlibdir		"${GLARE_CORE_TRUNK}/zlib")
set(pugixmldir	"${GLARE_CORE_TRUNK}/pugixml/src")
set(pngdir		"${GLARE_CORE_TRUNK}/libpng-1.6.37")


SET(LIBRESSL_VERSION 3.5.2)
if(WIN32)
	set(libressldir "${LIBS_DIR}/LibreSSL/libressl-${LIBRESSL_VERSION}-x64-vs${AIBOT_VS_VER}-install")
endif()


include_directories("./")
include_directories(${SDL_DIR}/include)
include_directories(${GLARE_CORE_TRUNK})
include_directories(${GLARE_CORE_TRUNK}/utils)

set(pugixml
${pugixmldir}/pugixml.cpp
${pugixmldir}/pugixml.hpp
${pugixmldir}/pugiconfig.hpp
)

set(utils
${GLARE_CORE_TRUNK}/utils/Clock.cpp
${GLARE_CORE_TRUNK}/utils/Clock.h
${GLARE_CORE_TRUNK}/utils/ConPrint.cpp
${GLARE_CORE_TRUNK}/utils/ConPrint.h
${GLARE_CORE_TRUNK}/utils/StringUtils.cpp
${GLARE_CORE_TRUNK}/utils/StringUtils.h
${GLARE_CORE_TRUNK}/utils/Mutex.cpp
${GLARE_CORE_TRUNK}/utils/Mutex.h
${GLARE_CORE_TRUNK}/utils/Lock.cpp
${GLARE_CORE_TRUNK}/utils/Lock.h
${GLARE_CORE_TRUNK}/utils/StandardPrintOutput.cpp
${GLARE_CORE_TRUNK}/utils/StandardPrintOutput.h
${GLARE_CORE_TRUNK}/utils/FileUtils.cpp
${GLARE_CORE_TRUNK}/utils/FileUtils.h
${GLARE_CORE_TRUNK}/utils/MemMappedFile.cpp
${GLARE_CORE_TRUNK}/utils/MemMappedFile.h
${GLARE_CORE_TRUNK}/utils/PlatformUtils.cpp
${GLARE_CORE_TRUNK}/utils/PlatformUtils.h
${GLARE_CORE_TRUNK}/utils/TaskManager.cpp
${GLARE_CORE_TRUNK}/utils/TaskManager.h
${GLARE_CORE_TRUNK}/utils/Task.cpp
${GLARE_CORE_TRUNK}/utils/Task.h
${GLARE_CORE_TRUNK}/utils/Condition.cpp
${GLARE_CORE_TRUNK}/utils/Condition.h
${GLARE_CORE_TRUNK}/utils/MyThread.cpp
${GLARE_CORE_TRUNK}/utils/MyThread.h
${GLARE_CORE_TRUNK}/utils/TaskRunnerThread.cpp
${GLARE_CORE_TRUNK}/utils/TaskRunnerThread.h
${GLARE_CORE_TRUNK}/utils/Parser.cpp
${GLARE_CORE_TRUNK}/utils/Parser.h
${GLARE_CORE_TRUNK}/utils/Timer.cpp
${GLARE_CORE_TRUNK}/utils/Timer.h
${GLARE_CORE_TRUNK}/utils/MemAlloc.cpp
${GLARE_CORE_TRUNK}/utils/MemAlloc.h
${GLARE_CORE_TRUNK}/utils/FileInStream.cpp
${GLARE_CORE_TRUNK}/utils/FileInStream.h
${GLARE_CORE_TRUNK}/utils/FileOutStream.cpp
${GLARE_CORE_TRUNK}/utils/FileOutStream.h
${GLARE_CORE_TRUNK}/utils/OutStream.cpp
${GLARE_CORE_TRUNK}/utils/OutStream.h
${GLARE_CORE_TRUNK}/utils/InStream.cpp
${GLARE_CORE_TRUNK}/utils/InStream.h
${GLARE_CORE_TRUNK}/utils/BufferInStream.cpp
${GLARE_CORE_TRUNK}/utils/BufferInStream.h
${GLARE_CORE_TRUNK}/utils/BufferOutStream.cpp
${GLARE_CORE_TRUNK}/utils/BufferOutStream.h
${GLARE_CORE_TRUNK}/utils/TestUtils.cpp
${GLARE_CORE_TRUNK}/utils/TestUtils.h
${GLARE_CORE_TRUNK}/utils/JSONParser.cpp
${GLARE_CORE_TRUNK}/utils/JSONParser.h
${GLARE_CORE_TRUNK}/utils/UTF8Utils.cpp
${GLARE_CORE_TRUNK}/utils/UTF8Utils.h
)

set(maths
${GLARE_CORE_TRUNK}/maths/mathstypes.h
)

set(networking
${GLARE_CORE_TRUNK}/networking/Networking.cpp
${GLARE_CORE_TRUNK}/networking/Networking.h
${GLARE_CORE_TRUNK}/networking/IPAddress.cpp
${GLARE_CORE_TRUNK}/networking/IPAddress.h
${GLARE_CORE_TRUNK}/networking/HTTPClient.cpp
${GLARE_CORE_TRUNK}/networking/HTTPClient.h
${GLARE_CORE_TRUNK}/networking/TLSSocket.cpp
${GLARE_CORE_TRUNK}/networking/TLSSocket.h
${GLARE_CORE_TRUNK}/networking/MySocket.cpp
${GLARE_CORE_TRUNK}/networking/MySocket.h
${GLARE_CORE_TRUNK}/networking/SocketInterface.cpp
${GLARE_CORE_TRUNK}/networking/SocketInterface.h
${GLARE_CORE_TRUNK}/networking/URL.cpp
${GLARE_CORE_TRUNK}/networking/URL.h
)

set(double-conversion
${GLARE_CORE_TRUNK}/double-conversion/bignum-dtoa.cc
${GLARE_CORE_TRUNK}/double-conversion/bignum-dtoa.h
${GLARE_CORE_TRUNK}/double-conversion/bignum.cc
${GLARE_CORE_TRUNK}/double-conversion/bignum.h
${GLARE_CORE_TRUNK}/double-conversion/cached-powers.cc
${GLARE_CORE_TRUNK}/double-conversion/cached-powers.h
${GLARE_CORE_TRUNK}/double-conversion/double-to-string.cc
${GLARE_CORE_TRUNK}/double-conversion/string-to-double.cc
${GLARE_CORE_TRUNK}/double-conversion/diy-fp.h
${GLARE_CORE_TRUNK}/double-conversion/double-conversion.h
${GLARE_CORE_TRUNK}/double-conversion/fast-dtoa.cc
${GLARE_CORE_TRUNK}/double-conversion/fast-dtoa.h
${GLARE_CORE_TRUNK}/double-conversion/fixed-dtoa.cc
${GLARE_CORE_TRUNK}/double-conversion/fixed-dtoa.h
${GLARE_CORE_TRUNK}/double-conversion/ieee.h
${GLARE_CORE_TRUNK}/double-conversion/strtod.cc
${GLARE_CORE_TRUNK}/double-conversion/strtod.h
${GLARE_CORE_TRUNK}/double-conversion/utils.h
)

set(webserver
${GLARE_CORE_TRUNK}/webserver/Escaping.cpp
${GLARE_CORE_TRUNK}/webserver/Escaping.h
)

set(whisper
./whisper.cpp/whisper.cpp
./whisper.cpp/whisper.h
./whisper.cpp/ggml.c
./whisper.cpp/ggml.h
)

set(llama
./llama.cpp/llama.cpp
./llama.cpp/llama.h
#./llama.cpp/ggml.c
#./llama.cpp/ggml.h
)

set(aibot
AIBot.cpp
Weather.cpp
Weather.h
notes.txt
)

SOURCE_GROUP(utils FILES ${utils})
SOURCE_GROUP(maths FILES ${maths})
SOURCE_GROUP(double-conversion FILES ${double-conversion})
SOURCE_GROUP(networking FILES ${networking})
SOURCE_GROUP(pugixml FILES ${pugixml})
SOURCE_GROUP(whisper FILES ${whisper})
SOURCE_GROUP(webserver FILES ${webserver})
SOURCE_GROUP(llama FILES ${llama})
SOURCE_GROUP(aibot FILES ${aibot})

add_executable(aibot
${aibot}
${utils}
${maths}
${double-conversion}
${pugixml}
${networking}
${whisper}
${webserver}
#${llama}
)


set(CURRENT_TARGET aibot)
include("cmake/libressl.cmake")


target_link_libraries(aibot
debug "${SDL_BUILD_DIR}/Debug/SDL2d.lib" 
debug "${SDL_BUILD_DIR}/Debug/SDL2maind.lib" 
optimized "${SDL_BUILD_DIR}/RelWithDebInfo/SDL2.lib" 
optimized "${SDL_BUILD_DIR}/RelWithDebInfo/SDL2main.lib" 
odbc32.lib
odbccp32.lib
winmm.lib
ws2_32 # Winsock
)
