add_executable(foo code.cpp)

add_subdirectory(matrix)
add_subdirectory(graph)
add_subdirectory(util)

target_link_libraries(foo 
  matrix
  graph
  util
)