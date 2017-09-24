cmake_minimum_required(VERSION 2.6)

project(jtalkdll)

set(CMAKE_BUILD_TYPE Release)

set(OPEN_JTALK_PATH "open_jtalk-1.10")
set(HTS_ENGINE_API_PATH "hts_engine_API-1.10")

include_directories(include)

set(DIC_VERSION 102)
set(PACKAGE "open_jtalk")
set(VERSION "1.10")

if (MSVC)
    set(CMAKE_C_FLAGS "/MD /O2 /Ob2 /Oi /Ot /Oy /GT /TC /D CHARSET_UTF_8 /D ASCII_HEADER")
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MD /O2 /Ob2 /Oi /Ot /Oy /GT /TP /EHsc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D HAVE_WINDOWS_H")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D ASCII_HEADER")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D DIC_VERSION=${DIC_VERSION}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D MECAB_USE_UTF8_ONLY")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D CHARSET_UTF_8")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D MECAB_WITHOUT_MUTEX_LOCK")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D MECAB_DEFAULT_RC=\"dummy\"")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D MECAB_DEFAULT_CHARSET=\"UTF-8\"")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D PACKAGE=\"\\\"${PACKAGE}\\\"\"")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D VERSION=\"\\\"${VERSION}\\\"\"")
    set(CMAKE_L_FLAGS "/LTCG")
else ()
    set(CMAKE_C_FLAGS "-O2 -Wall -g -finput-charset=UTF-8 -fexec-charset=UTF-8 -D CHARSET_UTF_8 -D ASCII_HEADER")
    if (${CMAKE_SYSTEM_NAME} MATCHES "Cygwin")
        set(CMAKE_C_FLAGS "-std=gnu11 ${CMAKE_C_FLAGS} -D __CYGWIN__")
        set(CMAKE_CXX_FLAGS "-std=gnu++11 -D__CYGWIN__")
    else ()
        set(CMAKE_C_FLAGS "-std=c11 ${CMAKE_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "")
    endif ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wno-deprecated")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finput-charset=UTF-8 -fexec-charset=UTF-8")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D HAVE_CONFIG_H  -D HAVE_UNISTD_H")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D DIC_VERSION=${DIC_VERSION}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D MECAB_USE_UTF8_ONLY")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D MECAB_DEFAULT_RC=\"$dummt\"")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D MECAB_DEFAULT_CHARSET=\"UTF-8\"")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D PACKAGE=\"\\\"${PACKAGE}\\\"\"")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D VERSION=\"\\\"${VERSION}\\\"\"")
    set(CMAKE_L_FLAGS "")
endif ()

set(serial "1.0.0")
set(soserial "1")

#set_target_properties(hts_engine PROPERTIES VERSION ${serial} )
#set_target_properties(HTSEngine PROPERTIES VERSION ${serial} SOVERSION ${soserial})

#install(TARGETS hts_engine DESTINATION bin)
#install(TARGETS HTSEngine LIBRARY DESTINATION lib)
#install(FILES HTS_engine.h DESTINATION include)

project(text2mecab C)
include_directories(${OPEN_JTALK_PATH}/text2mecab)
add_library(text2mecab STATIC
    ${OPEN_JTALK_PATH}/text2mecab/text2mecab.c
)

project(mecab CXX)
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

project(mecab2njd C)
include_directories(${OPEN_JTALK_PATH}/mecab2njd)
add_library(mecab2njd STATIC
    ${OPEN_JTALK_PATH}/mecab2njd/mecab2njd.c
)

project(mecab-dict-index CXX)
include_directories(${OPEN_JTALK_PATH}/mecab/src)
add_executable(mecab-dict-index
    ${OPEN_JTALK_PATH}/mecab/src/mecab-dict-index.cpp
)
target_link_libraries(mecab-dict-index mecab)
if (UNIX)
    target_link_libraries(mecab-dict-index iconv)
endif ()


project(njd C)
include_directories(${OPEN_JTALK_PATH}/njd)
add_library(njd STATIC
    ${OPEN_JTALK_PATH}/njd/njd.c
    ${OPEN_JTALK_PATH}/njd/njd_node.c
)

project(njd_set_pronunciation C)
include_directories(${OPEN_JTALK_PATH}/njd_set_pronunciation)
add_library(njd_set_pronunciation STATIC
    ${OPEN_JTALK_PATH}/njd_set_pronunciation/njd_set_pronunciation.c
)

project(njd_set_digit C)
include_directories(${OPEN_JTALK_PATH}/njd_set_digit)
add_library(njd_set_digit STATIC
    ${OPEN_JTALK_PATH}/njd_set_digit/njd_set_digit.c
)

project(njd_set_accent_phrase C)
include_directories(${OPEN_JTALK_PATH}/njd_set_accent_phrase)
add_library(njd_set_accent_phrase STATIC
    ${OPEN_JTALK_PATH}/njd_set_accent_phrase/njd_set_accent_phrase.c
)

project(njd_set_accent_type C)
include_directories(${OPEN_JTALK_PATH}/njd_set_accent_type)
add_library(njd_set_accent_type STATIC
    ${OPEN_JTALK_PATH}/njd_set_accent_type/njd_set_accent_type.c
)

project(njd_set_unvoiced_vowel C)
include_directories(${OPEN_JTALK_PATH}/njd_set_unvoiced_vowel)
add_library(njd_set_unvoiced_vowel STATIC
    ${OPEN_JTALK_PATH}/njd_set_unvoiced_vowel/njd_set_unvoiced_vowel.c
)

project(njd_set_long_vowel C)
include_directories(${OPEN_JTALK_PATH}/njd_set_long_vowel)
add_library(njd_set_long_vowel STATIC
    ${OPEN_JTALK_PATH}/njd_set_long_vowel/njd_set_long_vowel.c
)

project(njd2jpcommon C)
include_directories(${OPEN_JTALK_PATH}/njd2jpcommon)
add_library(njd2jpcommon STATIC
    ${OPEN_JTALK_PATH}/njd2jpcommon/njd2jpcommon.c
)

project(jpcommon C)
include_directories(${OPEN_JTALK_PATH}/jpcommon)
add_library(jpcommon STATIC
    ${OPEN_JTALK_PATH}/jpcommon/jpcommon.c
    ${OPEN_JTALK_PATH}/jpcommon/jpcommon_node.c
    ${OPEN_JTALK_PATH}/jpcommon/jpcommon_label.c
)

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

add_executable(hts_engine ${HTS_ENGINE_API_PATH}/bin/hts_engine.c)
target_link_libraries(hts_engine HTSEngine)
if (MSVC)
    target_link_libraries(hts_engine winmm)
elseif (UNIX)
    target_link_libraries(hts_engine m)
endif ()

project(open_jtalk C)
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
if (MSVC)
    target_link_libraries(open_jtalk winmm)
elseif (UNIX)
    target_link_libraries(open_jtalk iconv m)
endif ()


#project(lib C)
#include_directories(lib)
#add_library(lib STATIC
#    ${OPEN_JTALK_PATH}/lib/lib.c
#)

# mecab-naist-jdic
