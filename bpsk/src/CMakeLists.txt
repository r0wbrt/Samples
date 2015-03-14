add_executable(costablegen costablegen.c)
target_link_libraries(costablegen m)


add_custom_command(
    OUTPUT costable.h
    COMMAND costablegen > costable.h
    COMMENT "Generating Look Up Table"
)


include_directories(${CMAKE_CURRENT_BINARY_DIR})






set(DBPSK_SRCS
	bpsk_tx.c
	costable.h
)


set(DBPSK_PUBLIC_HEADERS
	dbpsk.h
	#src/libfixmath/fixmath.h
)

add_library(dbpsk SHARED ${DBPSK_SRCS} )
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(dbpsk ${CMAKE_REQUIRED_LIBRARIES})
set_target_properties(dbpsk PROPERTIES
	SOVERSION ${SOVERSION}
	PUBLIC_HEADER "${DBPSK_PUBLIC_HEADERS}"
	DEPENDS costablegen
)


add_executable(tx_test tx_test.c)
target_link_libraries(tx_test ${CMAKE_REQUIRED_LIBRARIES} dbpsk)



install(TARGETS dbpsk EXPORT dbpsk-config
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dbpsk
)


install(TARGETS
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
