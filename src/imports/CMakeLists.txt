# Generated from imports.pro.

if(TARGET Qt::IviCore)
    add_subdirectory(core)
endif()
if(TARGET Qt::IviMedia)
    add_subdirectory(media)
endif()
if(TARGET Qt::IviVehicleFunctions) # special case
    add_subdirectory(vehiclefunctions)
endif()
