

#### Inputs

# input ivigenerator
set(INPUT_ivigenerator "undefined" CACHE STRING "")
set_property(CACHE INPUT_ivigenerator PROPERTY STRINGS undefined no qt system)

# input qface
set(INPUT_qface "undefined" CACHE STRING "")
set_property(CACHE INPUT_qface PROPERTY STRINGS undefined no qt system)



#### Libraries



#### Tests



#### Features

qt_feature("python3" PRIVATE
    LABEL "python3"
    CONDITION tests.python3 OR FIXME
)
qt_feature("python3-virtualenv" PRIVATE
    LABEL "virtualenv"
    CONDITION QT_FEATURE_python3 AND tests.virtualenv OR FIXME
)
qt_feature("system-qface" PUBLIC
    LABEL "System QFace"
    CONDITION tests.qface OR FIXME
    ENABLE INPUT_qface STREQUAL 'system'
    DISABLE INPUT_qface STREQUAL 'no' OR INPUT_qface STREQUAL 'qt'
)
qt_feature("system-ivigenerator" PRIVATE
    LABEL "System IVI Generator"
    ENABLE INPUT_ivigenerator STREQUAL 'system'
    DISABLE ( NOT INPUT_ivigenerator STREQUAL 'system' )
)
qt_feature("ivigenerator" PUBLIC
    LABEL "IVI Generator"
    CONDITION QT_FEATURE_ivicore AND QT_FEATURE_python3 AND ( ( QT_FEATURE_python3_virtualenv AND tests.qface-submodule ) OR ( QT_FEATURE_system_qface ) ) OR QT_FEATURE_system_ivigenerator OR FIXME
    ENABLE INPUT_ivigenerator STREQUAL 'qt' OR INPUT_ivigenerator STREQUAL 'system'
    DISABLE INPUT_ivigenerator STREQUAL 'no'
)
qt_feature("host-tools-only" PRIVATE
    LABEL "Only build the host tools"
    CONDITION INPUT_host_tools_only STREQUAL 'yes'
)
qt_feature("remoteobjects" PUBLIC
    LABEL "QtRemoteObjects Support"
    CONDITION TARGET Qt::RemoteObjects OR INPUT_force_ivigenerator_qtremoteobjects STREQUAL 'yes'
)
qt_feature("ivicore" PUBLIC
    LABEL "Qt IVI Core"
)
qt_configure_add_summary_section(NAME "Qt IVI Core")
qt_configure_add_summary_section(NAME "Python3")
qt_configure_add_summary_entry(ARGS "python3-virtualenv")
qt_configure_add_summary_entry(ARGS "system-qface")
qt_configure_end_summary_section() # end of "Python3" section
qt_configure_add_summary_entry(ARGS "ivigenerator")
qt_configure_add_summary_entry(ARGS "remoteobjects")
qt_configure_end_summary_section() # end of "Qt IVI Core" section
qt_configure_add_summary_entry(
    ARGS "ivicore"
    CONDITION NOT QT_FEATURE_ivicore
)
qt_configure_add_report_entry(
    TYPE ERROR
    MESSAGE "Cannot build the IVI Generator because its dependencies are not satisfied. The IVI Generator provides tooling to generate source code out of IDL files. Make sure python3 and its 'virtualenv' packages are installed. E.g. by running apt-get install python3 python3-virtualenv  And make sure the qface submodule is initialized or the correct qface version is installed on your system. E.g. by running the following command: git submodule init && git submodule update"
    CONDITION QT_FEATURE_ivicore AND NOT QT_FEATURE_ivigenerator AND ( NOT INPUT_ivigenerator STREQUAL 'no' )
)
qt_configure_add_report_entry(
    TYPE WARNING
    MESSAGE "Cannot enable the QtRemoteObjects features because the QtRemoteObjects module is not installed."
    CONDITION NOT QT_FEATURE_remoteobjects
)
