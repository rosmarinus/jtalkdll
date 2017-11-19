# CMakeLists.txt for open_jtalk
cmake_minimum_required(VERSION 2.8.6)

# command line option
# -Dtarget_arch=(i686|x86_64) : cygwin
#    default value = uname -m

project(OpenJTalk)
set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_VERBOSE_MAKEFILE TRUE)
#set(target_arch x86_64)

### source directory
set(HTS_ENGINE_API_PATH "${CMAKE_SOURCE_DIR}/hts_engine_API-1.10")
set(OPEN_JTALK_PATH "${CMAKE_SOURCE_DIR}/open_jtalk-1.10")
set(OPEN_JTALK_DIC_PATH "${CMAKE_SOURCE_DIR}/open_jtalk_dic_utf_8-1.10")
set(HTS_VOICE_PATH "${CMAKE_SOURCE_DIR}/hts_voice_nitech_jp_atr503_m001-1.05")


### sub folder name install
set(FOLDER_INSTALL_BIN   bin)
set(FOLDER_INSTALL_LIB   lib)
set(FOLDER_INSTALL_DIC   dic_utf_8)
set(FOLDER_INSTALL_VOICE voice)


### set mingw compiler
if(MINGW)
	set(target_arch          "$ENV{MINGW_CHOST}")
    set(mingw_prefix         "$ENV{MINGW_PREFIX}")
    set(CMAKE_FIND_ROOT_PATH "${mingw_prefix}")
    set(CMAKE_C_COMPILER     gcc)
    set(CMAKE_CXX_COMPILER   g++)
    set(CMAKE_RANLIB         ranlib)
    set(CMAKE_AS             as)
    set(CMAKE_AR             ar)
    set(CMAKE_STRIP          strip)
    set(mingw_build 1)
elseif(CYGWIN)
    if(NOT target_arch)
		if(CMAKE_UNAME)
			exec_program(uname ARGS -m OUTPUT_VARIABLE target_arch)
		endif()
	endif()
    if(target_arch)
	   	set(target_arch          "${target_arch}-w64-mingw32")
	    set(CMAKE_FIND_ROOT_PATH "/usr/${target_arch}")
        set(CMAKE_C_COMPILER     ${target_arch}-gcc)
        set(CMAKE_CXX_COMPILER   ${target_arch}-g++)
        set(CMAKE_RANLIB         ${target_arch}-ranlib)
        set(CMAKE_AS             ${target_arch}-as)
        set(CMAKE_AR             ${target_arch}-ar)
        set(CMAKE_STRIP          ${target_arch}-strip)
	    set(mingw_build 1)
	endif()
endif()
if(mingw_build)
	set(CMAKE_SYSTEM_NAME Windows)
	set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
	set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
	set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endif()


### set install directory
if(WIN32 OR mingw_build)
    set(PROJECT_FOLDER open_jtalk)
else()
    set(PROJECT_FOLDER OpenJTalk)
endif()


### set install base path
if(MSVC AND WIN32)
    set(BASE_PATH c:/)
elseif(MINGW)
    set(BASE_PATH /c/)
elseif(CYGWIN)
    set(BASE_PATH /cygdrive/c/)
else()
    set(BASE_PATH /usr/local/)
endif()


### set install prefix
if(MSVC AND WIN32)
    set(CMAKE_INSTALL_PREFIX ${BASE_PATH}${PROJECT_FOLDER})
elseif(MINGW)
    set(CMAKE_INSTALL_PREFIX ${BASE_PATH}${PROJECT_FOLDER})
elseif(CYGWIN)
    set(CMAKE_INSTALL_PREFIX ${BASE_PATH}${PROJECT_FOLDER})
else()
    set(CMAKE_INSTALL_PREFIX /usr/local)
endif()


### data install prefix
set(DATA_INSTALL_PREFIX ${BASE_PATH}${PROJECT_FOLDER})


### mecab const
set(DIC_VERSION 102)
set(PACKAGE "open_jtalk")
set(VERSION "1.10")


### look for libiconv
set(ENABLE_LICONV 0)
if(UNIX)
    if(NOT mingw_build)
        find_library(ICONV_LIBRARY NAMES iconv)
        if(ICONV_LIBRARY)
            set(ENABLE_LICONV 1)
        endif()
    endif()
endif()
if(ENABLE_LICONV)
    message("-- Enable '-liconv'")
else()
    message("-- Disable '-liconv'")
endif()


### c/cxx flags
if(MSVC AND WIN32)
    set(CMAKE_C_FLAGS   "/MD /O2 /Ob2 /Oi /Ot /Oy /GT")
    set(CMAKE_CXX_FLAGS "/MD /O2 /Ob2 /Oi /Ot /Oy /GT")
    set(CMAKE_C_FLAGS_RELEASE "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
    set(CMAKE_C_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_DEBUG "")

    ### openjtalk c flags
    set(openjtalk_compile_flags "/TC /D CHARSET_UTF_8 /D ASCII_HEADER")

    ### hts_engine_API c flgas
    set(hts_c_compile_flags "/TC /D CHARSET_UTF_8 /D ASCII_HEADER")

    ### mecab cpp flgas
    set(cxx_compile_flags "/TP")
    set(cxx_compile_flags "${cxx_compile_flags} /DHAVE_WINDOWS_H")
    set(cxx_compile_flags "${cxx_compile_flags} /DASCII_HEADER")
    set(cxx_compile_flags "${cxx_compile_flags} /DDIC_VERSION=${DIC_VERSION}")
    set(cxx_compile_flags "${cxx_compile_flags} /DMECAB_USE_UTF8_ONLY")
    set(cxx_compile_flags "${cxx_compile_flags} /DCHARSET_UTF_8")
    set(cxx_compile_flags "${cxx_compile_flags} /DMECAB_WITHOUT_MUTEX_LOCK")
    set(cxx_compile_flags "${cxx_compile_flags} /DMECAB_DEFAULT_RC=\"dummy\"")
    set(cxx_compile_flags "${cxx_compile_flags} /DPACKAGE=\"\\\"${PACKAGE}\\\"\"")
    set(cxx_compile_flags "${cxx_compile_flags} /DVERSION=\"\\\"${VERSION}\\\"\"")
    set(mecab_cxx_compile_flags_with_EHa "/EHa ${cxx_compile_flags}")
    set(mecab_cxx_compile_flags "/EHsc ${cxx_compile_flags}")

    set(CMAKE_L_FLAGS "/LTCG")

else()
    ### openjtalk c flags
    set(openjtalk_compile_flags "-O2 -Wall -g -finput-charset=UTF-8 -fexec-charset=UTF-8 -D CHARSET_UTF_8 -D ASCII_HEADER")

    ### hts_engine_API c flgas
    set(hts_c_compile_flags "-O2 -Wall -g -finput-charset=UTF-8 -fexec-charset=UTF-8 -D CHARSET_UTF_8 -D ASCII_HEADER")

    ### mecab cpp flgas
    set(mecab_cxx_compile_flags "")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -O3 -Wall -Wno-deprecated")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -finput-charset=UTF-8 -fexec-charset=UTF-8")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -D HAVE_CONFIG_H  -D HAVE_UNISTD_H")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -D DIC_VERSION=${DIC_VERSION}")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -D MECAB_USE_UTF8_ONLY")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -D MECAB_DEFAULT_RC=\"$dummt\"")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -D PACKAGE=\"\\\"${PACKAGE}\\\"\"")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -D VERSION=\"\\\"${VERSION}\\\"\"")

    ### mingw
    if(mingw_build)

    ### cygwin without mingw compiler
    elseif(CYGWIN)
        set(openjtalk_compile_flags "${openjtalk_compile_flags} -std=gnu11")
        set(hts_c_compile_flags "${hts_c_compile_flags} -std=gnu11")
        set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -std=gnu++11")

    ### linux
    else()
        set(openjtalk_compile_flags "${openjtalk_compile_flags} -std=c11 -fPIC")
        set(hts_c_compile_flags "${hts_c_compile_flags} -std=c11 -fPIC")
        set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -std=c++11 -fPIC")
    endif()

endif()


### mecab
include_directories(${OPEN_JTALK_PATH}/mecab)
include_directories(${OPEN_JTALK_PATH}/mecab/src)
add_library(mecab STATIC
    ${OPEN_JTALK_PATH}/mecab/src/char_property.cpp
    ${OPEN_JTALK_PATH}/mecab/src/connector.cpp
    ${OPEN_JTALK_PATH}/mecab/src/context_id.cpp
    ${OPEN_JTALK_PATH}/mecab/src/dictionary.cpp
    ${OPEN_JTALK_PATH}/mecab/src/dictionary_compiler.cpp
    ${OPEN_JTALK_PATH}/mecab/src/dictionary_generator.cpp
    ${OPEN_JTALK_PATH}/mecab/src/dictionary_rewriter.cpp
    ${OPEN_JTALK_PATH}/mecab/src/eval.cpp
    ${OPEN_JTALK_PATH}/mecab/src/feature_index.cpp
    ${OPEN_JTALK_PATH}/mecab/src/iconv_utils.cpp
    ${OPEN_JTALK_PATH}/mecab/src/lbfgs.cpp
    ${OPEN_JTALK_PATH}/mecab/src/learner.cpp
    ${OPEN_JTALK_PATH}/mecab/src/learner_tagger.cpp
    ${OPEN_JTALK_PATH}/mecab/src/libmecab.cpp
    ${OPEN_JTALK_PATH}/mecab/src/mecab.cpp
    ${OPEN_JTALK_PATH}/mecab/src/nbest_generator.cpp
    ${OPEN_JTALK_PATH}/mecab/src/param.cpp
    ${OPEN_JTALK_PATH}/mecab/src/string_buffer.cpp
    ${OPEN_JTALK_PATH}/mecab/src/tagger.cpp
    ${OPEN_JTALK_PATH}/mecab/src/tokenizer.cpp
    ${OPEN_JTALK_PATH}/mecab/src/utils.cpp
    ${OPEN_JTALK_PATH}/mecab/src/viterbi.cpp
    ${OPEN_JTALK_PATH}/mecab/src/writer.cpp
)
set_target_properties(mecab PROPERTIES COMPILE_FLAGS ${mecab_cxx_compile_flags})


### text2mecab
include_directories(${OPEN_JTALK_PATH}/text2mecab)
add_library(text2mecab STATIC
    ${OPEN_JTALK_PATH}/text2mecab/text2mecab.c
)
set_target_properties(text2mecab PROPERTIES COMPILE_FLAGS ${openjtalk_compile_flags})


### mecab2njd
include_directories(${OPEN_JTALK_PATH}/mecab2njd)
add_library(mecab2njd STATIC
    ${OPEN_JTALK_PATH}/mecab2njd/mecab2njd.c
)
set_target_properties(mecab2njd PROPERTIES COMPILE_FLAGS ${openjtalk_compile_flags})


### njd
include_directories(${OPEN_JTALK_PATH}/njd)
add_library(njd STATIC
    ${OPEN_JTALK_PATH}/njd/njd.c
    ${OPEN_JTALK_PATH}/njd/njd_node.c
)
set_target_properties(njd PROPERTIES COMPILE_FLAGS ${openjtalk_compile_flags})


### njd_set_pronunciation
include_directories(${OPEN_JTALK_PATH}/njd_set_pronunciation)
add_library(njd_set_pronunciation STATIC
    ${OPEN_JTALK_PATH}/njd_set_pronunciation/njd_set_pronunciation.c
)
set_target_properties(njd_set_pronunciation PROPERTIES COMPILE_FLAGS ${openjtalk_compile_flags})


### njd_set_digit
include_directories(${OPEN_JTALK_PATH}/njd_set_digit)
add_library(njd_set_digit STATIC
    ${OPEN_JTALK_PATH}/njd_set_digit/njd_set_digit.c
)
set_target_properties(njd_set_digit PROPERTIES COMPILE_FLAGS ${openjtalk_compile_flags})


### njd_set_accent_phrase
include_directories(${OPEN_JTALK_PATH}/njd_set_accent_phrase)
add_library(njd_set_accent_phrase STATIC
    ${OPEN_JTALK_PATH}/njd_set_accent_phrase/njd_set_accent_phrase.c
)
set_target_properties(njd_set_accent_phrase PROPERTIES COMPILE_FLAGS ${openjtalk_compile_flags})


### njd_set_accent_type
include_directories(${OPEN_JTALK_PATH}/njd_set_accent_type)
add_library(njd_set_accent_type STATIC
    ${OPEN_JTALK_PATH}/njd_set_accent_type/njd_set_accent_type.c
)
set_target_properties(njd_set_accent_type PROPERTIES COMPILE_FLAGS ${openjtalk_compile_flags})


### njd_set_unvoiced_vowel
include_directories(${OPEN_JTALK_PATH}/njd_set_unvoiced_vowel)
add_library(njd_set_unvoiced_vowel STATIC
    ${OPEN_JTALK_PATH}/njd_set_unvoiced_vowel/njd_set_unvoiced_vowel.c
)
set_target_properties(njd_set_unvoiced_vowel PROPERTIES COMPILE_FLAGS ${openjtalk_compile_flags})


### njd_set_long_vowel
include_directories(${OPEN_JTALK_PATH}/njd_set_long_vowel)
add_library(njd_set_long_vowel STATIC
    ${OPEN_JTALK_PATH}/njd_set_long_vowel/njd_set_long_vowel.c
)
set_target_properties(njd_set_long_vowel PROPERTIES COMPILE_FLAGS ${openjtalk_compile_flags})


### njd2jpcommon
include_directories(${OPEN_JTALK_PATH}/njd2jpcommon)
add_library(njd2jpcommon STATIC
    ${OPEN_JTALK_PATH}/njd2jpcommon/njd2jpcommon.c
)
set_target_properties(njd2jpcommon PROPERTIES COMPILE_FLAGS ${openjtalk_compile_flags})


### jpcommon
include_directories(${OPEN_JTALK_PATH}/jpcommon)
add_library(jpcommon STATIC
    ${OPEN_JTALK_PATH}/jpcommon/jpcommon.c
    ${OPEN_JTALK_PATH}/jpcommon/jpcommon_node.c
    ${OPEN_JTALK_PATH}/jpcommon/jpcommon_label.c
)
set_target_properties(jpcommon PROPERTIES COMPILE_FLAGS ${openjtalk_compile_flags})


### HTSEngine
include_directories(${HTS_ENGINE_API_PATH}/include)
add_library(HTSEngine STATIC
    ${HTS_ENGINE_API_PATH}/lib/HTS_audio.c
    ${HTS_ENGINE_API_PATH}/lib/HTS_engine.c
    ${HTS_ENGINE_API_PATH}/lib/HTS_hidden.h
    ${HTS_ENGINE_API_PATH}/lib/HTS_misc.c
    ${HTS_ENGINE_API_PATH}/lib/HTS_pstream.c
    ${HTS_ENGINE_API_PATH}/lib/HTS_sstream.c
    ${HTS_ENGINE_API_PATH}/lib/HTS_model.c
    ${HTS_ENGINE_API_PATH}/lib/HTS_vocoder.c
    ${HTS_ENGINE_API_PATH}/lib/HTS_gstream.c
    ${HTS_ENGINE_API_PATH}/lib/HTS_label.c
)
set_target_properties(HTSEngine PROPERTIES COMPILE_FLAGS ${hts_c_compile_flags})


### hts_engine
add_executable(hts_engine ${HTS_ENGINE_API_PATH}/bin/hts_engine.c)
target_link_libraries(hts_engine HTSEngine)
if(mingw_build)
    target_link_libraries(hts_engine winmm)
elseif(MSVC AND WIN32)
    target_link_libraries(hts_engine winmm)
elseif(CYGWIN)
    target_link_libraries(hts_engine winmm)
elseif(UNIX)
    target_link_libraries(hts_engine m)
endif()
set_target_properties(hts_engine PROPERTIES COMPILE_FLAGS ${hts_c_compile_flags})
if(mingw_build)
    set_target_properties(hts_engine PROPERTIES PREFIX "" SUFFIX ".exe")
endif()
install(TARGETS hts_engine RUNTIME DESTINATION ${FOLDER_INSTALL_BIN})

### mecab-dict-index
include_directories(${OPEN_JTALK_PATH}/mecab/src)
add_executable(mecab-dict-index
    ${OPEN_JTALK_PATH}/mecab/src/mecab-dict-index.cpp
)
set_target_properties(mecab-dict-index PROPERTIES COMPILE_FLAGS ${mecab_cxx_compile_flags})
target_link_libraries(mecab-dict-index mecab)
if(APPLE)
    target_link_libraries(mecab-dict-index iconv)
elseif(UNIX AND ENABLE_LICONV)
    target_link_libraries(mecab-dict-index iconv)
endif()
if(mingw_build)
    set_target_properties(mecab-dict-index PROPERTIES LINK_FLAGS -static)
    set_target_properties(mecab-dict-index PROPERTIES PREFIX "" SUFFIX ".exe")
endif()
install(TARGETS mecab-dict-index RUNTIME DESTINATION ${FOLDER_INSTALL_BIN})

### open_jtalk
add_executable(open_jtalk
    ${OPEN_JTALK_PATH}/bin/open_jtalk.c
)
target_link_libraries(open_jtalk
    text2mecab
    mecab
    mecab2njd
    njd
    njd_set_pronunciation
    njd_set_digit
    njd_set_accent_phrase
    njd_set_accent_type
    njd_set_unvoiced_vowel
    njd_set_long_vowel
    njd2jpcommon
    jpcommon
    HTSEngine
)
set_target_properties(open_jtalk PROPERTIES COMPILE_FLAGS ${openjtalk_compile_flags})
if(mingw_build)
    set_target_properties(open_jtalk PROPERTIES LINK_FLAGS -static)
    target_link_libraries(open_jtalk winmm)
elseif(MSVC AND WIN32)
    target_link_libraries(open_jtalk winmm)
elseif(CYGWIN)
    target_link_libraries(open_jtalk winmm)
elseif(APPLE)
    target_link_libraries(open_jtalk iconv)
elseif(UNIX AND ENABLE_LICONV)
    target_link_libraries(open_jtalk iconv)
endif()
if(mingw_build)
    set_target_properties(open_jtalk PROPERTIES PREFIX "" SUFFIX ".exe")
endif()
install(TARGETS open_jtalk RUNTIME DESTINATION ${FOLDER_INSTALL_BIN})


### install dictionary
file(GLOB dicfiles ${OPEN_JTALK_DIC_PATH}/*)
install(FILES ${dicfiles} DESTINATION ${DATA_INSTALL_PREFIX}/${FOLDER_INSTALL_DIC})


### install voices
install(DIRECTORY ${HTS_VOICE_PATH} DESTINATION ${DATA_INSTALL_PREFIX}/${FOLDER_INSTALL_VOICE})
