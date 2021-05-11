# Generated from server_qtro_test.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_executable(tst_org-example-echo-qtro
    SOURCES
    contactsmodelservice.cpp contactsmodelservice.h
    echoservice.cpp echoservice.h
    echozonedservice.cpp echozonedservice.h
    main.cpp
    server.cpp server.h
    tst_echoqtro.cpp tst_echoqtro.h
)

# IVI Generator:
qt6_ivigenerator(tst_org-example-echo-qtro
    QFACE_SOURCES ../../../org.example.echo.qface
    QFACE_FORMAT server_qtro
)

target_link_libraries(tst_org-example-echo-qtro PUBLIC
    echo_qtro_frontend
    Qt::Core
    Qt::IviCore
    Qt::Test
    Qt::IviRemoteObjectsHelper
)
