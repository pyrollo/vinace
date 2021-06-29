
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h)

link_directories(
    ${GTKMM_LIBRARY_DIRS}
)

include_directories(
		${CMAKE_CURRENT_SOURCE_DIR}
    ${GTKMM_INCLUDE_DIRS}
)

add_subdirectory(core)
add_subdirectory(hardware)
add_subdirectory(gui)

set(SOURCE_FILES
	${CORE_SOURCE_FILES}
	${HARDWARE_SOURCE_FILES}
	${GUI_SOURCE_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
)

add_executable(vinace ${SOURCE_FILES})

target_link_libraries(vinace ${GTKMM_LIBRARIES})

if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(vinace PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(vinace ${CMAKE_THREAD_LIBS_INIT})
endif()

target_link_libraries(vinace pulse-simple pulse)


