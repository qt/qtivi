#include "loggingcategories.h"
#include <QtGeniviExtras/QtDlt>

QDLT_REGISTER_APPLICATION("APP1", "Description for APP")

QDLT_LOGGING_CATEGORY(FOO, "com.pelagicore.foo", "FOO", "FOO CATEGORY")
QDLT_LOGGING_CATEGORY(BAR, "com.pelagicore.bar", "BAR", "BAR CATEGORY")

QDLT_FALLBACK_CATEGORY(FOO)
