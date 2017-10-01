cmake_minimum_required(VERSION 2.6)

set(CMAKE_VERBOSE_MAKEFILE TRUE)

project(jtalkdll)
#set(CMAKE_IMPORT_LIBRARY_PREFIX "lib")

# compiler
if(MINGW)
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
    set(mingw_chost $ENV{MINGW_CHOST})
    #message("MINGW_CHOST: ${mingw_chost}")
    if(${mingw_chost} MATCHES "mingw")
        set(CMAKE_C_COMPILER ${mingw_chost}-gcc)
        set(CMAKE_CXX_COMPILER ${mingw_chost}-g++)
        #message("cxx: ${CMAKE_CXX_COMPILER}")
    endif()
    message("cc : ${CMAKE_C_COMPILER}")
    message("cxx: ${CMAKE_CXX_COMPILER}")
endif()

set(CMAKE_BUILD_TYPE Release)

set(OPEN_JTALK_PATH "open_jtalk-1.10")
set(HTS_ENGINE_API_PATH "hts_engine_API-1.10")

include_directories(include)

set(DIC_VERSION 102)
set(PACKAGE "open_jtalk")
set(VERSION "1.10")


if(MSVC)
    # openjtalk c flags
    set(openjtalk_compile_flags "/MD /O2 /Ob2 /Oi /Ot /Oy /GT /TC /D CHARSET_UTF_8 /D ASCII_HEADER")

    # hts_engine_API c flgas
    set(hts_c_compile_flags "/MD /O2 /Ob2 /Oi /Ot /Oy /GT /TC /D CHARSET_UTF_8 /D ASCII_HEADER")

    # mecab cpp flgas
    set(mecab_cxx_compile_flags "")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} /MD /O2 /Ob2 /Oi /Ot /Oy /GT /TP /EHsc")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} /D HAVE_WINDOWS_H")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} /D ASCII_HEADER")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} /D DIC_VERSION=${DIC_VERSION}")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} /D MECAB_USE_UTF8_ONLY")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} /D CHARSET_UTF_8")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} /D MECAB_WITHOUT_MUTEX_LOCK")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} /D MECAB_DEFAULT_RC=\"dummy\"")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} /D PACKAGE=\"\\\"${PACKAGE}\\\"\"")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} /D VERSION=\"\\\"${VERSION}\\\"\"")

    set(CMAKE_L_FLAGS "/LTCG")

    file(WRITE sharptest.cpp "using namespace System;void main(){}")

else()
    # openjtalk c flags
    set(openjtalk_compile_flags "-O2 -Wall -g -finput-charset=UTF-8 -fexec-charset=UTF-8 -D CHARSET_UTF_8 -D ASCII_HEADER")

    # hts_engine_API c flgas
    set(hts_c_compile_flags "-O2 -Wall -g -finput-charset=UTF-8 -fexec-charset=UTF-8 -D CHARSET_UTF_8 -D ASCII_HEADER")

    # mecab cpp flgas
    set(mecab_cxx_compile_flags "")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -O3 -Wall -Wno-deprecated")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -finput-charset=UTF-8 -fexec-charset=UTF-8")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -D HAVE_CONFIG_H  -D HAVE_UNISTD_H")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -D DIC_VERSION=${DIC_VERSION}")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -D MECAB_USE_UTF8_ONLY")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -D MECAB_DEFAULT_RC=\"$dummt\"")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -D PACKAGE=\"\\\"${PACKAGE}\\\"\"")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -D VERSION=\"\\\"${VERSION}\\\"\"")

    # cygwin
    #if(${CMAKE_SYSTEM_NAME} MATCHES "Cygwin")
    if(CYGWIN)
        set(openjtalk_compile_flags "${openjtalk_compile_flags} -std=gnu11")
        set(hts_c_compile_flags "${hts_c_compile_flags} -std=gnu11")
        set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -std=gnu++11")

    # mingw
    elseif(MINGW)
        message("mingw compiler")
        set(openjtalk_compile_flags "${openjtalk_compile_flags} -static-libgcc -static-libstdc++ -static")
        set(hts_c_compile_flags "${hts_c_compile_flags} -static-libgcc -static-libstdc++ -static")
        set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -static-libgcc -static-libstdc++ -static")

    # linux
    else()
        set(openjtalk_compile_flags "${openjtalk_compile_flags} -fPIC")
        set(hts_c_compile_flags "${hts_c_compile_flags} -fPIC")
        set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -fPIC")
    endif()

    set(CMAKE_L_FLAGS "")

endif()


set(serial "1.0.0")
set(soserial "1")


# project(mecab CXX)
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


# project(text2mecab C)
include_directories(${OPEN_JTALK_PATH}/text2mecab)
add_library(text2mecab STATIC
    ${OPEN_JTALK_PATH}/text2mecab/text2mecab.c
)
set(text2mecab_compile_flags ${openjtalk_compile_flags})
set_target_properties(text2mecab PROPERTIES COMPILE_FLAGS ${text2mecab_compile_flags})


# project(mecab2njd C)
include_directories(${OPEN_JTALK_PATH}/mecab2njd)
add_library(mecab2njd STATIC
    ${OPEN_JTALK_PATH}/mecab2njd/mecab2njd.c
)
set(mecab2njd_compile_flags ${openjtalk_compile_flags})
set_target_properties(mecab2njd PROPERTIES COMPILE_FLAGS ${mecab2njd_compile_flags})


# project(mecab-dict-index CXX)
include_directories(${OPEN_JTALK_PATH}/mecab/src)
add_executable(mecab-dict-index
    ${OPEN_JTALK_PATH}/mecab/src/mecab-dict-index.cpp
)
set_target_properties(mecab-dict-index PROPERTIES COMPILE_FLAGS ${mecab_cxx_compile_flags})
target_link_libraries(mecab-dict-index mecab)
if(APPLE)
    target_link_libraries(mecab-dict-index iconv)
endif()


# project(njd C)
include_directories(${OPEN_JTALK_PATH}/njd)
add_library(njd STATIC
    ${OPEN_JTALK_PATH}/njd/njd.c
    ${OPEN_JTALK_PATH}/njd/njd_node.c
)
set(njd_compile_flags ${openjtalk_compile_flags})
set_target_properties(njd PROPERTIES COMPILE_FLAGS ${njd_compile_flags})


# project(njd_set_pronunciation C)
include_directories(${OPEN_JTALK_PATH}/njd_set_pronunciation)
add_library(njd_set_pronunciation STATIC
    ${OPEN_JTALK_PATH}/njd_set_pronunciation/njd_set_pronunciation.c
)
set(njd_set_pronunciation_compile_flags ${openjtalk_compile_flags})
set_target_properties(njd_set_pronunciation PROPERTIES COMPILE_FLAGS ${njd_set_pronunciation_compile_flags})


# project(njd_set_digit C)
include_directories(${OPEN_JTALK_PATH}/njd_set_digit)
add_library(njd_set_digit STATIC
    ${OPEN_JTALK_PATH}/njd_set_digit/njd_set_digit.c
)
set(njd_set_digit_compile_compile_flags ${openjtalk_compile_flags})
set_target_properties(njd_set_digit PROPERTIES COMPILE_FLAGS ${njd_set_digit_compile_compile_flags})


# project(njd_set_accent_phrase C)
include_directories(${OPEN_JTALK_PATH}/njd_set_accent_phrase)
add_library(njd_set_accent_phrase STATIC
    ${OPEN_JTALK_PATH}/njd_set_accent_phrase/njd_set_accent_phrase.c
)
set(njd_set_accent_phrase_compile_flags ${openjtalk_compile_flags})
set_target_properties(njd_set_accent_phrase PROPERTIES COMPILE_FLAGS ${njd_set_accent_phrase_compile_flags})


# project(njd_set_accent_type C)
include_directories(${OPEN_JTALK_PATH}/njd_set_accent_type)
add_library(njd_set_accent_type STATIC
    ${OPEN_JTALK_PATH}/njd_set_accent_type/njd_set_accent_type.c
)
set(njd_set_accent_type_compile_flags ${openjtalk_compile_flags})
set_target_properties(njd_set_accent_type PROPERTIES COMPILE_FLAGS ${njd_set_accent_type_compile_flags})


# project(njd_set_unvoiced_vowel C)
include_directories(${OPEN_JTALK_PATH}/njd_set_unvoiced_vowel)
add_library(njd_set_unvoiced_vowel STATIC
    ${OPEN_JTALK_PATH}/njd_set_unvoiced_vowel/njd_set_unvoiced_vowel.c
)
set(njd_set_unvoiced_vowel_compile_flags ${openjtalk_compile_flags})
set_target_properties(njd_set_unvoiced_vowel PROPERTIES COMPILE_FLAGS ${njd_set_unvoiced_vowel_compile_flags})


# project(njd_set_long_vowel C)
include_directories(${OPEN_JTALK_PATH}/njd_set_long_vowel)
add_library(njd_set_long_vowel STATIC
    ${OPEN_JTALK_PATH}/njd_set_long_vowel/njd_set_long_vowel.c
)
set(njd_set_long_vowel_compile_flags ${openjtalk_compile_flags})
set_target_properties(njd_set_long_vowel PROPERTIES COMPILE_FLAGS ${njd_set_long_vowel_compile_flags})


# project(njd2jpcommon C)
include_directories(${OPEN_JTALK_PATH}/njd2jpcommon)
add_library(njd2jpcommon STATIC
    ${OPEN_JTALK_PATH}/njd2jpcommon/njd2jpcommon.c
)
set(njd2jpcommon_compile_flags ${openjtalk_compile_flags})
set_target_properties(njd2jpcommon PROPERTIES COMPILE_FLAGS ${njd2jpcommon_compile_flags})


# project(jpcommon C)
include_directories(${OPEN_JTALK_PATH}/jpcommon)
add_library(jpcommon STATIC
    ${OPEN_JTALK_PATH}/jpcommon/jpcommon.c
    ${OPEN_JTALK_PATH}/jpcommon/jpcommon_node.c
    ${OPEN_JTALK_PATH}/jpcommon/jpcommon_label.c
)
set(jpcommon_compile_flags ${openjtalk_compile_flags})
set_target_properties(jpcommon PROPERTIES COMPILE_FLAGS ${jpcommon_compile_flags})


# project(HTSEngine C)
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
set(HTSEngine_compile_flags ${hts_c_compile_flags})
set_target_properties(HTSEngine PROPERTIES COMPILE_FLAGS ${HTSEngine_compile_flags})


# project(hts_engine C)
add_executable(hts_engine ${HTS_ENGINE_API_PATH}/bin/hts_engine.c)
target_link_libraries(hts_engine HTSEngine)
if(MSVC)
    target_link_libraries(hts_engine winmm)
elseif(CYGWIN)
    target_link_libraries(hts_engine winmm)
elseif(MINGW)
    target_link_libraries(hts_engine winmm)
elseif(UNIX)
    target_link_libraries(hts_engine m)
endif()
set(hts_engine_compile_flags ${hts_c_compile_flags})
set_target_properties(hts_engine PROPERTIES COMPILE_FLAGS ${hts_engine_compile_flags})


# project(open_jtalk C)
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
set(open_jtalk_compile_flags ${openjtalk_compile_flags})
set_target_properties(open_jtalk PROPERTIES COMPILE_FLAGS ${open_jtalk_compile_flags})
if(MSVC)
    target_link_libraries(open_jtalk winmm)
elseif(CYGWIN)
    target_link_libraries(open_jtalk winmm)
elseif(MINGW)
    target_link_libraries(open_jtalk winmm)
elseif(APPLE)
    target_link_libraries(open_jtalk iconv)
elseif(UNIX)
    #target_link_libraries(open_jtalk)
endif()


# portaudio subdirectory
add_subdirectory(portaudio)


# project(openjtalk C)
include_directories(${OPEN_JTALK_PATH}/lib)
include_directories(portaudio/include)
if(WIN32 AND MSVC)
    if(CMAKE_CL_64)
        set(portaudio_lib portaudio_static_x64)
    else()
        set(portaudio_lib portaudio_static_x86)
    endif()
else()
    set(portaudio_lib portaudio)
endif()
add_library(openjtalk STATIC
    ${OPEN_JTALK_PATH}/lib/openjtalk.c
)
set_target_properties(openjtalk PROPERTIES COMPILE_FLAGS ${openjtalk_compile_flags})
if(UNIX AND NOT MINGW AND NOT CYGWIN)
    set(PORTAUDIO portaudio)
else()
    set(PORTAUDIO portaudio_static)
endif()
target_link_libraries(openjtalk
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
    ${PORTAUDIO}
)
if(MSVC)
    target_link_libraries(openjtalk winmm)
elseif(CYGWIN)
    target_link_libraries(openjtalk winmm)
elseif(MINGW)
    target_link_libraries(openjtalk winmm)
endif()


# project(jtalk C)
include_directories(${OPEN_JTALK_PATH}/lib)
include_directories(portaudio/include)
add_library(jtalk SHARED
    ${OPEN_JTALK_PATH}/lib/jtalk.c
    ${OPEN_JTALK_PATH}/lib/jtalk.def
)
set(jtalk_compile_flags ${openjtalk_compile_flags})
set_target_properties(jtalk PROPERTIES COMPILE_FLAGS ${jtalk_compile_flags})
target_link_libraries(jtalk
    openjtalk
)
if(CYGWIN)
    target_link_libraries(jtalk iconv)
    #set_target_properties(jtalk PROPERTIES PREFIX "" SUFFIX ".dll")
elseif(MINGW)
    #target_link_libraries(jtalk iconv)
    set_target_properties(jtalk PROPERTIES PREFIX "" SUFFIX ".dll")
elseif(APPLE)
    target_link_libraries(jtalk iconv)
endif()


install(FILES ${OPEN_JTALK_PATH}/lib/jtalk.h DESTINATION include)
install(TARGETS jtalk
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)


# project(jsay C)
add_executable(jsay
    ${OPEN_JTALK_PATH}/lib/jsay.c
)
set(jsay_compile_flags ${openjtalk_compile_flags})
set_target_properties(jsay PROPERTIES COMPILE_FLAGS ${jsay_compile_flags})
target_link_libraries(jsay
    jtalk
)


# project(jtd_c C)
add_executable(jtd_c
    ${OPEN_JTALK_PATH}/lib/jtd_c.c
)
set(jtd_c_compile_flags ${openjtalk_compile_flags})
set_target_properties(jtd_c PROPERTIES COMPILE_FLAGS ${jtd_c_compile_flags})
target_link_libraries(jtd_c
    jtalk
)


#set_target_properties(hts_engine PROPERTIES VERSION ${serial} )
#set_target_properties(HTSEngine PROPERTIES VERSION ${serial} SOVERSION ${soserial})
