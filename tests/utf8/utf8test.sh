#!/bin/bash
FILE=`echo $0 |sed -e 's/.sh$/.txt/g'`
./tests/utf8/utf8test ${srcdir}/${FILE}
exit $?
