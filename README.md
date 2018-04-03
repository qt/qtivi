# Qt Ivi

This repository hosts multiple Qt modules: the QtIvi modules and the QtGeniviExtras module.

# How to build

	qmake
	make
	make install
	

# Configuration

By running qmake for the first time, a configuration step runs and tries to detect a configuration that works on your
system. At the end a configuration summary is printed, incl. some notes on what the configuration might be missing
due to dependencies missing.

Please read this configuration output first and try to fix the dependencies before creating bug reports.

The configuration step is based on Qt's configure system and also offers some command-line arguments for enabling/disabling
certain features. More about how to configure qtivi can be found here:

https://doc-snapshots.qt.io/qtivi/configuration.html

# Qt Genivi Extras: Platform specific components for GENIVI

This module is similar to qtwinextras or qtmacextras and contains specific components for GENIVI based Linux systems.

Currently the only component it contains is a Qt Categorized Logging Binding towards GENIVI DLT (Diagnostic Log and Trace)

## Depedencies

This module has a dependency towards **dlt-daemon** (https://at.projects.genivi.org/wiki/display/PROJ/Diagnostic+Log+and+Trace)

The dlt-daemon needs to be installed into the system and needs to be detectable by pkg-config.

If the dependencies are not satisfied the complete module is disabled and is not built at all.

See the QtGeniviExtras documentation for more information:

https://doc.qt.io/QtGENIVIExtras/index.html
