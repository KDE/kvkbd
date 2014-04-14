cmake_minimum_required(VERSION 2.4)
project( kvkbd )

find_package(KDE4 REQUIRED)

include (KDE4Defaults)

add_subdirectory( doc )
add_subdirectory( src )
add_subdirectory( pics )
add_subdirectory( colors )
add_subdirectory( themes )