add_library(matrix 
  include/matrix.h
  include/matrix_bool.h
  include/matrix_int.h
  src/matrix.cpp  
  src/matrix_int.cpp  
  src/matrix_bool.cpp  
)

target_include_directories(matrix PUBLIC include)