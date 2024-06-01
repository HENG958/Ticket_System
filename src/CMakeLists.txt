cmake_minimum_required(VERSION 3.16)
project(trainTicket)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
add_executable(code
    main.cpp
    user_manager.cpp
    command_parser.cpp
    train_manager.cpp
    order_manager.cpp)