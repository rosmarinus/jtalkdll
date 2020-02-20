cmake_minimum_required(VERSION 3.10)

# command line option
# -Dtarget_arch=(i686|x86_64) : cygwin
#    default value = uname -m
# -Dbuild_jtalkcom=true : msvc
# -Dinstall_open_jtalk=true : install open_jtalk, hts_engine, mecab-dict-index


project(jtalkdll)
set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_VERBOSE_MAKEFILE TRUE)
#set(target_arch x86_64)
#set(build_jtalkcom TRUE)
set(install_open_jtalk TRUE)


### archive file version
set(HTS_ENGINE_API_VERSION "1.10")
set(OPEN_JTALK_VERSION "1.11")
set(OPEN_JTALK_DIC_VERSION "1.11")


### archive file name
set(HTS_ENGINE_API_NAME "hts_engine_API-${HTS_ENGINE_API_VERSION}")
set(OPEN_JTALK_NAME "open_jtalk-${OPEN_JTALK_VERSION}")
set(OPEN_JTALK_DIC_NAME "open_jtalk_dic_utf_8-${OPEN_JTALK_DIC_VERSION}")
set(HTS_VOICE "voice")


### source directory
set(HTS_ENGINE_API_PATH "${CMAKE_SOURCE_DIR}/${HTS_ENGINE_API_NAME}")
set(OPEN_JTALK_PATH "${CMAKE_SOURCE_DIR}/${OPEN_JTALK_NAME}")
set(OPEN_JTALK_DIC_PATH "${CMAKE_SOURCE_DIR}/${OPEN_JTALK_DIC_NAME}")
set(HTS_VOICE_PATH "${CMAKE_SOURCE_DIR}/${HTS_VOICE}")
set(JTALK_PATH "${CMAKE_SOURCE_DIR}/jtalk")


### directory name
set(DICDATADIR "${OPEN_JTALK_PATH}/mecab-naist-jdic")
set(DICBINDIR ${CMAKE_CURRENT_BINARY_DIR}/dic)


### sub folder name install
set(FOLDER_INSTALL_BIN bin)
set(FOLDER_INSTALL_LIB lib)
set(FOLDER_INSTALL_DIC dic_utf_8)
set(FOLDER_INSTALL_VOICE voice)


### set mingw compiler
if(MINGW)
    set(target_arch "$ENV{MINGW_CHOST}")
    set(mingw_prefix "$ENV{MINGW_PREFIX}")
    set(CMAKE_FIND_ROOT_PATH "${mingw_prefix}")
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
    set(CMAKE_RANLIB ranlib)
    set(CMAKE_AS as)
    set(CMAKE_AR ar)
    set(CMAKE_STRIP strip)
    set(mingw_build 1)
elseif(CYGWIN)
    if(NOT target_arch)
        if(CMAKE_UNAME)
            exec_program(uname ARGS -m OUTPUT_VARIABLE target_arch)
        endif()
    endif()
    if(target_arch)
        set(target_arch "${target_arch}-w64-mingw32")
        set(CMAKE_FIND_ROOT_PATH "/usr/${target_arch}")
        set(CMAKE_C_COMPILER ${target_arch}-gcc)
        set(CMAKE_CXX_COMPILER ${target_arch}-g++)
        set(CMAKE_RANLIB ${target_arch}-ranlib)
        set(CMAKE_AS ${target_arch}-as)
        set(CMAKE_AR ${target_arch}-ar)
        set(CMAKE_STRIP ${target_arch}-strip)
        set(mingw_build 1)
    endif()
endif()
if(mingw_build)
    set(CMAKE_SYSTEM_NAME Windows)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

    message("-- Target: ${target_arch}")
    message("-- CC: ${CMAKE_C_COMPILER}")
    message("-- CXX: ${CMAKE_CXX_COMPILER}")
    message("-- AS: ${CMAKE_AS}")
    message("-- AR: ${CMAKE_AR}")
    message("-- RANLIB: ${CMAKE_RANLIB}")
    message("-- STRIP:  ${CMAKE_STRIP}")
    message("-- Find Root Path: ${CMAKE_FIND_ROOT_PATH}")
endif()


### set install directory
if(WIN32 OR mingw_build)
    set(PROJECT_FOLDER "open_jtalk")
    set(INSTALL_BASE_PATH "C:/")
else()
    set(PROJECT_FOLDER "OpenJTalk")
    set(INSTALL_BASE_PATH "/usr/local/")
endif()


### set install base path
if(MSVC AND WIN32)
    set(BASE_PATH "C:/")
elseif(MINGW)
    set(BASE_PATH "/c/")
elseif(CYGWIN)
    set(BASE_PATH "/cygdrive/c/")
else()
    set(BASE_PATH "/usr/local/")
endif()


### set install prefix
if(MSVC AND WIN32)
    set(CMAKE_INSTALL_PREFIX ${BASE_PATH}${PROJECT_FOLDER})
elseif(MINGW)
    set(CMAKE_INSTALL_PREFIX ${BASE_PATH}${PROJECT_FOLDER})
    #set(CMAKE_INSTALL_PREFIX "${mingw_prefix}")
elseif(CYGWIN)
    set(CMAKE_INSTALL_PREFIX ${BASE_PATH}${PROJECT_FOLDER})
    #set(CMAKE_INSTALL_PREFIX "/usr/${target_arch}")
else()
    set(CMAKE_INSTALL_PREFIX ${BASE_PATH})
endif()


### data install prefix
set(MACRO_DATA_INSTALL_PREFIX ${INSTALL_BASE_PATH}${PROJECT_FOLDER})
set(DATA_INSTALL_PREFIX ${BASE_PATH}${PROJECT_FOLDER})


### mecab const
set(PACKAGE "open_jtalk")
set(VERSION "1.11")
set(DIC_VERSION 102)
set(PACKAGE_NAME,"${PACKAGE}")
set(PACKAGE_VERSION,"${VERSION}")
set(PACKAGE_STRING,$"{PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_TARNAME,"${PACKAGE_NAME}")
set(PACKAGE_URL,"")
set(PACKAGE_BUGREPORT,"")


### mecab definition flags
include(CheckFunctionExists)
include(CheckIncludeFiles)
include(TestBigEndian)

set(def_flags "")
check_function_exists(getenv HAVE_GETENV)
if(${HAVE_GETENV})
    set(def_flags "${def_flags} -DHAVE_GETENV")
endif()

check_function_exists(mmap HAVE_MMAP)
if(${HAVE_MMAP})
    set(def_flags "${def_flags} -DHAVE_MMAP")
endif()

check_include_files (dirent.h HAVE_DIRENT_H)
if(${HAVE_DIRENT_H})
    set(def_flags "${def_flags} -DHAVE_DIRENT_H")
endif()

check_include_files (fcntl.h HAVE_FCNTL_H)
if(${HAVE_FCNTL_H})
    set(def_flags "${def_flags} -DHAVE_FCNTL_H")
endif()

check_include_files(stdint.h HAVE_STDINT_H)
if(${HAVE_STDINT_H})
    set(def_flags "${def_flags} -DHAVE_STDINT_H")
endif()

check_include_files(string.h HAVE_STRING_H)
if(${HAVE_STRING_H})
    set(def_flags "${def_flags} -DHAVE_STRING_H")
endif()

check_include_files(sys/mman.h HAVE_SYS_MMAN_H)
if(${HAVE_SYS_MMAN_H})
    set(def_flags "${def_flags} -DHAVE_SYS_MMAN_H")
endif()

check_include_files(sys/stat.h HAVE_SYS_STAT_H)
if(${HAVE_SYS_STAT_H})
    set(def_flags "${def_flags} -DHAVE_SYS_STAT_H")
endif()

check_include_files(sys/types.h HAVE_SYS_TYPES_H)
if(${HAVE_SYS_TYPES_H})
    set(def_flags "${def_flags} -DHAVE_SYS_TYPES_H")
endif()

check_include_files(unistd.h HAVE_UNISTD_H)
if(${HAVE_UNISTD_H})
    set(def_flags "${def_flags} -DHAVE_UNISTD_H")
endif()

check_include_files(windows.h HAVE_WINDOWS_H)
if(${HAVE_WINDOWS_H})
    set(def_flags "${def_flags} -DHAVE_WINDOWS_H")
endif()

test_big_endian(WORDS_BIGENDIAN)
if(${WORDS_BIGENDIAN})
    set(def_flags "${def_flags} -DWORDS_BIGENDIAN")
endif()

set(def_flags "${def_flags} -DDIC_VERSION=${DIC_VERSION}")
set(def_flags "${def_flags} -DMECAB_USE_UTF8_ONLY")
set(def_flags "${def_flags} -DMECAB_DEFAULT_RC=\"dummy\"")
set(def_flags "${def_flags} -DPACKAGE=\"\\\"${PACKAGE}\\\"\"")
set(def_flags "${def_flags} -DVERSION=\"\\\"${VERSION}\\\"\"")


### get git revision
execute_process(
    COMMAND git show --format=%h -s
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_REVISION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)


### jtalk version
set(JTALK_MAJOR_NUM 0)
set(JTALK_MINOR_NUM 0)
file(READ ${JTALK_PATH}/build.version JTALK_BUILD_NUM OFFSET 6)
string(REGEX MATCH [0-9]+ JTALK_BUILD_NUM ${JTALK_BUILD_NUM})
set(JTALK_SERIAL "${JTALK_MAJOR_NUM}.${JTALK_MINOR_NUM}")
#set(JTALK_SERIAL "${JTALK_MAJOR_NUM}.${JTALK_MINOR_NUM}.${JTALK_BUILD_NUM}")
set(JTALK_SOSERIAL "${JTALK_MAJOR_NUM}")
message("-- Serial Number: ${JTALK_SERIAL}")
message("-- Git Revision : ${GIT_REVISION}")


# look for mecab-dict-index.exe
find_program(MECAB_DICT_INDEX_EXISTS "mecab-dict-index.exe")
if(MECAB_DICT_INDEX_EXISTS)
message("MECAB_DICT_INDEX_EXISTS")
else()
message("MECAB_DICT_INDEX_NOT_EXISTS")
endif()


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


### look for libportaudio
set(EXIST_PORTAUDIO 0)
if(UNIX AND NOT APPLE)
    if(NOT mingw_build)
        find_library(PORTAUDIO_LIBRARY NAMES portaudio)
        if(PORTAUDIO_LIBRARY)
            set(EXIST_PORTAUDIO 1)
        endif()
    endif()
endif()
if(mingw_build)
    #find_library(PORTAUDIO_LIBRARY NAMES portaudio PATHS ${CMAKE_FIND_ROOT_PATH} ONLY_CMAKE_FIND_ROOT_PATH)
    find_library(PORTAUDIO_LIBRARY NAMES portaudio ONLY_CMAKE_FIND_ROOT_PATH)
    if(PORTAUDIO_LIBRARY)
        set(EXIST_PORTAUDIO 1)
    else()
        message(FATAL_ERROR "libportaudio.a not found\nPlease run the script 'build_portaudio_mingw'.")
    endif()
endif()
if(EXIST_PORTAUDIO)
    message("-- Portaudio Library found: ${PORTAUDIO_LIBRARY}")
else()
    message("-- Portaudio Library not found")
endif()


### portaudio subdirectory
if(NOT EXIST_PORTAUDIO)
    add_subdirectory(portaudio)
endif()


### c/cxx flags
if(MSVC AND WIN32)
    set(CMAKE_C_FLAGS  "-MD -O2 -Ob2 -Oi -Ot -Oy -GT")
    set(CMAKE_CXX_FLAGS "-MD -O2 -Ob2 -Oi -Ot -Oy -GT")
    set(CMAKE_C_FLAGS_RELEASE "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
    set(CMAKE_C_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_DEBUG "")

    ### openjtalk c flags
    set(openjtalk_compile_flags "-TC -DCHARSET_UTF_8 -DASCII_HEADER")

    ### hts_engine_API c flgas
    set(hts_c_compile_flags "-TC -DCHARSET_UTF_8 -DASCII_HEADER")

    ### jtalk c flgas
    set(jtalk_c_compile_flags "-TC")
    set(jtalk_c_compile_flags "${jtalk_c_compile_flags} -DVER_MAJOR=${JTALK_MAJOR_NUM}")
    set(jtalk_c_compile_flags "${jtalk_c_compile_flags} -DVER_MINOR=${JTALK_MINOR_NUM}")
    set(jtalk_c_compile_flags "${jtalk_c_compile_flags} -DVER_BUILD=${JTALK_BUILD_NUM}")
    set(jtalk_c_compile_flags "${jtalk_c_compile_flags} -DGIT_REV=\"\\\"${GIT_REVISION}\\\"\"")
    set(jtalk_c_compile_flags "${jtalk_c_compile_flags} -DINSTALL_PATH=\"\\\"${MACRO_DATA_INSTALL_PREFIX}\\\"\"")

    ### jtalkcom c++/cli flgas
    if(build_jtalkcom)
        set(jtalkcom_cli_compile_flags "-TP -clr -EHa")
        if(CMAKE_CL_64)
            set(SNKFILE ${JTALK_PATH}/JTalkCOMx64Pub.snk)
        else()
            set(SNKFILE ${JTALK_PATH}/JTalkCOMx86Pub.snk)
        endif()
        if(EXISTS ${SNKFILE})
            message("-- SNK File: ${SNKFILE}")
            set(jtalkcom_cli_link_flags "-DELAYSIGN -KEYFILE:${SNKFILE}")
        endif()
    endif()
    
    ### mecab cpp flgas
    set(def_flags "${def_flags} -DHAVE_WINDOWS_H")
    set(def_flags "${def_flags} -DASCII_HEADER")
    set(def_flags "${def_flags} -DCHARSET_UTF_8")
    set(def_flags "${def_flags} -DMECAB_WITHOUT_MUTEX_LOCK")

    set(cxx_compile_flags "-TP")
    set(cxx_compile_flags "${cxx_compile_flags} ${def_flags}")
    set(mecab_cxx_compile_flags_with_EHa "-EHa ${cxx_compile_flags}")
    set(mecab_cxx_compile_flags "-EHsc ${cxx_compile_flags}")

    set(CMAKE_L_FLAGS "/LTCG")

else()
    ### openjtalk c flags
    set(openjtalk_compile_flags "-O2 -Wall -g -finput-charset=UTF-8 -fexec-charset=UTF-8 -DCHARSET_UTF_8 -DASCII_HEADER")

    ### hts_engine_API c flgas
    set(hts_c_compile_flags "-O2 -Wall -g -finput-charset=UTF-8 -fexec-charset=UTF-8 -DCHARSET_UTF_8 -DASCII_HEADER")

    ### jtalk c flgas
    set(jtalk_c_compile_flags "-O2 -Wall -g")
    set(jtalk_c_compile_flags "${jtalk_c_compile_flags} -DVER_MAJOR=${JTALK_MAJOR_NUM}")
    set(jtalk_c_compile_flags "${jtalk_c_compile_flags} -DVER_MINOR=${JTALK_MINOR_NUM}")
    set(jtalk_c_compile_flags "${jtalk_c_compile_flags} -DVER_BUILD=${JTALK_BUILD_NUM}")
    set(jtalk_c_compile_flags "${jtalk_c_compile_flags} -DGIT_REV=\"\\\"${GIT_REVISION}\\\"\"")
    set(jtalk_c_compile_flags "${jtalk_c_compile_flags} -DINSTALL_PATH=\"\\\"${MACRO_DATA_INSTALL_PREFIX}\\\"\"")

    ### mecab cpp flgas
    set(def_flags "${def_flags} -DHAVE_CONFIG_H  -DHAVE_UNISTD_H")

    set(mecab_cxx_compile_flags "")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -O3 -Wall -Wno-deprecated")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -finput-charset=UTF-8 -fexec-charset=UTF-8")
    set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} ${def_flags}")

    ### mingw
    if(mingw_build)
        set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
        set(CMAKE_SHARED_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

    ### cygwin without mingw compiler
    elseif(CYGWIN)
        set(openjtalk_compile_flags "${openjtalk_compile_flags} -std=gnu11")
        set(hts_c_compile_flags "${hts_c_compile_flags} -std=gnu11")
        set(jtalk_c_compile_flags "${jtalk_c_compile_flags} -std=gnu11")
        set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -std=gnu++11")

    ### linux
    else()
        set(openjtalk_compile_flags "${openjtalk_compile_flags} -std=gnu11 -fPIC")
        set(hts_c_compile_flags "${hts_c_compile_flags} -std=gnu11 -fPIC")
        set(jtalk_c_compile_flags "${jtalk_c_compile_flags} -std=gnu11 -fPIC")
        set(mecab_cxx_compile_flags "${mecab_cxx_compile_flags} -std=gnu++11 -fPIC")
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
if(mingw_build OR (MSVC AND WIN32) OR CYGWIN)
    target_link_libraries(hts_engine winmm)
elseif(UNIX)
    target_link_libraries(hts_engine m)
endif()
set_target_properties(hts_engine PROPERTIES COMPILE_FLAGS ${hts_c_compile_flags})
if(mingw_build)
    set_target_properties(hts_engine PROPERTIES PREFIX "" SUFFIX ".exe")
endif()
if(install_open_jtalk)
    install(TARGETS hts_engine RUNTIME DESTINATION ${FOLDER_INSTALL_BIN})
endif()

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
if(install_open_jtalk)
    install(TARGETS mecab-dict-index RUNTIME DESTINATION ${FOLDER_INSTALL_BIN})
endif()

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
if(install_open_jtalk)
    install(TARGETS open_jtalk RUNTIME DESTINATION ${FOLDER_INSTALL_BIN})
endif()


### openjtalk
include_directories(${OPEN_JTALK_PATH}/bin)
include_directories(portaudio/include)
add_library(openjtalk STATIC
    ${OPEN_JTALK_PATH}/bin/openjtalk.c
)
set_target_properties(openjtalk PROPERTIES COMPILE_FLAGS ${openjtalk_compile_flags})
if(MSVC AND WIN32 OR APPLE)
    set(PORTAUDIO portaudio_static)
elseif(mingw_build)
    set(PORTAUDIO ${PORTAUDIO_LIBRARY})
else()
    set(PORTAUDIO portaudio)
endif()
target_link_libraries(openjtalk
    text2mecab
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
if(mingw_build OR (MSVC AND WIN32) OR  CYGWIN)
    target_link_libraries(openjtalk winmm)
endif()


### main project
### jtalk
include_directories(${JTALK_PATH})
include_directories(portaudio/include)
add_library(jtalk SHARED
    ${JTALK_PATH}/jtalk.c
    ${JTALK_PATH}/jtalk.def
)
set_target_properties(jtalk PROPERTIES COMPILE_FLAGS ${jtalk_c_compile_flags})
target_link_libraries(jtalk
    openjtalk
    mecab
)
if(MSVC AND WIN32)
    set_target_properties(jtalk PROPERTIES PREFIX "" SUFFIX ".dll")
    if(CMAKE_CL_64)
        set_target_properties(jtalk PROPERTIES IMPORT_PREFIX "" IMPORT_SUFFIX "x64.lib")
    else()
        set_target_properties(jtalk PROPERTIES IMPORT_PREFIX "" IMPORT_SUFFIX "x86.lib")
    endif()
elseif(mingw_build)
    #target_link_libraries(jtalk iconv)
    set_target_properties(jtalk PROPERTIES PREFIX "" SUFFIX ".dll")
    set_target_properties(jtalk PROPERTIES IMPORT_PREFIX "lib")
    set_target_properties(jtalk PROPERTIES IMPORT_SUFFIX ".lib")
elseif(CYGWIN)
    target_link_libraries(jtalk iconv)
    set_target_properties(jtalk PROPERTIES PREFIX "" SUFFIX ".dll")
elseif(UNIX AND ENABLE_LICONV)
    target_link_libraries(jtalk iconv)
elseif(APPLE)
    target_link_libraries(jtalk iconv)
endif()
if(NOT mingw_build)
    set_target_properties(jtalk PROPERTIES VERSION ${JTALK_SERIAL} SOVERSION ${JTALK_SOSERIAL})
endif()
if(mingw_build)
    add_custom_command(TARGET jtalk
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${JTALK_PATH}/${target_arch}
        COMMAND ${CMAKE_COMMAND} -E copy jtalk.dll ${JTALK_PATH}/${target_arch}/.
        COMMAND ${CMAKE_STRIP} ${JTALK_PATH}/${target_arch}/jtalk.dll
    )
endif()
install(FILES ${JTALK_PATH}/jtalk.h DESTINATION include)
if(mingw_build)
    install(TARGETS jtalk
        LIBRARY DESTINATION ${FOLDER_INSTALL_LIB}
        RUNTIME DESTINATION ${FOLDER_INSTALL_BIN}
    )
    #install(FILES ${JTALK_PATH}/jtalk.h DESTINATION ${MINGW_INSTALL_PREFIX}/include)
    #install(FILES ${CMAKE_CURRENT_BINARY_DIR}/jtalk.dll DESTINATION ${MINGW_INSTALL_PREFIX}/${FOLDER_INSTALL_BIN})
else()
    install(TARGETS jtalk
        ARCHIVE DESTINATION ${FOLDER_INSTALL_LIB}
        LIBRARY DESTINATION ${FOLDER_INSTALL_LIB}
        RUNTIME DESTINATION ${FOLDER_INSTALL_BIN}
    )
endif()


### build JTalkCOM.dll
if(build_jtalkcom AND MSVC AND WIN32)

    ### mecab_eha CXX
    include_directories(${OPEN_JTALK_PATH}/mecab)
    include_directories(${OPEN_JTALK_PATH}/mecab/src)
    add_library(mecab_eha STATIC
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
    set_target_properties(mecab_eha PROPERTIES COMPILE_FLAGS ${mecab_cxx_compile_flags_with_EHa})

    ###project(JTalkCOM CXX)
    include_directories(${JTALK_PATH})
    include_directories(portaudio/include)
    add_library(jtalk_static STATIC
        ${JTALK_PATH}/jtalk.c
        ${JTALK_PATH}/jtalk.def
    )
    set_target_properties(jtalk_static PROPERTIES COMPILE_FLAGS "${jtalk_c_compile_flags} /DDISABLE_JTALK_DLLMAIN")
    add_library(JTalkCOM SHARED
        ${JTALK_PATH}/JTalkCOM.cpp
    )
    set_target_properties(JTalkCOM PROPERTIES COMPILE_FLAGS "${jtalkcom_cli_compile_flags}")
    set_target_properties(JTalkCOM PROPERTIES LINK_FLAGS "${jtalkcom_cli_link_flags}")
    target_link_libraries(JTalkCOM jtalk_static openjtalk mecab_eha)
    if(CMAKE_CL_64)
        set_target_properties(JTalkCOM PROPERTIES OUTPUT_NAME JTalkCOMx64)
    else()
        set_target_properties(JTalkCOM PROPERTIES OUTPUT_NAME JTalkCOMx86)
    endif()
    set_target_properties(JTalkCOM PROPERTIES VERSION ${JTALK_SERIAL} SOVERSION ${JTALK_SOSERIAL})
    install(TARGETS JTalkCOM
        RUNTIME DESTINATION ${FOLDER_INSTALL_BIN}
        #ARCHIVE DESTINATION ${FOLDER_INSTALL_LIB}
        #LIBRARY DESTINATION ${FOLDER_INSTALL_LIB}
    )
    install(FILES ${JTALK_PATH}/regist_jtalkcom.bat   DESTINATION ${FOLDER_INSTALL_BIN})
    install(FILES ${JTALK_PATH}/unregist_jtalkcom.bat DESTINATION ${FOLDER_INSTALL_BIN})
endif()


### c sample
### jsay
add_executable(jsay
    ${JTALK_PATH}/jsay.c
)
set_target_properties(jsay PROPERTIES COMPILE_FLAGS ${jtalk_c_compile_flags})
target_link_libraries(jsay
    jtalk
)
if(UNIX AND ENABLE_LICONV)
    target_link_libraries(jsay iconv)
endif()
if(mingw_build)
    set_target_properties(jsay PROPERTIES PREFIX "" SUFFIX ".exe")
endif()
install(TARGETS jsay RUNTIME DESTINATION ${FOLDER_INSTALL_BIN})


### c sample
### jtd_c
add_executable(jtd_c
    ${JTALK_PATH}/jtd_c.c
)
set_target_properties(jtd_c PROPERTIES COMPILE_FLAGS ${jtalk_c_compile_flags})
target_link_libraries(jtd_c
    jtalk
)
if(UNIX AND ENABLE_LICONV)
    target_link_libraries(jtd_c iconv)
endif()
if(mingw_build)
    set_target_properties(jtd_c PROPERTIES PREFIX "" SUFFIX ".exe")
endif()
install(TARGETS jtd_c RUNTIME DESTINATION ${FOLDER_INSTALL_BIN})


### list of dictionary files
set(copy_list char.bin;matrix.bin;sys.dic;unk.dic;left-id.def;right-id.def;rewrite.def;pos-id.def;COPYING)
set(dic_files "")
foreach(path IN LISTS copy_list)
    list(APPEND dic_files ${DICBINDIR}/${path})
endforeach()

# copy and rename
add_custom_command(
    OUTPUT ${DICBINDIR}/left-id.def;${DICBINDIR}/right-id.def;${DICBINDIR}/rewrite.def;${DICBINDIR}/pos-id.def;${DICBINDIR}/COPYING
    COMMAND ${CMAKE_COMMAND} -E copy ${DICDATADIR}/COPYING ${DICBINDIR}/COPYING
    COMMAND ${CMAKE_COMMAND} -E copy ${DICDATADIR}/naist-jdic.csv ${DICBINDIR}/naist-jdic.csv
    COMMAND ${CMAKE_COMMAND} -E copy ${DICDATADIR}/unidic-csj.csv ${DICBINDIR}/unidic-csj.csv
    COMMAND ${CMAKE_COMMAND} -E copy ${DICDATADIR}/char.def ${DICBINDIR}/char.def
    COMMAND ${CMAKE_COMMAND} -E copy ${DICDATADIR}/feature.def ${DICBINDIR}/feature.def
    COMMAND ${CMAKE_COMMAND} -E copy ${DICDATADIR}/matrix.def ${DICBINDIR}/matrix.def
    COMMAND ${CMAKE_COMMAND} -E copy ${DICDATADIR}/unk.def ${DICBINDIR}/unk.def
    COMMAND ${CMAKE_COMMAND} -E copy ${DICDATADIR}/_left-id.def ${DICBINDIR}/left-id.def
    COMMAND ${CMAKE_COMMAND} -E copy ${DICDATADIR}/_right-id.def ${DICBINDIR}/right-id.def
    COMMAND ${CMAKE_COMMAND} -E copy ${DICDATADIR}/_rewrite.def ${DICBINDIR}/rewrite.def
    COMMAND ${CMAKE_COMMAND} -E copy ${DICDATADIR}/_pos-id.def ${DICBINDIR}/pos-id.def
)

# do mecab-dict-index
add_custom_command(
    OUTPUT ${DICBINDIR}/char.bin ${DICBINDIR}/matrix.bin ${DICBINDIR}/sys.dic ${DICBINDIR}/unk.dic
    DEPENDS mecab-dict-index ${DICBINDIR}/left-id.def ${DICBINDIR}/right-id.def ${DICBINDIR}/rewrite.def ${DICBINDIR}/pos-id.def
    COMMAND "mecab-dict-index" "-d" "${DICBINDIR}" "-o" "${DICBINDIR}" "-f" "UTF-8" "-t" "UTF-8"
)

### target dictionary
add_custom_target (dictionary
    DEPENDS mecab-dict-index
    SOURCES ${DICBINDIR}/char.bin ${DICBINDIR}/matrix.bin ${DICBINDIR}/sys.dic ${DICBINDIR}/unk.dic
    COMMENT "Compiling MeCab dictionary"
)
add_dependencies(openjtalk dictionary)
install(FILES ${dic_files} DESTINATION ${DATA_INSTALL_PREFIX}/${FOLDER_INSTALL_DIC})


### install voices
install(DIRECTORY ${HTS_VOICE_PATH} DESTINATION ${DATA_INSTALL_PREFIX})
