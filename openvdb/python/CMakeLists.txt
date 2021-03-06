
PROJECT ( PyOpenVDB )

FIND_PACKAGE ( PythonInterp REQUIRED )
FIND_PACKAGE ( PythonLibs REQUIRED )

IF (OPENVDB_STATIC_PYTHON_MODULE)
  SET (Boost_USE_STATIC_LIBS ON)
  SET (Ilmbase_USE_STATIC_LIBS ON)
ENDIF()

FIND_PACKAGE ( Boost ${MINIMUM_BOOST_VERSION} REQUIRED COMPONENTS python )


IF ( NOT OPENVDB_BUILD_CORE )
  FIND_PACKAGE ( OpenVDB REQUIRED )

  FIND_PACKAGE ( ILMBase REQUIRED )
  FIND_PACKAGE ( OpenEXR REQUIRED )
  FIND_PACKAGE ( Blosc REQUIRED )
  FIND_PACKAGE ( TBB REQUIRED )
  FIND_PACKAGE ( ZLIB REQUIRED )


  # Set the variables that are otherwise defined in the core module.
  SET ( OPENVDB_MAJOR_VERSION_NUMBER ${OpenVDB_MAJOR_VERSION} )
  SET ( OPENVDB_MINOR_VERSION_NUMBER ${OpenVDB_MINOR_VERSION} )
  SET ( OPENVDB_PATCH_VERSION_NUMBER ${OpenVDB_PATCH_VERSION} )

  # Use the library installed
  SET ( OPENVDB_LIB ${OpenVDB_OPENVDB_LIBRARY} )

ELSE ()
  # Use the target
  IF ( OPENVDB_STATIC_PYTHON_MODULE)
    SET ( OPENVDB_LIB openvdb_static)
  ELSE()
    SET ( OPENVDB_LIB openvdb_shared)
  ENDIF()

ENDIF ( NOT OPENVDB_BUILD_CORE )

INCLUDE_DIRECTORIES ( SYSTEM ${ILMBASE_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES ( SYSTEM ${OPENEXR_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES ( SYSTEM ${Boost_INCLUDE_DIR} )
INCLUDE_DIRECTORIES ( SYSTEM ${BLOSC_INCLUDE_DIR} )
INCLUDE_DIRECTORIES ( SYSTEM ${TBB_INCLUDE_DIR} )
INCLUDE_DIRECTORIES ( SYSTEM ${ZLIB_INCLUDE_DIR} )
INCLUDE_DIRECTORIES ( SYSTEM ${PYTHON_INCLUDE_DIR} )
# MESSAGE ( "PYTHONLIBS_VERSION_STRING = ${PYTHONLIBS_VERSION_STRING}" )

STRING(REPLACE "." ";" VERSION_LIST ${PYTHONLIBS_VERSION_STRING})
LIST(GET VERSION_LIST 0 OPENVDB_PYTHON_VERSION_MAJOR)
LIST(GET VERSION_LIST 1 OPENVDB_PYTHON_VERSION_MINOR)
LIST(GET VERSION_LIST 2 OPENVDB_PYTHON_VERSION_PATCH)

# MESSAGE ( "OPENVDB_PYTHON_VERSION_MAJOR = ${OPENVDB_PYTHON_VERSION_MAJOR}" )
# MESSAGE ( "OPENVDB_PYTHON_VERSION_MINOR = ${OPENVDB_PYTHON_VERSION_MINOR}" )
SET ( OPENVDB_PYTHON_MODULE_SOURCE_FILES
  pyFloatGrid.cc
  pyIntGrid.cc
  pyMetadata.cc
  pyOpenVDBModule.cc
  pyTransform.cc
  pyVec3Grid.cc
  )

SET ( PYOPENVDB_INSTALL_DIRECTORY
  lib/python${OPENVDB_PYTHON_VERSION_MAJOR}.${OPENVDB_PYTHON_VERSION_MINOR}
  CACHE STRING "The directory to install the pyopenvdb.so module."
  )

SET_SOURCE_FILES_PROPERTIES ( ${OPENVDB_PYTHON_MODULE_SOURCE_FILES}
  PROPERTIES
  COMPILE_FLAGS "-DOPENVDB_PRIVATE -DOPENVDB_USE_BLOSC ${OPENVDB_USE_GLFW_FLAG}"
  )

IF (NOT WIN32)
  # RPath handling
  IF ( OPENVDB_ENABLE_RPATH )

    SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

    IF( OpenVDB_LIBRARY_DIR )
      SET(CMAKE_INSTALL_RPATH ${OpenVDB_LIBRARY_DIR})
    ELSE ()
      MESSAGE ( WARNING "Assumes the openvdb shared library is one level up")
      MESSAGE ( WARNING "If this is wrong set OpenVDB_LIBRARY_DIR to correct path")
      SET(CMAKE_INSTALL_RPATH "$ORIGIN/..")
    ENDIF ()
  ENDIF ( OPENVDB_ENABLE_RPATH )
ENDIF()

ADD_LIBRARY ( pyopenvdb SHARED
  ${OPENVDB_PYTHON_MODULE_SOURCE_FILES}
  )

IF (WIN32)

  IF (MSVC14)
    TARGET_SOURCES(pyopenvdb PRIVATE msvc14fixes.cpp)
  ENDIF()

  ADD_DEFINITIONS ( -D_WIN32 -DNOMINMAX)
  IF ( OPENVDB_STATIC_PYTHON_MODULE)
    ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB -DBOOST_PYTHON_STATIC_LIB)
  ELSE ()
    ADD_DEFINITIONS(-DOPENVDB_DLL)

  ENDIF ()
ENDIF ()

TARGET_LINK_LIBRARIES ( pyopenvdb
  ${OPENVDB_LIB}
  ${OPENGL_gl_LIBRARY}
  ${OPENGL_glu_LIBRARY}
  #${COCOA_LIBRARY}
  #${IOKIT_LIBRARY}
  #${COREVIDEO_LIBRARY}
  ${GLFW_LINK_LIBRARY}
  ${GLFW_DEPENDENT_LIBRARIES}
  ${GLEW_GLEW_LIBRARY}
  ${Boost_PYTHON_LIBRARY}
  ${PYTHON_LIBRARY}
  ${Ilmbase_HALF_LIBRARY}
  )

SET_TARGET_PROPERTIES ( pyopenvdb PROPERTIES
  PREFIX ""
  )

ADD_TEST ( pytest ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/test/TestOpenVDB.py )
SET_TESTS_PROPERTIES ( pytest PROPERTIES ENVIRONMENT "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}")

INSTALL ( TARGETS
  pyopenvdb
  DESTINATION
  ${PYOPENVDB_INSTALL_DIRECTORY}
  )       

INSTALL ( DIRECTORY ${PROJECT_SOURCE_DIR} DESTINATION include
  FILES_MATCHING PATTERN "*.h"
  )
