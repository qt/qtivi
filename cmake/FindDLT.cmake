# We can't create the same interface imported target multiple times, CMake will complain if we do
# that. This can happen if the find_package call is done in multiple different subdirectories.
if(TARGET DLT::DLT)
    set(DLT_FOUND ON)
    return()
endif()

find_package(PkgConfig)

pkg_check_modules(DLT automotive-dlt IMPORTED_TARGET)
set(__dlt_target_name "PkgConfig::DLT")

if (NOT TARGET "${__dlt_target_name}")
    set(DLT_FOUND 0)
    return()
endif()

add_library(DLT::DLT INTERFACE IMPORTED)
target_link_libraries(DLT::DLT INTERFACE ${__dlt_target_name})
target_include_directories(DLT::DLT INTERFACE ${DLT_INCLUDEDIR})
set(DLT_FOUND TRUE)
unset(__dlt_target_name)
