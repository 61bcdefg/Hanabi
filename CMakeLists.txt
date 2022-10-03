cmake_minimum_required(VERSION "3.13.0")
add_library(LLVMHanabiDeps SHARED #This is for linking a minimum subset of LLVM needed which serves as the escape plan
  ${CMAKE_CURRENT_LIST_DIR}/Dummy.cpp
)
add_dependencies(LLVMHanabiDeps
  LLVMCore
  LLVMSupport
  LLVMPasses
)

set(CMAKE_INSTALL_RPATH "@loader_path/")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

add_library(LLVMHanabi SHARED
  ${CMAKE_CURRENT_LIST_DIR}/Loader.cpp
)
add_dependencies(LLVMHanabi
  LLVMHanabiDeps
  LLVMObfuscation
)
target_link_libraries(LLVMHanabiDeps PRIVATE LLVMCore LLVMSupport LLVMPasses ${CMAKE_CURRENT_LIST_DIR}/libsubstitute.a)
target_link_options(LLVMHanabiDeps PRIVATE -all_load -lobjc)

target_link_libraries(LLVMHanabi PRIVATE LLVMObfuscation LLVMHanabiDeps)
target_link_options(LLVMHanabi PRIVATE -flat_namespace)
target_compile_options(LLVMHanabi PRIVATE -Wno-dollar-in-identifier-extension PRIVATE -Wno-variadic-macros PRIVATE)
