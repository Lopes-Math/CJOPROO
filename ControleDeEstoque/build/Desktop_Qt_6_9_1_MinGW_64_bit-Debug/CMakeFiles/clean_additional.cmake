# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\ProjetoFinal_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\ProjetoFinal_autogen.dir\\ParseCache.txt"
  "ProjetoFinal_autogen"
  )
endif()
