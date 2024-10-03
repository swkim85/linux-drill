# step8
message(STATUS ">> start MakeTable.cmake")
# first we add the executable that generates the table
add_executable(MakeTable  MakeTable.cxx)
target_link_libraries(MakeTable PRIVATE  tutorial_compiler_flags)

# add the command to generate the source code
# MakeTable 프로그램을 이용하여 소스코드를 생성한다. 
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  DEPENDS MakeTable
  )

message(STATUS "<< end MakeTable.cmake")
