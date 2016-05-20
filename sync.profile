%modules = ( # path to module name map
    "QtIviCore" => "$basedir/src/ivicore",
    "QtIviVehicleFunctions" => "$basedir/src/ivivehiclefunctions",
    "QtGeniviExtras" => "$basedir/src/geniviextras",
);
%moduleheaders = ( # restrict the module headers to those found in relative path
);
%classnames = (
     "qdlt.h" => "QtDlt",
     "qtiviqueryterm.h" => "QIviAbstractQueryTerm", "QIviConjunctionTerm", "QIviScopeTerm", "QIviFilterTerm", "QIviOrderTerm",
);
%deprecatedheaders = (
);
# Module dependencies.
# Every module that is required to build this module should have one entry.
# Each of the module version specifiers can take one of the following values:
#   - A specific Git revision.
#   - any git symbolic ref resolvable from the module's repository (e.g. "refs/heads/master" to track master branch)
#
%dependencies = (
    "qtbase" => "",
    "qtdeclarative" => ""
);

