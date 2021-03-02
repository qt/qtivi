# We can't create the same interface imported target multiple times, CMake will complain if we do
# that. This can happen if the find_package call is done in multiple different subdirectories.
if(TARGET WrapSystemTagLib::WrapSystemTagLib)
    set(WrapSystemTagLib_FOUND ON)
    return()
endif()

find_package(PkgConfig)

pkg_check_modules(TagLib taglib IMPORTED_TARGET)
set(__taglib_target_name "PkgConfig::TagLib")

if (NOT TARGET "${__taglib_target_name}")
    set(WrapSystemTagLib_FOUND 0)
    return()
endif()

add_library(WrapSystemTagLib::WrapSystemTagLib INTERFACE IMPORTED)
target_link_libraries(WrapSystemTagLib::WrapSystemTagLib INTERFACE ${__taglib_target_name})
target_include_directories(WrapSystemTagLib::WrapSystemTagLib INTERFACE ${TAGLIB_INCLUDEDIR})
set(WrapSystemTagLib_FOUND TRUE)
unset(__taglib_target_name)
