cmake_minimum_required(VERSION 3.16)
project(Ticket_System)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
add_executable(code
    main.cpp
    user.cpp
    command.cpp
    train.cpp
    order.cpp)