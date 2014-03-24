#!/bin/sh
#
#   Bindle Binaries Tools
#   Copyright (C) 2014 Bindle Binaries <syzdek@bindlebinaries.com>.
#

DATFILE=`echo ${0} |sed -e 's/.sh$/.txt/g'`

COUNT=`./tests/fd/fdtest -R ${DATFILE} |awk -D: '{print$2}'`

echo "......................................................................"
echo "parsing:   ${DATFILE}"
echo "Expecting: 36"
echo "Recieved:  ${COUNT}"
echo "......................................................................"

test "x${COUNT}" == "x36"
exit $?

# end of script