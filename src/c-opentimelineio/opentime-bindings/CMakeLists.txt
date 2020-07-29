set(OPENTIME_HEADER_FILES
        include/copentime/timeRange.h
        include/copentime/rationalTime.h
        include/copentime/errorStatus.h
        include/copentime/timeTransform.h
        )

add_library(copentime STATIC
        src/timeRange.cpp
        src/rationalTime.cpp
        src/errorStatus.cpp
        src/timeTransform.cpp
        ${OPENTIME_HEADER_FILES})

target_include_directories(copentime PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/src/c-opentimelineio/opentime-bindings/include")

target_link_libraries(copentime opentime)

set_target_properties(copentime PROPERTIES 
        INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
        MACOSX_RPATH true 
        WINDOWS_EXPORT_ALL_SYMBOLS true)

if(OTIO_C_INSTALL)
    install(FILES ${OPENTIME_HEADER_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/copentime)
    install(TARGETS copentime
            RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
            LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
endif()
