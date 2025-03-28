cmake_minimum_required(VERSION 3.25)

project(z_generals LANGUAGES C CXX)

# Do we want to build extra SDK stuff or just the game binary?
option(GENZH_BUILD_ZEROHOUR_TOOLS "Build tools for Zero Hour" OFF)
add_feature_info(ZeroHourTools GENZH_BUILD_ZEROHOUR_TOOLS "Build Zero Hour Mod Tools")

# Set where the build results will end up
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Add main build targets
add_subdirectory(Code)

# If we are building on windows for windows, try and get the game install path from the registry.
if("${CMAKE_HOST_SYSTEM}" MATCHES "Windows" AND "${CMAKE_SYSTEM}" MATCHES "Windows")
    if(NOT GENZH_ZEROHOUR_INSTALL_PREFIX)
        get_filename_component(GENZH_ZEROHOUR_INSTALL_PREFIX  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Electronic Arts\\EA Games\\Command and Conquer Generals Zero Hour;InstallPath]" ABSOLUTE CACHE)
    endif()

    # Check the "First Decade" registry path
    if(NOT GENZH_ZEROHOUR_INSTALL_PREFIX OR "${GENZH_ZEROHOUR_INSTALL_PREFIX}" STREQUAL "/registry")
        get_filename_component(GENZH_ZEROHOUR_INSTALL_PREFIX  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Electronic Arts\\EA Games\\Command and Conquer The First Decade;zh_folder]" ABSOLUTE CACHE)
    endif()

    # Check the Steam registry path
    if(NOT GENZH_ZEROHOUR_INSTALL_PREFIX OR "${GENZH_ZEROHOUR_INSTALL_PREFIX}" STREQUAL "/registry")
        get_filename_component(GENZH_ZEROHOUR_INSTALL_PREFIX  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Electronic Arts\\EA Games\\ZeroHour;installPath]" ABSOLUTE CACHE)
    endif()
endif()

if(GENZH_ZEROHOUR_INSTALL_PREFIX AND NOT "${GENZH_ZEROHOUR_INSTALL_PREFIX}" STREQUAL "/registry")
    install(TARGETS z_generals RUNTIME DESTINATION "${GENZH_ZEROHOUR_INSTALL_PREFIX}")
    install(FILES $<TARGET_PDB_FILE:z_generals> DESTINATION "${GENZH_ZEROHOUR_INSTALL_PREFIX}" OPTIONAL)
    
    if(TARGET z_worldbuilder)
        install(TARGETS z_worldbuilder RUNTIME DESTINATION "${GENZH_ZEROHOUR_INSTALL_PREFIX}" OPTIONAL)
        install(FILES $<TARGET_PDB_FILE:z_worldbuilder> DESTINATION "${GENZH_ZEROHOUR_INSTALL_PREFIX}" OPTIONAL)
    endif()
endif()

feature_summary(WHAT ENABLED_FEATURES DESCRIPTION "Enabled features:")
feature_summary(WHAT DISABLED_FEATURES DESCRIPTION "Disabled features:")
