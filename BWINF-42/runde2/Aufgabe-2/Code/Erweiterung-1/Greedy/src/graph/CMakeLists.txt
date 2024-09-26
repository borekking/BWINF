add_library(graph
  include/max_cliques.h
  src/max_cliques.cpp
)

target_include_directories(graph PUBLIC include)

target_link_libraries(graph matrix)