
FILE=`echo $0 |sed -e 's/.sh$/.txt/g'`
./tests/utf8test ${FILE}
exit $?