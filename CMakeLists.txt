cmake_minimum_required(VERSION 3.4)
project(S3L)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set(SOURCE_FILES src/main.cpp src/binary_expression_ast.cpp src/block_ast.cpp src/bool_const_ast.cpp src/function_call_ast.cpp
        src/function_definition_ast.cpp src/int_const_ast.cpp src/ir_visitor.cpp src/lexer.cpp src/real_const_ast.cpp src/parser.cpp src/return_ast.cpp
        src/unary_operator_ast.cpp src/variable_ast.cpp src/decl_var_ast.cpp src/assign_var_ast.cpp src/include/assign_var_ast.h src/composite_ast.cpp src/if_ast.cpp src/while_ast.cpp)
add_executable(S3L ${SOURCE_FILES})

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(S3L ${llvm_libs})