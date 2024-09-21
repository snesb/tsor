# Get the latest abbreviated commit hash of the working branch
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest commit date of the working branch
execute_process(
  COMMAND git log -1 --format=%cD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_DATE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Generate header file with string macros
configure_file(
  ${CMAKE_SOURCE_DIR}/src/util/version.h.in
  ${CMAKE_SOURCE_DIR}/src/util/version.h
)
