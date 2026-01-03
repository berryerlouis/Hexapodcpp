#!/bin/sh

rm -rf cppcheck_html 
rm -f cppcheck-report.xml

cppcheck \
    --error-exitcode=1 \
    --enable=all \
    --suppress=checkersReport \
    --suppress=missingIncludeSystem \
    --suppress=unusedFunction \
    --suppress=missingInclude \
    --suppress=knownConditionTrueFalse \
    --inconclusive \
    --force \
    --inline-suppr \
    --language=c++ \
    --xml \
    --xml-version=2 \
    src \
    2> cppcheck-report.xml

cppcheck-htmlreport --file=cppcheck-report.xml --report-dir=cppcheck_html

NB_ERROR=$(grep -c '<error ' cppcheck-report.xml)
echo "Error count: $NB_ERROR"
exit "$NB_ERROR"
