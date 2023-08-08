cmake_minimum_required(VERSION 3.26)
project(CredManger)

add_executable(CredManager credmanager.cpp)
install(TARGETS CredManager)