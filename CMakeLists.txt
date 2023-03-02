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

set(SDL_DIR "$ENV{INDIGO_LIBS}/SDL/SDL2-2.0.14" CACHE FILEPATH "SDL source directory")
set(SDL_BUILD_DIR "$ENV{INDIGO_LIBS}/SDL/SDL2-2.0.14_vs2019_build" CACHE FILEPATH "SDL build directory")

add_definitions(-DBASE_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
add_definitions(-DINDIGO_TRUNK="${INDIGO_TRUNK}")

set(jpegdir		"${INDIGO_LIBS_ENV}/libjpeg-turbo/libjpeg-turbo-vs2022-install")
set(tiffdir		"${GLARE_CORE_TRUNK}/tiff-3.8.2/libtiff")

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
include_directories(${tiffdir})
include_directories(${zlibdir})
include_directories(${imguidir})



#include_directories("${ilmbasedir}/Half")
#include_directories("${ilmbasedir}/Imath")
#include_directories("${ilmbasedir}/IlmThread")
#include_directories("${ilmbasedir}/Iex")
#include_directories("${ilmbasedir}/IexMath")
#include_directories("${ilmbasedir}/IlmThread")
#include_directories("${openexrdir}/IlmImf")
#include_directories("${openexrdir}/IlmImfUtil")
#if(WIN32)
#	include_directories("${ilmbasedir}/config.windows")
#	include_directories("${openexrdir}/config.windows")
#elseif(APPLE)
#	include_directories("${ilmbasedir}/config_mac")
#	include_directories("${openexrdir}/config_mac")
#else()
#	include_directories("${ilmbasedir}/config_linux")
#	include_directories("${openexrdir}/config_linux")
#endif()


# OpenEXR:
include_directories("${imathdir}/src/Imath")
include_directories("${openexrdir}/src/lib/Iex")
include_directories("${openexrdir}/src/lib/IlmThread")
include_directories("${openexrdir}/src/lib/OpenEXR")
include_directories("${openexrdir}/src/lib/OpenEXRUtil")

# OpenEXR per-platform config files:
if(WIN32)
	include_directories("${imathdir}/config_windows")
	include_directories("${openexrdir}/config_windows")
elseif(APPLE)
	include_directories("${imathdir}/config_mac")
	include_directories("${openexrdir}/config_mac")
else()
	include_directories("${imathdir}/config_linux")
	include_directories("${openexrdir}/config_linux")
endif()



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

#set(spoutdir "./SpoutSDK/Source")
#set(spout
#${spoutdir}/SpoutCopy.cpp
#${spoutdir}/Spout.h
#${spoutdir}/SpoutCommon.h
#${spoutdir}/SpoutCopy.cpp
#${spoutdir}/SpoutCopy.h
#${spoutdir}/SpoutDirectX.cpp
#${spoutdir}/SpoutDirectX.h
#${spoutdir}/SpoutGLDXinterop.cpp
#${spoutdir}/SpoutGLDXinterop.h
#${spoutdir}/SpoutGLextensions.cpp
#${spoutdir}/SpoutGLextensions.h
#${spoutdir}/SpoutMemoryShare.cpp
#${spoutdir}/SpoutMemoryShare.h
#${spoutdir}/SpoutReceiver.cpp
#${spoutdir}/SpoutReceiver.h
#${spoutdir}/SpoutSDK.cpp
#${spoutdir}/SpoutSDK.h
#${spoutdir}/SpoutSender.cpp
#${spoutdir}/SpoutSender.h
#${spoutdir}/SpoutSenderMemory.cpp
#${spoutdir}/SpoutSenderMemory.h
#${spoutdir}/SpoutSenderNames.cpp
#${spoutdir}/SpoutSenderNames.h
#${spoutdir}/SpoutSharedMemory.cpp
#${spoutdir}/SpoutSharedMemory.h
#)

############# Stuff from indigo_libs/CMakeLists.txt ##################

# OpenEXR's math lib
set(imath
${imathdir}/src/Imath/half.cpp
${imathdir}/src/Imath/ImathColorAlgo.cpp
${imathdir}/src/Imath/ImathFun.cpp
${imathdir}/src/Imath/ImathMatrixAlgo.cpp
${imathdir}/src/Imath/ImathRandom.cpp
${imathdir}/src/Imath/toFloat.h
${imathdir}/src/Imath/half.h
${imathdir}/src/Imath/halfFunction.h
${imathdir}/src/Imath/halfLimits.h
${imathdir}/src/Imath/ImathBox.h
${imathdir}/src/Imath/ImathBoxAlgo.h
${imathdir}/src/Imath/ImathColor.h
${imathdir}/src/Imath/ImathColorAlgo.h
${imathdir}/src/Imath/ImathEuler.h
${imathdir}/src/Imath/ImathExport.h
${imathdir}/src/Imath/ImathForward.h
${imathdir}/src/Imath/ImathFrame.h
${imathdir}/src/Imath/ImathFrustum.h
${imathdir}/src/Imath/ImathFrustumTest.h
${imathdir}/src/Imath/ImathFun.h
${imathdir}/src/Imath/ImathGL.h
${imathdir}/src/Imath/ImathGLU.h
${imathdir}/src/Imath/ImathInt64.h
${imathdir}/src/Imath/ImathInterval.h
${imathdir}/src/Imath/ImathLine.h
${imathdir}/src/Imath/ImathLineAlgo.h
${imathdir}/src/Imath/ImathMath.h
${imathdir}/src/Imath/ImathMatrix.h
${imathdir}/src/Imath/ImathMatrixAlgo.h
${imathdir}/src/Imath/ImathNamespace.h
${imathdir}/src/Imath/ImathPlane.h
${imathdir}/src/Imath/ImathPlatform.h
${imathdir}/src/Imath/ImathQuat.h
${imathdir}/src/Imath/ImathRandom.h
${imathdir}/src/Imath/ImathRoots.h
${imathdir}/src/Imath/ImathShear.h
${imathdir}/src/Imath/ImathSphere.h
${imathdir}/src/Imath/ImathTypeTraits.h
${imathdir}/src/Imath/ImathVec.h
${imathdir}/src/Imath/ImathVecAlgo.h
)

# OpenEXR exception classes.
SET(iex
${openexrdir}/src/lib/Iex/IexBaseExc.cpp
${openexrdir}/src/lib/Iex/IexMathFloatExc.cpp
${openexrdir}/src/lib/Iex/IexMathFpu.cpp
${openexrdir}/src/lib/Iex/IexThrowErrnoExc.cpp

${openexrdir}/src/lib/Iex/Iex.h
${openexrdir}/src/lib/Iex/IexBaseExc.h
${openexrdir}/src/lib/Iex/IexErrnoExc.h
${openexrdir}/src/lib/Iex/IexExport.h
${openexrdir}/src/lib/Iex/IexForward.h
${openexrdir}/src/lib/Iex/IexMacros.h
${openexrdir}/src/lib/Iex/IexMathExc.h
${openexrdir}/src/lib/Iex/IexMathFloatExc.h
${openexrdir}/src/lib/Iex/IexMathIeeeExc.h
${openexrdir}/src/lib/Iex/IexNamespace.h
${openexrdir}/src/lib/Iex/IexThrowErrnoExc.h
)

SET(ilmthread
${openexrdir}/src/lib/IlmThread/IlmThread.cpp
${openexrdir}/src/lib/IlmThread/IlmThread.h
${openexrdir}/src/lib/IlmThread/IlmThreadExport.h
${openexrdir}/src/lib/IlmThread/IlmThreadForward.h
#$openexrdirr}src/lib//IlmThread/IlmThreadMutex.cpp   		# Effectively empty
${openexrdir}/src/lib/IlmThread/IlmThreadMutex.h
#$openexrdirr}src/lib//IlmThread/IlmThreadMutexPosix.cpp   	# Effectively empty
${openexrdir}/src/lib/IlmThread/IlmThreadNamespace.h
${openexrdir}/src/lib/IlmThread/IlmThreadPool.cpp
${openexrdir}/src/lib/IlmThread/IlmThreadPool.h
#$openexrdirr}src/lib//IlmThread/IlmThreadPosix.cpp     	# Effectively empty
#$openexrdirr}src/lib//IlmThread/IlmThreadSemaphore.cpp		# Effectively empty
${openexrdir}/src/lib/IlmThread/IlmThreadSemaphore.h
${openexrdir}/src/lib/IlmThread/IlmThreadSemaphorePosix.cpp
${openexrdir}/src/lib/IlmThread/IlmThreadSemaphorePosixCompat.cpp
)
if(WIN32)
	SET(ilmthread
	${ilmthread}
	${openexrdir}/src/lib/IlmThread/IlmThreadSemaphoreWin32.cpp)
elseif(APPLE)
	SET(ilmthread
	${ilmthread}
	${openexrdir}/src/lib/IlmThread/IlmThreadSemaphoreOSX.cpp)
endif()

SET(openexr
${openexrdir}/src/lib/OpenEXR/b44ExpLogTable.h
${openexrdir}/src/lib/OpenEXR/dwaLookups.h
${openexrdir}/src/lib/OpenEXR/ImfAcesFile.cpp
${openexrdir}/src/lib/OpenEXR/ImfAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfB44Compressor.cpp
${openexrdir}/src/lib/OpenEXR/ImfBoxAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfChannelList.cpp
${openexrdir}/src/lib/OpenEXR/ImfChannelListAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfChromaticities.cpp
${openexrdir}/src/lib/OpenEXR/ImfChromaticitiesAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfCompositeDeepScanLine.cpp
${openexrdir}/src/lib/OpenEXR/ImfCompressionAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfCompressor.cpp
${openexrdir}/src/lib/OpenEXR/ImfConvert.cpp
${openexrdir}/src/lib/OpenEXR/ImfCRgbaFile.cpp
${openexrdir}/src/lib/OpenEXR/ImfDeepCompositing.cpp
${openexrdir}/src/lib/OpenEXR/ImfDeepFrameBuffer.cpp
${openexrdir}/src/lib/OpenEXR/ImfDeepImageStateAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfDeepScanLineInputFile.cpp
${openexrdir}/src/lib/OpenEXR/ImfDeepScanLineInputPart.cpp
${openexrdir}/src/lib/OpenEXR/ImfDeepScanLineOutputFile.cpp
${openexrdir}/src/lib/OpenEXR/ImfDeepScanLineOutputPart.cpp
${openexrdir}/src/lib/OpenEXR/ImfDeepTiledInputFile.cpp
${openexrdir}/src/lib/OpenEXR/ImfDeepTiledInputPart.cpp
${openexrdir}/src/lib/OpenEXR/ImfDeepTiledOutputFile.cpp
${openexrdir}/src/lib/OpenEXR/ImfDeepTiledOutputPart.cpp
${openexrdir}/src/lib/OpenEXR/ImfDoubleAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfDwaCompressor.cpp
${openexrdir}/src/lib/OpenEXR/ImfEnvmap.cpp
${openexrdir}/src/lib/OpenEXR/ImfEnvmapAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfFastHuf.cpp
${openexrdir}/src/lib/OpenEXR/ImfFloatAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfFloatVectorAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfFrameBuffer.cpp
${openexrdir}/src/lib/OpenEXR/ImfFramesPerSecond.cpp
${openexrdir}/src/lib/OpenEXR/ImfGenericInputFile.cpp
${openexrdir}/src/lib/OpenEXR/ImfGenericOutputFile.cpp
${openexrdir}/src/lib/OpenEXR/ImfHeader.cpp
${openexrdir}/src/lib/OpenEXR/ImfHuf.cpp
${openexrdir}/src/lib/OpenEXR/ImfIDManifest.cpp
${openexrdir}/src/lib/OpenEXR/ImfIDManifestAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfInputFile.cpp
${openexrdir}/src/lib/OpenEXR/ImfInputPart.cpp
${openexrdir}/src/lib/OpenEXR/ImfInputPartData.cpp
${openexrdir}/src/lib/OpenEXR/ImfIntAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfIO.cpp
${openexrdir}/src/lib/OpenEXR/ImfKeyCode.cpp
${openexrdir}/src/lib/OpenEXR/ImfKeyCodeAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfLineOrderAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfLut.cpp
${openexrdir}/src/lib/OpenEXR/ImfMatrixAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfMisc.cpp
${openexrdir}/src/lib/OpenEXR/ImfMultiPartInputFile.cpp
${openexrdir}/src/lib/OpenEXR/ImfMultiPartOutputFile.cpp
${openexrdir}/src/lib/OpenEXR/ImfMultiView.cpp
${openexrdir}/src/lib/OpenEXR/ImfOpaqueAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfOutputFile.cpp
${openexrdir}/src/lib/OpenEXR/ImfOutputPart.cpp
${openexrdir}/src/lib/OpenEXR/ImfOutputPartData.cpp
${openexrdir}/src/lib/OpenEXR/ImfPartType.cpp
${openexrdir}/src/lib/OpenEXR/ImfPizCompressor.cpp
${openexrdir}/src/lib/OpenEXR/ImfPreviewImage.cpp
${openexrdir}/src/lib/OpenEXR/ImfPreviewImageAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfPxr24Compressor.cpp
${openexrdir}/src/lib/OpenEXR/ImfRational.cpp
${openexrdir}/src/lib/OpenEXR/ImfRationalAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfRgbaFile.cpp
${openexrdir}/src/lib/OpenEXR/ImfRgbaYca.cpp
${openexrdir}/src/lib/OpenEXR/ImfRle.cpp
${openexrdir}/src/lib/OpenEXR/ImfRleCompressor.cpp
${openexrdir}/src/lib/OpenEXR/ImfScanLineInputFile.cpp
${openexrdir}/src/lib/OpenEXR/ImfStandardAttributes.cpp
${openexrdir}/src/lib/OpenEXR/ImfStdIO.cpp
${openexrdir}/src/lib/OpenEXR/ImfStringAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfStringVectorAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfSystemSpecific.cpp
${openexrdir}/src/lib/OpenEXR/ImfTestFile.cpp
${openexrdir}/src/lib/OpenEXR/ImfThreading.cpp
${openexrdir}/src/lib/OpenEXR/ImfTileDescriptionAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfTiledInputFile.cpp
${openexrdir}/src/lib/OpenEXR/ImfTiledInputPart.cpp
${openexrdir}/src/lib/OpenEXR/ImfTiledMisc.cpp
${openexrdir}/src/lib/OpenEXR/ImfTiledOutputFile.cpp
${openexrdir}/src/lib/OpenEXR/ImfTiledOutputPart.cpp
${openexrdir}/src/lib/OpenEXR/ImfTiledRgbaFile.cpp
${openexrdir}/src/lib/OpenEXR/ImfTileOffsets.cpp
${openexrdir}/src/lib/OpenEXR/ImfTimeCode.cpp
${openexrdir}/src/lib/OpenEXR/ImfTimeCodeAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfVecAttribute.cpp
${openexrdir}/src/lib/OpenEXR/ImfVersion.cpp
${openexrdir}/src/lib/OpenEXR/ImfWav.cpp
${openexrdir}/src/lib/OpenEXR/ImfZip.cpp
${openexrdir}/src/lib/OpenEXR/ImfZipCompressor.cpp

${openexrdir}/src/lib/OpenEXR/ImfAcesFile.h
${openexrdir}/src/lib/OpenEXR/ImfArray.h
${openexrdir}/src/lib/OpenEXR/ImfAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfBoxAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfChannelList.h
${openexrdir}/src/lib/OpenEXR/ImfChannelListAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfChromaticities.h
${openexrdir}/src/lib/OpenEXR/ImfChromaticitiesAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfCompositeDeepScanLine.h
${openexrdir}/src/lib/OpenEXR/ImfCompression.h
${openexrdir}/src/lib/OpenEXR/ImfCompressionAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfConvert.h
${openexrdir}/src/lib/OpenEXR/ImfCRgbaFile.h
${openexrdir}/src/lib/OpenEXR/ImfDeepCompositing.h
${openexrdir}/src/lib/OpenEXR/ImfDeepFrameBuffer.h
${openexrdir}/src/lib/OpenEXR/ImfDeepImageState.h
${openexrdir}/src/lib/OpenEXR/ImfDeepImageStateAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfDeepScanLineInputFile.h
${openexrdir}/src/lib/OpenEXR/ImfDeepScanLineInputPart.h
${openexrdir}/src/lib/OpenEXR/ImfDeepScanLineOutputFile.h
${openexrdir}/src/lib/OpenEXR/ImfDeepScanLineOutputPart.h
${openexrdir}/src/lib/OpenEXR/ImfDeepTiledInputFile.h
${openexrdir}/src/lib/OpenEXR/ImfDeepTiledInputPart.h
${openexrdir}/src/lib/OpenEXR/ImfDeepTiledOutputFile.h
${openexrdir}/src/lib/OpenEXR/ImfDeepTiledOutputPart.h
${openexrdir}/src/lib/OpenEXR/ImfDoubleAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfEnvmap.h
${openexrdir}/src/lib/OpenEXR/ImfEnvmapAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfExport.h
${openexrdir}/src/lib/OpenEXR/ImfFloatAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfFloatVectorAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfForward.h
${openexrdir}/src/lib/OpenEXR/ImfFrameBuffer.h
${openexrdir}/src/lib/OpenEXR/ImfFramesPerSecond.h
${openexrdir}/src/lib/OpenEXR/ImfGenericInputFile.h
${openexrdir}/src/lib/OpenEXR/ImfGenericOutputFile.h
${openexrdir}/src/lib/OpenEXR/ImfHeader.h
${openexrdir}/src/lib/OpenEXR/ImfHuf.h
${openexrdir}/src/lib/OpenEXR/ImfIDManifest.h
${openexrdir}/src/lib/OpenEXR/ImfIDManifestAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfInputFile.h
${openexrdir}/src/lib/OpenEXR/ImfInputPart.h
${openexrdir}/src/lib/OpenEXR/ImfInt64.h
${openexrdir}/src/lib/OpenEXR/ImfIntAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfIO.h
${openexrdir}/src/lib/OpenEXR/ImfKeyCode.h
${openexrdir}/src/lib/OpenEXR/ImfKeyCodeAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfLineOrder.h
${openexrdir}/src/lib/OpenEXR/ImfLineOrderAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfLut.h
${openexrdir}/src/lib/OpenEXR/ImfMatrixAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfMultiPartInputFile.h
${openexrdir}/src/lib/OpenEXR/ImfMultiPartOutputFile.h
${openexrdir}/src/lib/OpenEXR/ImfMultiView.h
${openexrdir}/src/lib/OpenEXR/ImfName.h
${openexrdir}/src/lib/OpenEXR/ImfNamespace.h
${openexrdir}/src/lib/OpenEXR/ImfOpaqueAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfOutputFile.h
${openexrdir}/src/lib/OpenEXR/ImfOutputPart.h
${openexrdir}/src/lib/OpenEXR/ImfPartHelper.h
${openexrdir}/src/lib/OpenEXR/ImfPartType.h
${openexrdir}/src/lib/OpenEXR/ImfPixelType.h
${openexrdir}/src/lib/OpenEXR/ImfPreviewImage.h
${openexrdir}/src/lib/OpenEXR/ImfPreviewImageAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfRational.h
${openexrdir}/src/lib/OpenEXR/ImfRationalAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfRgba.h
${openexrdir}/src/lib/OpenEXR/ImfRgbaFile.h
${openexrdir}/src/lib/OpenEXR/ImfRgbaYca.h
${openexrdir}/src/lib/OpenEXR/ImfStandardAttributes.h
${openexrdir}/src/lib/OpenEXR/ImfStdIO.h
${openexrdir}/src/lib/OpenEXR/ImfStringAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfStringVectorAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfTestFile.h
${openexrdir}/src/lib/OpenEXR/ImfThreading.h
${openexrdir}/src/lib/OpenEXR/ImfTileDescription.h
${openexrdir}/src/lib/OpenEXR/ImfTileDescriptionAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfTiledInputFile.h
${openexrdir}/src/lib/OpenEXR/ImfTiledInputPart.h
${openexrdir}/src/lib/OpenEXR/ImfTiledOutputFile.h
${openexrdir}/src/lib/OpenEXR/ImfTiledOutputPart.h
${openexrdir}/src/lib/OpenEXR/ImfTiledRgbaFile.h
${openexrdir}/src/lib/OpenEXR/ImfTimeCode.h
${openexrdir}/src/lib/OpenEXR/ImfTimeCodeAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfVecAttribute.h
${openexrdir}/src/lib/OpenEXR/ImfVersion.h
${openexrdir}/src/lib/OpenEXR/ImfWav.h
${openexrdir}/src/lib/OpenEXR/ImfXdr.h
)

# FILE(GLOB lib3ds "../../../lib3ds-1.2.0/lib3ds/*.c")


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


set(libtiff
${tiffdir}/tif_aux.c ${tiffdir}/tif_close.c 
${tiffdir}/tif_codec.c ${tiffdir}/tif_color.c ${tiffdir}/tif_compress.c ${tiffdir}/tif_dir.c 
${tiffdir}/tif_dirinfo.c ${tiffdir}/tif_dirread.c 
${tiffdir}/tif_dirwrite.c ${tiffdir}/tif_dumpmode.c ${tiffdir}/tif_error.c ${tiffdir}/tif_extension.c
${tiffdir}/tif_fax3.c ${tiffdir}/tif_fax3sm.c ${tiffdir}/tif_flush.c ${tiffdir}/tif_getimage.c ${tiffdir}/tif_jpeg.c 
${tiffdir}/tif_luv.c ${tiffdir}/tif_lzw.c ${tiffdir}/tif_next.c ${tiffdir}/tif_ojpeg.c 
${tiffdir}/tif_open.c ${tiffdir}/tif_packbits.c ${tiffdir}/tif_pixarlog.c ${tiffdir}/tif_predict.c ${tiffdir}/tif_print.c
${tiffdir}/tif_read.c ${tiffdir}/tif_strip.c ${tiffdir}/tif_swab.c ${tiffdir}/tif_thunder.c ${tiffdir}/tif_tile.c 
${tiffdir}/tif_unix.c ${tiffdir}/tif_version.c ${tiffdir}/tif_warning.c ${tiffdir}/tif_write.c ${tiffdir}/tif_zip.c
)

FILE(GLOB lua "${luadir}/src/*.c")

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

#set(zlib
#${zlibdir}/adler32.c ${zlibdir}/compress.c ${zlibdir}/crc32.c ${zlibdir}/deflate.c ${zlibdir}/gzclose.c ${zlibdir}/gzlib.c ${zlibdir}/gzread.c ${zlibdir}/gzwrite.c
#${zlibdir}/infback.c ${zlibdir}/inffast.c ${zlibdir}/inflate.c ${zlibdir}/inftrees.c ${zlibdir}/trees.c ${zlibdir}/uncompr.c ${zlibdir}/zutil.c
#${zlibdir}/contrib/minizip/unzip.c ${zlibdir}/contrib/minizip/zip.c ${zlibdir}/contrib/minizip/ioapi.c
#)

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

FILE(GLOB hdr "${INDIGO_TRUNK}/hdr/*.c" "${INDIGO_TRUNK}/hdr/*.h")



FILE(GLOB embreecommonsys "../embree/common/sys/*.cpp")# "common/sys/*.h")
FILE(GLOB embreecommonsysstl "../embree/common/sys/stl/*.cpp")# "common/sys/stl/*.h")
FILE(GLOB embreecommonsyssync "../embree/common/sys/sync/*.cpp")# "common/sys/sync/*.h")
FILE(GLOB embreecommonmath "../embree/common/math/*.h")
FILE(GLOB embreecommonsimd "../embree/common/simd/*.h")

FILE(GLOB embreertcore "../embree/rtcore/*.cpp")# "rtcore/*.h")
FILE(GLOB embreertcorebvh2 "../embree/rtcore/bvh2/*.cpp")# "rtcore/bvh2/*.h")
FILE(GLOB embreertcorebvh4 "../embree/rtcore/bvh4/*.cpp")# "rtcore/bvh4/*.h")
FILE(GLOB embreertcorecommon "../embree/rtcore/common/*.cpp")# "rtcore/common/*.h")


#SOURCE_GROUP(fftss FILES ${fftss})
SOURCE_GROUP(ilmbase FILES ${ilmbase})
SOURCE_GROUP(ilmthread FILES ${ilmthread})
SOURCE_GROUP(openexr FILES ${openexr})
#SOURCE_GROUP(lib3ds FILES ${lib3ds})
SOURCE_GROUP(libpng FILES ${libpng})
SOURCE_GROUP(libtiff FILES ${libtiff})
#SOURCE_GROUP(lua FILES ${lua})
SOURCE_GROUP(zlib FILES ${zlib})
SOURCE_GROUP(zlib\\minizip FILES ${minizip})
SOURCE_GROUP(pugixml FILES ${pugixml})
SOURCE_GROUP(giflib FILES ${giflib})
SOURCE_GROUP(little_cms FILES ${little_cms})
SOURCE_GROUP(zstandard FILES ${zstandard})

#Embree:
SOURCE_GROUP(embree\\common)
SOURCE_GROUP(embree\\common\\image FILES ${embreecommonimage})
SOURCE_GROUP(embree\\common\\sys FILES ${embreecommonsys})
SOURCE_GROUP(embree\\common\\sys\\stl FILES ${embreecommonsysstl})
SOURCE_GROUP(embree\\common\\sys\\sync FILES ${embreecommonsyssync})
SOURCE_GROUP(embree\\common\\math FILES ${embreecommonmath})
SOURCE_GROUP(embree\\common\\simd FILES ${embreecommonsimd})

SOURCE_GROUP(embree\\rtcore FILES ${embreertcore})
SOURCE_GROUP(embree\\rtcore\\bvh2 FILES ${embreertcorebvh2})
SOURCE_GROUP(embree\\rtcore\\bvh4 FILES ${embreertcorebvh4})
SOURCE_GROUP(embree\\rtcore\\common FILES ${embreertcorecommon})

#SOURCE_GROUP(spout FILES ${spout})


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

set(scenes
scenes/Scene.h
scenes/JuliaFractalScene.h
scenes/TestPatternScene.h
scenes/LinesDistanceScene.h
scenes/SynthWaveScene.h
scenes/VectorFieldScene.h
scenes/VideoScene.h
scenes/VoronoiScene.h
scenes/voronoi_vert.glsl
scenes/voronoi_frag.glsl
scenes/BoxesScene.h
scenes/IFSScene.h
scenes/KaleidoscopeRefractScene.h
scenes/PointsDistanceScene.h
blend_vert.glsl
blend_frag.glsl
scenes/BlendScene.h
scenes/LightTrailScene.h
scenes/light_trail_frag.glsl
scenes/light_trail_vert.glsl
scenes/FractalFeedbackScene.h
scenes/fractal_feedback_vert.glsl
scenes/fractal_feedback_frag.glsl
)

SOURCE_GROUP(rtmidi FILES ${rtmidi})
SOURCE_GROUP(indigo FILES ${indigo})
SOURCE_GROUP(graphics FILES ${graphics})
SOURCE_GROUP(opengl FILES ${opengl})
SOURCE_GROUP(utils FILES ${utils})
SOURCE_GROUP(maths FILES ${maths})
SOURCE_GROUP(double-conversion FILES ${double-conversion})
SOURCE_GROUP(hdr FILES ${hdr})
SOURCE_GROUP(video FILES ${video})
SOURCE_GROUP(scenes FILES ${scenes})
SOURCE_GROUP(kissfft FILES ${kissfft})
SOURCE_GROUP(imgui FILES ${imgui})
SOURCE_GROUP(networking FILES ${networking})

add_executable(aibot
AIBot.cpp
notes.txt
#${scenes}
#${rtmidi}
#${indigo}
#${graphics}
#${opengl}
${utils}
${maths}
${double-conversion}
#${hdr}
#${video}
#${fftss}
#${ilmbase}
#${ilmthread}
#${openexr}
#${lib3ds}
#${libjpg}
#${libpng}
#${libtiff}
#${lua}
${zlib}
#${minizip}
${pugixml}
#${giflib}
#${little_cms}
${zstandard}
${networking}
#${embreecommonimage}
#${embreecommonsys}
#${embreecommonsysstl}
#${embreecommonsyssync}
#${embreecommonmath}
#${embreecommonsimd}
#${embreertcore}
#${embreertcorebvh2}
#${embreertcorebvh4}
#${embreertcorecommon}
#${kissfft}
#${imgui}
)


if(WIN32)
	SET(TURBOJPEG_LIB ${LIBJPEG_TURBO_DIR}/$(Configuration)/turbojpeg-static.lib)
else()
	SET(TURBOJPEG_LIB ${LIBJPEG_TURBO_DIR}/libjpeg.a)
endif()


set(CURRENT_TARGET aibot)
include("cmake/libressl.cmake")



target_link_libraries(aibot
#debug "${SDL_BUILD_DIR}/Debug/SDL2d.lib" 
#debug "${SDL_BUILD_DIR}/Debug/SDL2maind.lib" 
#optimized "${SDL_BUILD_DIR}/RelWithDebInfo/SDL2.lib" 
#optimized "${SDL_BUILD_DIR}/RelWithDebInfo/SDL2main.lib" 
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










