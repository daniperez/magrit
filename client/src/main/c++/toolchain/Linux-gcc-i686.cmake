set ( CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" )
# '-s' means to remove all symbol table and relocation information
# from the executable. It keeps the executable shorter.
set ( CMAKE_CXX_FLAGS "-s" CACHE STRING "Release compilation flags" )
set ( CMAKE_SYSTEM_NAME Linux )
