# Based on FindThor.cmake module

find_path(SFMLIGHT_INCLUDE_DIR sfl/Config.h
		  PATH_SUFFIXES inc
		  PATHS
		  ${SFMLIGHT_ROOT}
		  $ENV{SFMLIGHT_ROOT}
		  /usr/local/
		  /usr/
		  /sw
		  /opt/local/
		  /opt/csw/
		  /opt/)

# Find the requested modules
set(SFMLIGHT_FOUND TRUE) # will be set to false if one of the required modules is not found
set(FINDSFMLIGHT_LIB_PATHS
	${SFMLIGHT_ROOT}
	$ENV{SFMLIGHT_ROOT}
	/usr/local
	/usr
	/sw
	/opt/local
	/opt/csw
	/opt)

	
# Debug library
find_library(SFMLIGHT_LIBRARY_DEBUG
			 NAMES SFMLight-d
			 PATH_SUFFIXES lib64 lib bin
			 PATHS ${FINDSFMLIGHT_LIB_PATHS})

# Release library
find_library(SFMLIGHT_LIBRARY_RELEASE
			 NAMES SFMLight
			 PATH_SUFFIXES lib64 lib bin
			 PATHS ${FINDSFMLIGHT_LIB_PATHS})
			 
if(SFMLIGHT_LIBRARY_DEBUG OR SFMLIGHT_LIBRARY_RELEASE)
	# Library found
	set(SFMLIGHT_FOUND TRUE)

	# If both are found, set SFMLIGHT_LIBRARY to contain both
	if(SFMLIGHT_LIBRARY_DEBUG AND SFMLIGHT_LIBRARY_RELEASE)
		set(SFMLIGHT_LIBRARY debug	 ${SFMLIGHT_LIBRARY_DEBUG}
						 optimized ${SFMLIGHT_LIBRARY_RELEASE})
	endif()

	# If only one debug/release variant is found, set the other to be equal to the found one
	if(SFMLIGHT_LIBRARY_DEBUG AND NOT SFMLIGHT_LIBRARY_RELEASE)
		# debug and not release
		set(SFMLIGHT_LIBRARY_RELEASE ${SFMLIGHT_LIBRARY_DEBUG})
		set(SFMLIGHT_LIBRARY		 ${SFMLIGHT_LIBRARY_DEBUG})
	endif()
	if(SFMLIGHT_LIBRARY_RELEASE AND NOT SFMLIGHT_LIBRARY_DEBUG)
		# release and not debug
		set(SFMLIGHT_LIBRARY_DEBUG ${SFMLIGHT_LIBRARY_RELEASE})
		set(SFMLIGHT_LIBRARY	   ${SFMLIGHT_LIBRARY_RELEASE})
	endif()
else()
	# Library not found
	set(SFMLIGHT_FOUND FALSE)
	set(SFMLIGHT_LIBRARY "")
	set(FINDSFMLIGHT_MISSING "${FINDSFMLIGHT_MISSING} SFMLIGHT_LIBRARY")
endif()

# Mark variables as advanced
mark_as_advanced(SFMLIGHT_INCLUDE_DIR
				 SFMLIGHT_LIBRARY
				 SFMLIGHT_LIBRARY_RELEASE
				 SFMLIGHT_LIBRARY_DEBUG)

# Result
if(SFMLIGHT_FOUND)
	# Success
	message("Found SFMLight: ${SFMLIGHT_INCLUDE_DIR}")
else()
	# include directory or library not found
	set(FINDSFMLIGHT_ERROR "Could NOT find SFMLight (missing: ${FINDSFMLIGHT_MISSING})")

	if(SFMLIGHT_FIND_REQUIRED)
		# Fatal error
		message(FATAL_ERROR ${FINDSFMLIGHT_ERROR})
	elseif(NOT SFMLIGHT_FIND_QUIETLY)
		# Error, but continue
		message("${FINDSFMLIGHT_ERROR}")
	endif()
endif()
