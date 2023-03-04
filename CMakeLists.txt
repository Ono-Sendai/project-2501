cmake_minimum_required(VERSION 3.0)

project(aibot)

add_definitions(/MP) # Enable multi-processor compilation.
add_definitions(/W4)
add_definitions(-DUNICODE -D_UNICODE)
add_definitions(-D__SSE3__ -D__SSSE3__ -D__SSE4_1__)

add_definitions(-DPNG_ALLOW_BENIGN_ERRORS=1)
add_definitions(-DPNG_INTEL_SSE=1)
add_definitions(-DPNG_NO_SETJMP=1)
add_definitions(-D__WINDOWS_MM__=1)
add_definitions(-DUSING_LIBRESSL=1)

add_definitions(-DBUILD_TESTS=1)


if(WIN32)
	set(VJ_VS_VER 2022)
	
	set(CYBERSPACE_BITNESS 64)
	set(CYBERSPACE_PLATFORM "x64")
	
	
else()
	#linux stuff goes here
endif()



SET(GLARE_CORE_TRUNK $ENV{GLARE_CORE_TRUNK_DIR})
SET(INDIGO_TRUNK $ENV{INDIGO_TRUNK_DIR})
SET(INDIGO_LIBS_ENV $ENV{INDIGO_LIBS})

set(SDL_DIR "$ENV{INDIGO_LIBS}/SDL/SDL-release-2.26.2" CACHE FILEPATH "SDL source directory")
set(SDL_BUILD_DIR "$ENV{INDIGO_LIBS}/SDL/sdl_build_vs2022" CACHE FILEPATH "SDL build directory")

add_definitions(-DBASE_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
add_definitions(-DINDIGO_TRUNK="${INDIGO_TRUNK}")

set(jpegdir		"${INDIGO_LIBS_ENV}/libjpeg-turbo/libjpeg-turbo-vs2022-install")


# These libraries are checked into the Indigo repo to make setting up builds easier.
set(ilmbasedir	"${GLARE_CORE_TRUNK}/IlmBase")
set(imathdir	"${GLARE_CORE_TRUNK}/Imath") # OpenEXR's math library
set(openexrdir	"${GLARE_CORE_TRUNK}/OpenEXR")
set(zlibdir		"${GLARE_CORE_TRUNK}/zlib")
set(pugixmldir	"${GLARE_CORE_TRUNK}/pugixml/src")
set(pngdir		"${GLARE_CORE_TRUNK}/libpng-1.6.37")
set(imguidir 	"${INDIGO_LIBS_ENV}/imgui")

set(LIBJPEG_TURBO_DIR "${INDIGO_LIBS_ENV}/libjpeg-turbo-builds")

SET(INDIGO_LIBRESSL_VERSION 3.5.2)

if(WIN32)
	set(libressldir "${INDIGO_LIBS_ENV}/LibreSSL/libressl-${INDIGO_LIBRESSL_VERSION}-${CYBERSPACE_PLATFORM}-vs${VJ_VS_VER}-install")
	set(LIBJPEG_TURBO_DIR "${LIBJPEG_TURBO_DIR}/vs_${VJ_VS_VER}_64") # Append dir suffix for VS version and bitness.
endif()


include_directories("./")
include_directories(${SDL_DIR}/include)
include_directories(${INDIGO_TRUNK})
include_directories(${GLARE_CORE_TRUNK})
include_directories(${GLARE_CORE_TRUNK}/utils)
include_directories(${GLARE_CORE_TRUNK}/opengl)
include_directories(${GLARE_CORE_TRUNK}/zstd-1.5.2/lib)
include_directories(${GLARE_CORE_TRUNK}/zstd-1.5.2/lib/common) # for xxhash
include_directories(${GLARE_CORE_TRUNK}/little_cms/include)
include_directories(${GLARE_CORE_TRUNK}/giflib/lib)

include_directories(${jpegdir}/include)  # libjpeg-turbo-master (jpegdir) has most of the source,
include_directories(${LIBJPEG_TURBO_DIR})  # LIBJPEG_TURBO_DIR has jconfig.h
include_directories(${pngdir})
include_directories(${zlibdir})
include_directories(${imguidir})


include_directories("kissfft")


set (imgui
${imguidir}/imgui.cpp
${imguidir}/imgui.h
${imguidir}/imgui_demo.cpp
${imguidir}/imgui_draw.cpp
${imguidir}/imgui_internal.h
${imguidir}/imgui_widgets.cpp
${imguidir}/imgui_tables.cpp
${imguidir}/imstb_rectpack.h
${imguidir}/imstb_textedit.h
${imguidir}/imstb_truetype.h
${imguidir}/backends/imgui_impl_sdl.cpp
${imguidir}/backends/imgui_impl_sdl.h
${imguidir}/backends/imgui_impl_opengl3.cpp
${imguidir}/backends/imgui_impl_opengl3.h
)


############# Stuff from indigo_libs/CMakeLists.txt ##################


set(libpng
${pngdir}/png.c ${pngdir}/pngerror.c ${pngdir}/pngget.c ${pngdir}/pngmem.c ${pngdir}/pngpread.c ${pngdir}/pngread.c ${pngdir}/pngrio.c ${pngdir}/pngrtran.c
${pngdir}/pngrutil.c ${pngdir}/pngset.c ${pngdir}/pngtrans.c ${pngdir}/pngwio.c ${pngdir}/pngwrite.c ${pngdir}/pngwtran.c ${pngdir}/pngwutil.c
${pngdir}/png.h
${pngdir}/pngconf.h
${pngdir}/pngdebug.h
${pngdir}/pnginfo.h
${pngdir}/pnglibconf.h
${pngdir}/pngpriv.h
${pngdir}/pngstruct.h

${pngdir}/intel/intel_init.c
${pngdir}/intel/filter_sse2_intrinsics.c
)


set(zlib
${zlibdir}/adler32.c
${zlibdir}/compress.c
${zlibdir}/crc32.c
${zlibdir}/deflate.c
${zlibdir}/inffast.c
${zlibdir}/inflate.c
${zlibdir}/inftrees.c
${zlibdir}/trees.c
${zlibdir}/uncompr.c
${zlibdir}/zutil.c
${zlibdir}/crc32.h
${zlibdir}/deflate.h
${zlibdir}/inffast.h
${zlibdir}/inffixed.h
${zlibdir}/inflate.h
${zlibdir}/inftrees.h
${zlibdir}/trees.h
${zlibdir}/zconf.h
${zlibdir}/zlib.h
${zlibdir}/zutil.h
)

set(pugixml
${pugixmldir}/pugixml.cpp
${pugixmldir}/pugixml.hpp
${pugixmldir}/pugiconfig.hpp
)

# MESSAGE(${pugixml})

FILE(GLOB giflib "${INDIGO_TRUNK}/giflib/lib/*.c" "${INDIGO_TRUNK}/giflib/lib/*.cpp" "${INDIGO_TRUNK}/giflib/lib/*.h")
FILE(GLOB little_cms "${INDIGO_TRUNK}/little_cms/src/*.c"  "${INDIGO_TRUNK}/little_cms/include/*.h")
FILE(GLOB zstandard 
"${INDIGO_TRUNK}/zstd-1.4.4/lib/*.h"  
"${INDIGO_TRUNK}/zstd-1.4.4/lib/common/*.c" 
"${INDIGO_TRUNK}/zstd-1.4.4/lib/common/*.h"
"${INDIGO_TRUNK}/zstd-1.4.4/lib/compress/*.c" 
"${INDIGO_TRUNK}/zstd-1.4.4/lib/compress/*.h"
"${INDIGO_TRUNK}/zstd-1.4.4/lib/decompress/*.c" 
"${INDIGO_TRUNK}/zstd-1.4.4/lib/decompress/*.h"
)


SOURCE_GROUP(libpng FILES ${libpng})
SOURCE_GROUP(zlib FILES ${zlib})
SOURCE_GROUP(zlib\\minizip FILES ${minizip})
SOURCE_GROUP(pugixml FILES ${pugixml})
SOURCE_GROUP(giflib FILES ${giflib})
SOURCE_GROUP(little_cms FILES ${little_cms})
SOURCE_GROUP(zstandard FILES ${zstandard})


set(indigo
${GLARE_CORE_TRUNK}/indigo/TextureServer.cpp
${GLARE_CORE_TRUNK}/indigo/TextureServer.h

${GLARE_CORE_TRUNK}/dll/IndigoMesh.cpp
${GLARE_CORE_TRUNK}/dll/include/IndigoMesh.h
${GLARE_CORE_TRUNK}/dll/IndigoAllocation.cpp
${GLARE_CORE_TRUNK}/dll/include/IndigoAllocation.h
)

set(graphics
${GLARE_CORE_TRUNK}/graphics/BatchedMesh.cpp
${GLARE_CORE_TRUNK}/graphics/BatchedMesh.h
${GLARE_CORE_TRUNK}/graphics/bitmap.cpp
${GLARE_CORE_TRUNK}/graphics/bitmap.h
${GLARE_CORE_TRUNK}/graphics/Image.cpp
${GLARE_CORE_TRUNK}/graphics/Image.h
${GLARE_CORE_TRUNK}/graphics/Map2D.cpp
${GLARE_CORE_TRUNK}/graphics/Map2D.h
${GLARE_CORE_TRUNK}/graphics/ImageMap.cpp
${GLARE_CORE_TRUNK}/graphics/ImageMap.h
${GLARE_CORE_TRUNK}/graphics/imformatdecoder.cpp
${GLARE_CORE_TRUNK}/graphics/imformatdecoder.h
${GLARE_CORE_TRUNK}/graphics/jpegdecoder.cpp
${GLARE_CORE_TRUNK}/graphics/jpegdecoder.h
${GLARE_CORE_TRUNK}/graphics/PNGDecoder.cpp
${GLARE_CORE_TRUNK}/graphics/PNGDecoder.h
#${GLARE_CORE_TRUNK}/graphics/RGBEDecoder.cpp
#${GLARE_CORE_TRUNK}/graphics/RGBEDecoder.h
${GLARE_CORE_TRUNK}/graphics/TIFFDecoder.cpp
${GLARE_CORE_TRUNK}/graphics/TIFFDecoder.h
${GLARE_CORE_TRUNK}/graphics/tgadecoder.cpp
${GLARE_CORE_TRUNK}/graphics/tgadecoder.h
${GLARE_CORE_TRUNK}/graphics/bmpdecoder.cpp
${GLARE_CORE_TRUNK}/graphics/bmpdecoder.h
${GLARE_CORE_TRUNK}/graphics/EXRDecoder.cpp
${GLARE_CORE_TRUNK}/graphics/EXRDecoder.h
${GLARE_CORE_TRUNK}/graphics/FloatDecoder.cpp
${GLARE_CORE_TRUNK}/graphics/FloatDecoder.h
${GLARE_CORE_TRUNK}/graphics/GifDecoder.cpp
${GLARE_CORE_TRUNK}/graphics/GifDecoder.h
${GLARE_CORE_TRUNK}/graphics/DXTCompression.cpp
${GLARE_CORE_TRUNK}/graphics/DXTCompression.h
${GLARE_CORE_TRUNK}/graphics/KTXDecoder.cpp
${GLARE_CORE_TRUNK}/graphics/KTXDecoder.h
${GLARE_CORE_TRUNK}/graphics/CompressedImage.cpp
${GLARE_CORE_TRUNK}/graphics/CompressedImage.h

${GLARE_CORE_TRUNK}/graphics/Image4f.cpp
${GLARE_CORE_TRUNK}/graphics/Image4f.h
${GLARE_CORE_TRUNK}/graphics/BoxFilterFunction.cpp
${GLARE_CORE_TRUNK}/graphics/BoxFilterFunction.h
${GLARE_CORE_TRUNK}/graphics/FilterFunction.cpp
${GLARE_CORE_TRUNK}/graphics/FilterFunction.h
)

set(opengl
${GLARE_CORE_TRUNK}/opengl/FrameBuffer.cpp
${GLARE_CORE_TRUNK}/opengl/FrameBuffer.h
${GLARE_CORE_TRUNK}/opengl/gl3w.c
${GLARE_CORE_TRUNK}/opengl/IncludeOpenGL.h
${GLARE_CORE_TRUNK}/opengl/OpenGLEngine.cpp
${GLARE_CORE_TRUNK}/opengl/OpenGLEngine.h
${GLARE_CORE_TRUNK}/opengl/OpenGLEngineTests.cpp
${GLARE_CORE_TRUNK}/opengl/OpenGLEngineTests.h
${GLARE_CORE_TRUNK}/opengl/OpenGLProgram.cpp
${GLARE_CORE_TRUNK}/opengl/OpenGLProgram.h
${GLARE_CORE_TRUNK}/opengl/OpenGLShader.cpp
${GLARE_CORE_TRUNK}/opengl/OpenGLShader.h
${GLARE_CORE_TRUNK}/opengl/OpenGLTexture.cpp
${GLARE_CORE_TRUNK}/opengl/OpenGLTexture.h
${GLARE_CORE_TRUNK}/opengl/ShadowMapping.cpp
${GLARE_CORE_TRUNK}/opengl/ShadowMapping.h
${GLARE_CORE_TRUNK}/opengl/TextureLoading.cpp
${GLARE_CORE_TRUNK}/opengl/TextureLoading.h
${GLARE_CORE_TRUNK}/opengl/TextureLoadingTests.cpp
${GLARE_CORE_TRUNK}/opengl/TextureLoadingTests.h
${GLARE_CORE_TRUNK}/opengl/VAO.cpp
${GLARE_CORE_TRUNK}/opengl/VAO.h
${GLARE_CORE_TRUNK}/opengl/VBO.cpp
${GLARE_CORE_TRUNK}/opengl/VBO.h

${GLARE_CORE_TRUNK}/opengl/shaders/depth_frag_shader.glsl
${GLARE_CORE_TRUNK}/opengl/shaders/depth_vert_shader.glsl
${GLARE_CORE_TRUNK}/opengl/shaders/edge_extract_frag_shader.glsl
${GLARE_CORE_TRUNK}/opengl/shaders/edge_extract_vert_shader.glsl
${GLARE_CORE_TRUNK}/opengl/shaders/env_frag_shader.glsl
${GLARE_CORE_TRUNK}/opengl/shaders/env_vert_shader.glsl
${GLARE_CORE_TRUNK}/opengl/shaders/outline_frag_shader.glsl
${GLARE_CORE_TRUNK}/opengl/shaders/outline_vert_shader.glsl
${GLARE_CORE_TRUNK}/opengl/shaders/overlay_frag_shader.glsl
${GLARE_CORE_TRUNK}/opengl/shaders/overlay_vert_shader.glsl
${GLARE_CORE_TRUNK}/opengl/shaders/phong_frag_shader.glsl
${GLARE_CORE_TRUNK}/opengl/shaders/phong_vert_shader.glsl
${GLARE_CORE_TRUNK}/opengl/shaders/transparent_frag_shader.glsl
${GLARE_CORE_TRUNK}/opengl/shaders/transparent_vert_shader.glsl
)

set(utils
${GLARE_CORE_TRUNK}/utils/Clock.cpp
${GLARE_CORE_TRUNK}/utils/Clock.h
${GLARE_CORE_TRUNK}/utils/CycleTimer.cpp
${GLARE_CORE_TRUNK}/utils/CycleTimer.h
${GLARE_CORE_TRUNK}/utils/ConPrint.cpp
${GLARE_CORE_TRUNK}/utils/ConPrint.h
${GLARE_CORE_TRUNK}/utils/StringUtils.cpp
${GLARE_CORE_TRUNK}/utils/StringUtils.h
${GLARE_CORE_TRUNK}/utils/Mutex.cpp
${GLARE_CORE_TRUNK}/utils/Mutex.h
${GLARE_CORE_TRUNK}/utils/Lock.cpp
${GLARE_CORE_TRUNK}/utils/Lock.h
${GLARE_CORE_TRUNK}/utils/DynamicLib.cpp
${GLARE_CORE_TRUNK}/utils/DynamicLib.h
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
${GLARE_CORE_TRUNK}/utils/FileHandle.cpp
${GLARE_CORE_TRUNK}/utils/FileHandle.h
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
${GLARE_CORE_TRUNK}/maths/SSE.cpp
${GLARE_CORE_TRUNK}/maths/SSE.h
${GLARE_CORE_TRUNK}/maths/Vec4f.cpp
${GLARE_CORE_TRUNK}/maths/Vec4f.h
${GLARE_CORE_TRUNK}/maths/Matrix4f.cpp
${GLARE_CORE_TRUNK}/maths/Matrix4f.h
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
#${GLARE_CORE_TRUNK}/double-conversion/diy-fp.cc
${GLARE_CORE_TRUNK}/double-conversion/diy-fp.h
#${GLARE_CORE_TRUNK}/double-conversion/double-conversion.cc
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


set(video
${GLARE_CORE_TRUNK}/video/VideoReader.cpp
${GLARE_CORE_TRUNK}/video/VideoReader.h
${GLARE_CORE_TRUNK}/video/WMFVideoReader.cpp
${GLARE_CORE_TRUNK}/video/WMFVideoReader.h
)

set(rtmidi
./rtmidi-3.0.0/RtMidi.cpp
./rtmidi-3.0.0/RtMidi.h
)

set(kissfft
./kissfft/kiss_fft.c
./kissfft/kiss_fft.h
./kissfft/tools/kiss_fftr.c
./kissfft/tools/kiss_fftr.h
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


SOURCE_GROUP(rtmidi FILES ${rtmidi})
SOURCE_GROUP(indigo FILES ${indigo})
SOURCE_GROUP(graphics FILES ${graphics})
SOURCE_GROUP(opengl FILES ${opengl})
SOURCE_GROUP(utils FILES ${utils})
SOURCE_GROUP(maths FILES ${maths})
SOURCE_GROUP(double-conversion FILES ${double-conversion})
SOURCE_GROUP(video FILES ${video})
SOURCE_GROUP(kissfft FILES ${kissfft})
SOURCE_GROUP(imgui FILES ${imgui})
SOURCE_GROUP(networking FILES ${networking})
SOURCE_GROUP(whisper FILES ${whisper})
SOURCE_GROUP(webserver FILES ${webserver})

add_executable(aibot
AIBot.cpp
notes.txt
${utils}
${maths}
${double-conversion}
${zlib}
${pugixml}
${zstandard}
${networking}
${whisper}
${webserver}
)


if(WIN32)
	SET(TURBOJPEG_LIB ${LIBJPEG_TURBO_DIR}/$(Configuration)/turbojpeg-static.lib)
else()
	SET(TURBOJPEG_LIB ${LIBJPEG_TURBO_DIR}/libjpeg.a)
endif()


set(CURRENT_TARGET aibot)
include("cmake/libressl.cmake")



target_link_libraries(aibot
debug "${SDL_BUILD_DIR}/Debug/SDL2d.lib" 
debug "${SDL_BUILD_DIR}/Debug/SDL2maind.lib" 
optimized "${SDL_BUILD_DIR}/RelWithDebInfo/SDL2.lib" 
optimized "${SDL_BUILD_DIR}/RelWithDebInfo/SDL2main.lib" 
#Opengl32.lib
odbc32.lib
odbccp32.lib
winmm.lib
#debug "${INDIGO_LIBS_ENV}/libjpeg-turbo/libjpeg-turbo-vs2019-install-debug/lib/turbojpeg-static.lib" 
#optimized "${INDIGO_LIBS_ENV}/libjpeg-turbo/libjpeg-turbo-vs2019-install/lib/turbojpeg-static.lib" 
#mfplat.lib 
#mfreadwrite.lib 
#mfuuid.lib

#Mf.lib
#d3d11.lib
ws2_32 # Winsock
)










