function(qt6_ivigenerator target)
    qt6_ivigenerator_generate(${ARGN})
    qt6_ivigenerator_include(${target} ${ARGN})
endfunction()

if(NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    function(qt_ivigenerator)
        qt6_ivigenerator(${ARGV})
    endfunction()
endif()

macro(internal_resolve_ivigenerator_path)
    if (DEFINED QtIvi_BINARY_DIR AND (NOT DEFINED QT_BUILD_STANDALONE_TESTS))
        set (VIRTUALENV ${QtIvi_BINARY_DIR}/src/tools/ivigenerator/qtivi_qface_virtualenv)
        set (GENERATOR_PATH ${QtIvi_SOURCE_DIR}/src/tools/ivigenerator)
        set (IVIGENERATOR_CONFIG ${QtIvi_BINARY_DIR}/src/tools/ivigenerator/.config)
    else()
        set (GENERATOR_PATH ${QTIVI_INSTALL_PREFIX}/${QT6_INSTALL_BINS}/ivigenerator)
        set (VIRTUALENV ${GENERATOR_PATH}/qtivi_qface_virtualenv)
    endif()
endmacro()

function(qt6_ivigenerator_generate)
    internal_resolve_ivigenerator_path()

    if (NOT EXISTS ${VIRTUALENV}/bin/activate AND NOT EXISTS ${VIRTUALENV}/Scripts/activate.bat)
        return()
    endif()

    cmake_parse_arguments(
        PARSE_ARGV 0
        ARG
        "" "QFACE_FORMAT;QFACE_OUTPUT_DIR;QFACE_SOURCES;QFACE_HEADERS_OUTPUT_DIR" "QFACE_ANNOTATIONS;QFACE_IMPORT_PATH"
    )

    if (DEFINED ARG_KEYWORDS_MISSING_VALUES)
        message(FATAL_ERROR "Keywords can't be empty: ${ARG_KEYWORDS_MISSING_VALUES}")
    endif()

    if (NOT DEFINED ARG_QFACE_FORMAT)
        set(ARG_QFACE_FORMAT frontend)
    endif()
    set(QFACE_OUTPUT_DIR ${ARG_QFACE_OUTPUT_DIR})
    if (NOT DEFINED ARG_QFACE_OUTPUT_DIR)
        set(QFACE_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
    endif()
    if (NOT DEFINED ARG_QFACE_SOURCES)
        message(FATAL_ERROR "QFACE_SOURCES can't be empty")
    endif()
    get_filename_component(QFACE_SOURCES "${ARG_QFACE_SOURCES}" REALPATH BASE_DIR)
    get_filename_component(QFACE_SOURCE_DIR "${QFACE_SOURCES}" DIRECTORY)
    get_filename_component(QFACE_BASE_NAME "${QFACE_SOURCES}" NAME_WLE)
    get_filename_component(QFACE_SOURCE_ANNOTATION ${QFACE_SOURCE_DIR}/${QFACE_BASE_NAME}.yaml REALPATH BASE_DIR)

    set(QFACE_TEMPLATE_PWD "${GENERATOR_PATH}/templates/${ARG_QFACE_FORMAT}")
    if(EXISTS ${QFACE_TEMPLATE_PWD})
        set(FORMAT_PATH ${QFACE_TEMPLATE_PWD})
        set(QFACE_FORMAT ${ARG_QFACE_FORMAT})
    else()
        get_filename_component(FORMAT_PATH "${ARG_QFACE_FORMAT}" REALPATH)
        if(EXISTS ${FORMAT_PATH})
            set(QFACE_FORMAT ${FORMAT_PATH})
        endif()
    endif()
    if (NOT DEFINED QFACE_FORMAT)
        message(FATAL_ERROR "Invalid QFACE_FORMAT: Couldn't find the template folder: ${FORMAT_PATH}")
    endif()

    # Register all source files to cause a cmake rerun
    set(GEN_DEPENDENCIES)
    list(APPEND GEN_DEPENDENCIES ${QFACE_SOURCES})
    if (EXISTS ${QFACE_SOURCE_ANNOTATION})
        list(APPEND GEN_DEPENDENCIES ${QFACE_SOURCE_ANNOTATION})
    endif()
    # Also register all files which are part of the current template
    file(GLOB FORMAT_FILES ${FORMAT_PATH}/*)
    list(APPEND GEN_DEPENDENCIES ${FORMAT_FILES})
    list(APPEND GEN_DEPENDENCIES ${FORMAT_PATH}.yaml)
    # Most templates also have a dependency to a common folder
    file(GLOB COMMON_FORMAT_FILES ${GENERATOR_PATH}/templates/*common*/*)
    list(APPEND GEN_DEPENDENCIES ${COMMON_FORMAT_FILES})

    set(GENERATOR_ARGUMENTS --format=${QFACE_FORMAT} --force)
    foreach(ANNOTATION ${ARG_QFACE_ANNOTATIONS})
        get_filename_component(ANNOTATION_PATH "${ANNOTATION}" REALPATH BASE_DIR)
        list(APPEND GENERATOR_ARGUMENTS -A ${ANNOTATION_PATH})
        # Dependency for regeneration
        list(APPEND GEN_DEPENDENCIES ${ANNOTATION_PATH})
    endforeach()

    foreach(IMPORT ${ARG_QFACE_IMPORT_PATH})
        get_filename_component(IMPORT_PATH "${IMPORT}" REALPATH BASE_DIR)
        list(APPEND GENERATOR_ARGUMENTS -I ${IMPORT_PATH})
        # Dependency for regeneration
        file(GLOB QFACE_FILES ${IMPORT_PATH}/*.qface)
        list(APPEND GEN_DEPENDENCIES ${QFACE_FILES})
    endforeach()

    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${GEN_DEPENDENCIES})

    # If the generator was run successfully before
    # Check for the timestamps to determine when to run it again.
    set(RUN_GENERATOR FALSE)
    foreach(DEP ${GEN_DEPENDENCIES})
        if (${DEP} IS_NEWER_THAN ${QFACE_OUTPUT_DIR}/.stamp-ivigenerator)
            set(RUN_GENERATOR TRUE)
            break()
        endif()
    endforeach()

    if (RUN_GENERATOR)
        # TODO How to best unset those again afterwards ?
        # Use cmake -E slee + cmake -E env COMMAND instead ?
        #equals(QMAKE_HOST.os, Windows): ENV = chcp 65001 &&
        if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
            set(PYTHON_EXECUTABLE ${VIRTUALENV}/Scripts/python.exe)
            file(TO_NATIVE_PATH "${VIRTUALENV}" VIRTUALENV)
        else()
            set(PYTHON_EXECUTABLE ${VIRTUALENV}/bin/python)
            set(ENV{LC_ALL} en_US.UTF-8)
            set(ENV{LD_LIBRARY_PATH} ${VIRTUALENV}/bin)
        endif()
        if (DEFINED IVIGENERATOR_CONFIG)
            set(ENV{IVIGENERATOR_CONFIG} ${IVIGENERATOR_CONFIG})
        endif()
        set(ENV{PYTHONHOME} ${VIRTUALENV})
        set(ENV{VIRTUAL_ENV} ${VIRTUALENV})

        message(STATUS "Running ivigenerator for ${QFACE_SOURCES}")
        execute_process(
                        COMMAND ${PYTHON_EXECUTABLE}
                                ${GENERATOR_PATH}/generate.py
                                ${GENERATOR_ARGUMENTS}
                                ${QFACE_SOURCES}
                                ${QFACE_OUTPUT_DIR}
                        RESULT_VARIABLE RET_CODE
                        OUTPUT_VARIABLE GENERATOR_LOG
                        ERROR_VARIABLE GENERATOR_LOG
                        COMMAND_ECHO STDOUT
                       )
        message("${GENERATOR_LOG}")

        # Touch the stamp file if the generator run was successful
        if("${RET_CODE}" EQUAL "0")
            execute_process(COMMAND ${CMAKE_COMMAND} -E touch ${QFACE_OUTPUT_DIR}/.stamp-ivigenerator)
        else()
            message(FATAL_ERROR "Error while running the ivigenerator")
        endif()
    endif()

    # hack for the developer-build to have all headers where the Qt build expects them
    if (DEFINED ARG_QFACE_HEADERS_OUTPUT_DIR)
        file(GLOB HEADER_FILES ${QFACE_OUTPUT_DIR}/*.h)
        file(COPY ${HEADER_FILES} DESTINATION ${ARG_QFACE_HEADERS_OUTPUT_DIR})
    endif()

endfunction()

if(NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    function(qt_ivigenerator_generate)
        qt6_ivigenerator_generate(${ARGV})
    endfunction()
endif()

function(qt6_ivigenerator_include target)
    internal_resolve_ivigenerator_path()

    if (NOT EXISTS ${VIRTUALENV}/bin/activate AND NOT EXISTS ${VIRTUALENV}/Scripts/activate.bat)
        # Create a dummy target instead
        if (NOT TARGET ${target} AND NOT TEST ${target})
            if (DEFINED QtIvi_BINARY_DIR)
                file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
                add_library(${target} ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
            else()
                message(FATAL_ERROR "No virtualenv environment to run the ivigenerator")
            endif()
        endif()
        return()
    endif()

    cmake_parse_arguments(
        PARSE_ARGV 1
        ARG
        "" "QFACE_SOURCES;QFACE_OUTPUT_DIR" ""
    )

    if (DEFINED ARG_KEYWORDS_MISSING_VALUES)
        message(FATAL_ERROR "Keywords can't be empty: ${ARG_KEYWORDS_MISSING_VALUES}")
    endif()

    set(QFACE_OUTPUT_DIR ${ARG_QFACE_OUTPUT_DIR})
    if (NOT DEFINED ARG_QFACE_OUTPUT_DIR)
        set(QFACE_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
    endif()
    if (NOT DEFINED ARG_QFACE_SOURCES)
        message(FATAL_ERROR "QFACE_SOURCES can't be empty")
    endif()
    get_filename_component(QFACE_SOURCES "${ARG_QFACE_SOURCES}" REALPATH BASE_DIR)
    get_filename_component(QFACE_BASE_NAME "${ARG_QFACE_SOURCES}" NAME_WLE)

    set(CURRENT_TARGET "${target}")
    include(${QFACE_OUTPUT_DIR}/${QFACE_BASE_NAME}.cmake)
endfunction()

if(NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    function(qt_ivigenerator_include)
        qt6_ivigenerator_include(${ARGV})
    endfunction()
endif()
