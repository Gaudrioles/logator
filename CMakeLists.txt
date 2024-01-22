CMAKE_MINIMUM_REQUIRED(VERSION 3.7)

PROJECT(logator)

IF(MSVC)
    MESSAGE(STATUS"Microsoft Visual C")
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

IF(MINGW)
    MESSAGE(STATUS "MinGW")
    SET(CMAKE_C_FLAGS  "-Wall -Werror -pedantic-errors -O3 -march=znver1")
    SET(CMAKE_EXE_LINKER_FLAGS  "-lucrt")
ENDIF()

IF(UNIX)
    MESSAGE(STATUS "Linux")
    SET(CMAKE_C_FLAGS  "-Wall -Werror -pedantic-errors -O3 -march=znver1")
ENDIF()

ADD_EXECUTABLE(${PROJECT_NAME} main.c log.c fonction.c fichier.c message.c Resource.rc)