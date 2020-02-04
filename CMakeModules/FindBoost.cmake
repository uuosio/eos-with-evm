message(STATUS "Using custom FindBoost.cmake")
set(Boost_FOUND TRUE)
set(Boost_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/boost/includes")
set(Boost_LIBRARIES boost_1_67_0)

set(libboost_library "${CMAKE_BINARY_DIR}/externals/boost/${CMAKE_STATIC_LIBRARY_PREFIX}boost_1_67_0${CMAKE_STATIC_LIBRARY_SUFFIX}")

if (TARGET Boost::program_options)
else()
    add_library(Boost::program_options STATIC IMPORTED)
    set_property(TARGET Boost::program_options PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
    set_property(TARGET Boost::program_options PROPERTY IMPORTED_CONFIGURATIONS Release)
    set_property(TARGET Boost::program_options PROPERTY IMPORTED_LOCATION_RELEASE ${libboost_library})

    add_library(Boost::fiber STATIC IMPORTED)
    add_library(Boost::context STATIC IMPORTED)
    add_library(Boost::thread STATIC IMPORTED)
    add_library(Boost::log STATIC IMPORTED)

    add_library(Boost::filesystem STATIC IMPORTED)
    set_property(TARGET Boost::filesystem PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
    set_property(TARGET Boost::filesystem PROPERTY IMPORTED_CONFIGURATIONS Release)
    set_property(TARGET Boost::filesystem PROPERTY IMPORTED_LOCATION_RELEASE ${libboost_library})

    add_library(Boost::system STATIC IMPORTED)
    set_property(TARGET Boost::system PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
    set_property(TARGET Boost::system PROPERTY IMPORTED_CONFIGURATIONS Release)
    set_property(TARGET Boost::system PROPERTY IMPORTED_LOCATION_RELEASE ${libboost_library})


    add_library(Boost::boost STATIC IMPORTED)
    set_property(TARGET Boost::boost PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})

    add_library(Boost::unit_test_framework STATIC IMPORTED)
    set_property(TARGET Boost::unit_test_framework PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
    set_property(TARGET Boost::unit_test_framework PROPERTY IMPORTED_CONFIGURATIONS Release)
    set_property(TARGET Boost::unit_test_framework PROPERTY IMPORTED_LOCATION_RELEASE ${libboost_library})

    add_library(Boost::chrono STATIC IMPORTED)
    set_property(TARGET Boost::chrono PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
    set_property(TARGET Boost::chrono PROPERTY IMPORTED_CONFIGURATIONS Release)
    set_property(TARGET Boost::chrono PROPERTY IMPORTED_LOCATION_RELEASE ${libboost_library})

    add_library(Boost::iostreams STATIC IMPORTED)
    set_property(TARGET Boost::iostreams PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
    set_property(TARGET Boost::iostreams PROPERTY IMPORTED_CONFIGURATIONS Release)
    set_property(TARGET Boost::iostreams PROPERTY IMPORTED_LOCATION_RELEASE ${libboost_library})

    add_library(Boost::date_time STATIC IMPORTED)
    set_property(TARGET Boost::date_time PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
    set_property(TARGET Boost::date_time PROPERTY IMPORTED_CONFIGURATIONS Release)
    set_property(TARGET Boost::date_time PROPERTY IMPORTED_LOCATION_RELEASE ${libboost_library})
    
endif()
