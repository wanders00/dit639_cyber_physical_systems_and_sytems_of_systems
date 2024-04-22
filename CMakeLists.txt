cmake_minimum_required(VERSION 3.2)
project(helloworld)
set(CMAKE_CXX_STANDARD 11)
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/helloworld.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/PrimeChecker.cpp)

# gcc flags for test coverage
set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")
# gcc linker flags for test coverage
set(CMAKE_EXE_LINKER_FLAGS "-lgcov")

enable_testing()
add_executable(${PROJECT_NAME}-Runner test/TestPrimeChecker.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/PrimeChecker.cpp)
add_test(NAME ${PROJECT_NAME}-Runner COMMAND ${PROJECT_NAME}-Runner)