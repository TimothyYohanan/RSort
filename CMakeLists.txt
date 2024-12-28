cmake_minimum_required(VERSION 3.26.0)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

project(RSort C CXX)


################## Compile-time options ##################
set(DEBUG_BUILD            ON                           )
set(USE_ADDRESS_SANITIZER  ON                           )
set(PROJECT_BINARY_DIR     "${CMAKE_SOURCE_DIR}/build"  )
set(CPU_THREADS            18                           )
################ end Compile-time options ################


set(CMAKE_CONFIGURATION_TYPES "DEBUG;RELEASE" CACHE STRING "Available build configurations" FORCE)

if(DEBUG_BUILD) 
    set(CMAKE_BUILD_TYPE DEBUG CACHE STRING "" FORCE)
    add_compile_options(-Wall)
    add_compile_options(-Wextra)
    add_compile_options(-Wformat-security)
    add_compile_options(-Wconversion)
    if (USE_ADDRESS_SANITIZER)
        add_compile_options(-fsanitize=address)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
        set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
    endif()
    add_compile_options(-O0)
    add_compile_options(-pedantic)
else()
    set(CMAKE_BUILD_TYPE RELEASE CACHE STRING "" FORCE)
    add_compile_options(-O3)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/library")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/library")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_DATA_DIRECTORY "${PROJECT_BINARY_DIR}/data")

if(USE_ADDRESS_SANITIZER)
find_library(asan_LIBRARY NAMES asan PATHS "/usr/lib/gcc/x86_64-redhat-linux/11")
endif()

#
# RSort Test
#
add_executable(RSort tests/test_rsort.c)

target_sources(RSort
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src/rsort.c
        ${PROJECT_SOURCE_DIR}/src/array_util.c
    )

target_include_directories(RSort 
    PUBLIC 
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/src/
        ${PROJECT_SOURCE_DIR}/tests/
    )

if(USE_ADDRESS_SANITIZER)
target_link_libraries(RSort  
    PUBLIC 
        asan
)
endif()

install(
    TARGETS               RSort  
    EXPORT                ${PACKAGE_TARGETS}
    LIBRARY DESTINATION   ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION   ${INSTALL_ARCHIVE_DIR}
    RUNTIME DESTINATION   ${INSTALL_RUNTIME_DIR}
    INCLUDES DESTINATION  ${INSTALL_INCLUDE_DIR}
)


#
# Array Utils Test
#
add_executable(ArrayUtil tests/test_array_util.c)

target_sources(ArrayUtil
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src/array_util.c
    )

target_include_directories(ArrayUtil 
    PUBLIC 
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/src/
        ${PROJECT_SOURCE_DIR}/tests/
    )

if(USE_ADDRESS_SANITIZER)
target_link_libraries(ArrayUtil  
    PUBLIC 
        asan
)
endif()

install(
    TARGETS               ArrayUtil  
    EXPORT                ${PACKAGE_TARGETS}
    LIBRARY DESTINATION   ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION   ${INSTALL_ARCHIVE_DIR}
    RUNTIME DESTINATION   ${INSTALL_RUNTIME_DIR}
    INCLUDES DESTINATION  ${INSTALL_INCLUDE_DIR}
)
