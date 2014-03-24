#!/bin/sh
#
#   Bindle Binaries Tools
#   Copyright (C) 2014 Bindle Binaries <syzdek@bindlebinaries.com>.
#

DATFILE=`echo ${0} |sed -e 's/.sh$/.txt/g'`

COUNT=`./tests/fd/fdtest -eL ${DATFILE} |awk -D: '{print$2}'`
echo "......................................................................"
echo "parsing:   ${DATFILE}"
echo "Expecting: 3"
echo "Recieved:  ${COUNT}"
echo "......................................................................"
test "x${COUNT}" == "x3"
exit $?

COUNT=`./tests/fd/fdtest -eR ${DATFILE} |awk -D: '{print$2}'`
echo "......................................................................"
echo "parsing:   ${DATFILE}"
echo "Expecting: 6"
echo "Recieved:  ${COUNT}"
echo "......................................................................"
test "x${COUNT}" == "x6"
exit $?

# end of script