SET( PETSC_FOUND 0 )

FIND_PATH(
  PETSC_INCLUDE_DIR
  NAMES petsc.h
  PATHS /usr/include/petsc/
  PATHS /usr/include/petscdir/
  )
FIND_PATH(
  PETSCCONF_INCLUDE_DIR
  NAMES petscconf.h
  PATHS /usr/include/petsc/ 
  PATHS /usr/include/petscdir/
  )
#MESSAGE( STATUS "PETSC_INCLUDE_DIR: ${PETSC_INCLUDE_DIR}")
#MESSAGE( STATUS "PETSCCONF_INCLUDE_DIR: ${PETSCCONF_INCLUDE_DIR}")

IF( PETSCCONF_INCLUDE_DIR STREQUAL PETSC_INCLUDE_DIR )
  SET(CMAKE_REQUIRED_INCLUDES "${PETSC_INCLUDE_DIR} ${CMAKE_REQUIRED_INCLUDES}")
ELSE( PETSCCONF_INCLUDE_DIR STREQUAL PETSC_INCLUDE_DIR )
  SET(CMAKE_REQUIRED_INCLUDES "${PETSC_INCLUDE_DIR} ${PETSCCONF_INCLUDE_DIR} ${CMAKE_REQUIRED_INCLUDES}")
ENDIF( PETSCCONF_INCLUDE_DIR STREQUAL PETSC_INCLUDE_DIR )

#MESSAGE( STATUS "CMAKE_REQUIRED_INCLUDES: ${CMAKE_REQUIRED_INCLUDES}")

IF (PETSC_INCLUDE_DIR)
  IF (PETSCCONF_INCLUDE_DIR)
    SET( PETSC_FOUND 1 )
    MESSAGE( STATUS "Found PETSC: ${PETSC_INCLUDE_DIR}")
  ENDIF (PETSCCONF_INCLUDE_DIR)
ELSE (PETSC_INCLUDE_DIR)
  MESSAGE(FATAL_ERROR "No se encontró petsc.h")
ENDIF (PETSC_INCLUDE_DIR)

IF ( PETSC_FOUND )
  ADD_DEFINITIONS( -DHAVE_PETSC_H -DHAVE_PETSC )
ENDIF( PETSC_FOUND )


#FIND_LIBRARY(PETSC_LIB_MPIUNI    mpiuni    PATHS /usr/local/petsc-2.3.3-p8/lib/linux-gnu-c-debug QUIET )
FIND_LIBRARY(PETSC_LIB_PETSC     petsc     PATHS /usr/lib)
FIND_LIBRARY(PETSC_LIB_PETSCDM   petscdm   PATHS /usr/lib)
FIND_LIBRARY(PETSC_LIB_PETSCVEC  petscvec  PATHS /usr/lib)
FIND_LIBRARY(PETSC_LIB_PETSCMAT  petscmat  PATHS /usr/lib)
FIND_LIBRARY(PETSC_LIB_PETSCKSP  petscksp  PATHS /usr/lib)
FIND_LIBRARY(PETSC_LIB_PETSCSNES petscsnes PATHS /usr/lib)


SET(PETSC_LIBRARIES
  ${PETSC_LIB_PETSCSNES}
  ${PETSC_LIB_PETSCKSP}
  ${PETSC_LIB_PETSCMAT}
  ${PETSC_LIB_PETSCVEC}
  ${PETSC_LIB_PETSCDM}
  ${PETSC_LIB_PETSC}
#  ${PETSC_LIB_MPIUNI}
)
