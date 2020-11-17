# Generated from server_qtro_test.pro.

#####################################################################
## tst_org-example-echo-qtro Test:
#####################################################################

qt_add_test(tst_org-example-echo-qtro
    OUTPUT_DIRECTORY ".."
    SOURCES
        contactsmodelservice.cpp contactsmodelservice.h
        echoservice.cpp echoservice.h
        echozonedservice.cpp echozonedservice.h
        main.cpp
        server.cpp server.h
        tst_echoqtro.cpp tst_echoqtro.h
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_BINARY_DIR}/../backend_qtro
        ${CMAKE_CURRENT_BINARY_DIR}/../frontend
    PUBLIC_LIBRARIES
        # Remove: L${CMAKE_CURRENT_BINARY_DIR}/..
        Qt::IviCore
        iviremoteobjects_helper_private
)

#### Keys ignored in scope 1:.:.:server_qtro_test.pro:<TRUE>:
# QFACE_FORMAT = "server_qtro"
# QFACE_SOURCES = "../../../org.example.echo.qface"
# QMAKE_PROJECT_NAME = "$$TARGET"
# QMAKE_RPATHDIR = "$$OUT_PWD/.." "$$OUT_PWD/../qtivi"
