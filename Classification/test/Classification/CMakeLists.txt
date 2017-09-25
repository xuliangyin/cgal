# Created by the script cgal_create_CMakeLists
# This is the CMake script for compiling a set of CGAL applications.

project( Classification_Tests )


cmake_minimum_required(VERSION 2.8.11)

# CGAL and its components
find_package( CGAL QUIET COMPONENTS  )

if ( NOT CGAL_FOUND )

  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()  

endif()

# include helper file
include( ${CGAL_USE_FILE} )


# Boost and its components
find_package( Boost REQUIRED )

# Use Eigen
find_package(Eigen3 3.1.0) #(requires 3.1.0 or greater)
if (EIGEN3_FOUND)
  include( ${EIGEN3_USE_FILE} )
endif()

if ( NOT Boost_FOUND )

  message(STATUS "This project requires the Boost library, and will not be compiled.")

  return()  

endif()

find_package( TBB )
if( TBB_FOUND )
  include(${TBB_USE_FILE})
  list(APPEND CGAL_3RD_PARTY_LIBRARIES ${TBB_LIBRARIES})
endif()

# include for local directory
include_directories( BEFORE include )

# include for local package
include_directories( BEFORE ../../include )


# Creating entries for all C++ files with "main" routine
# ##########################################################

include( CGAL_CreateSingleSourceCGALProgram )

#add_definitions("-DCGAL_DO_NOT_USE_BOYKOV_KOLMOGOROV_MAXFLOW_SOFTWARE")

set(needed_cxx_features cxx_rvalue_references cxx_variadic_templates)
create_single_source_cgal_program( "test_classification_point_set.cpp" CXX_FEATURES ${needed_cxx_features} )
