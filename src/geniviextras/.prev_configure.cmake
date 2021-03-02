

#### Inputs



#### Libraries

qt_find_package(DLT PROVIDED_TARGETS DLT::DLT MODULE_NAME geniviextras QMAKE_LIB dlt)


#### Tests



#### Features

qt_feature("dlt" PRIVATE
    LABEL "DLT"
    AUTODETECT UNIX
    CONDITION DLT_FOUND
)
qt_feature("dlt_2_12" PRIVATE
    LABEL "DLT > 2.12"
    AUTODETECT UNIX
    CONDITION libs.dlt_2_12 OR FIXME
)
qt_feature("geniviextras-only" PRIVATE
    LABEL "Only build Qt GENIVI Extras"
    CONDITION INPUT_geniviextras_only STREQUAL 'yes'
)
qt_feature("geniviextras" PUBLIC
    LABEL "Qt GENIVI Extras"
    CONDITION QT_FEATURE_dlt
)
qt_configure_add_summary_section(NAME "Qt GENIVI Extras")
qt_configure_add_summary_entry(ARGS "dlt")
qt_configure_add_summary_entry(ARGS "dlt_2_12")
qt_configure_end_summary_section() # end of "Qt GENIVI Extras" section
qt_configure_add_summary_entry(
    ARGS "geniviextras"
    CONDITION NOT QT_FEATURE_geniviextras
)
qt_configure_add_report_entry(
    TYPE NOTE
    MESSAGE "No DLT libs found. Disabled building Qt GENIVI Extras."
    CONDITION NOT QT_FEATURE_dlt
)
qt_configure_add_report_entry(
    TYPE NOTE
    MESSAGE "Old DLT version detected. Not all features of Qt GENIVI Extras can be enabled."
    CONDITION QT_FEATURE_dlt AND NOT QT_FEATURE_dlt_2_12
)
qt_configure_add_report_entry(
    TYPE ERROR
    MESSAGE "Mandatory dependency missing for only building Qt GENIVI Extras."
    CONDITION NOT QT_FEATURE_dlt AND INPUT_geniviextras_only STREQUAL 'yes'
)
