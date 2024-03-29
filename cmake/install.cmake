#####################################################################
# First, install the library itself
# 
MESSAGE(STATUS "\n-- CONFIGURING INSTALLATION DESTINATIONS")
# Configuration files, of course
SET(prefix 		"${PREFIX}")
SET(exec_prefix 	"${PREFIX}")
SET(libdir              "${LIBDIR}")
SET(includedir 		"${INCLUDEDIR}")

# Install the library
IF(BUILDING_SHARED)
	INSTALL(TARGETS sword
		DESTINATION "${LIBDIR}")
ENDIF(BUILDING_SHARED)
IF(BUILDING_STATIC)
	INSTALL(TARGETS sword_static
		DESTINATION "${LIBDIR}")
ENDIF(BUILDING_STATIC)
# Install the locales
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/locales.d
	DESTINATION "${PREFIX}/share/sword")

# Install the headers
INSTALL(FILES ${SWORD_INSTALL_HEADERS}
	DESTINATION "${INCLUDEDIR}/sword")

IF(SWORD_INTERNAL_REGEX)
	INSTALL(FILES ${INTERNAL_REGEX_HEADER}
		DESTINATION "${INCLUDEDIR}/sword")
ENDIF(SWORD_INTERNAL_REGEX)

MESSAGE(STATUS "Destination: ${PREFIX}")

SET(VERSION		${SWORD_VERSION})
IF(WITH_CURL)
	SET(CURL_LIBS	${CURL_LIBRARY})
ENDIF(WITH_CURL)
IF(WITH_CLUCENE)
	SET(CLUCENE_LIBS	${CLUCENE_LIBRARY})
ENDIF(WITH_CLUCENE)
IF(WITH_ICU)
	SET(ICU_LIBS	"${ICU_LIBRARIES} ${ICU_I18N_LIBRARIES}")
ENDIF(WITH_ICU)

IF(LIBSWORD_LIBRARY_TYPE STREQUAL "Static")
	SET(SHAREDLIB_TRUE "#")
ELSE(LIBSWORD_LIBRARY_TYPE STREQUAL "Static")
	SET(SHAREDLIB_FALSE "#")
ENDIF(LIBSWORD_LIBRARY_TYPE STREQUAL "Static")
# The @ONLY restricts it because our ${variable} which are left there as part of pkg-config
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/sword.pc.in ${CMAKE_CURRENT_BINARY_DIR}/sword.pc @ONLY)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/include/swversion.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/swversion.h @ONLY)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/sword.pc
	DESTINATION "${LIBDIR}/pkgconfig")
