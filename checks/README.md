My standard practices is to place test programs and scripts in the tests/
directory, however since `$bindletools_srcdir` is `.` in this project,
automake is creating duplicate targets for `tests/.dirstamp`.  The following
are the resulting warnings from make:

    /Applications/Xcode.app/Contents/Developer/usr/bin/make  all-recursive
    Makefile:1432: warning: overriding commands for target `tests/.dirstamp'
    Makefile:1420: warning: ignoring old commands for target `tests/.dirstamp'
    Makefile:1432: warning: overriding commands for target `tests/.dirstamp'
    Makefile:1420: warning: ignoring old commands for target `tests/.dirstamp'

To avoid this issue in this project, I moved the sources for the test programs
to the `checks/` directory to avoid the conflict.
