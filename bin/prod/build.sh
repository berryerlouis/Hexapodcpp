#! /bin/sh

echo #######################
echo Uncrustifying...
#bin/dev/uncrustify-all.sh
echo Uncrustify done.
echo #######################

echo #######################
echo Building X64 sources RELEASE...
bin/dev/build.sh X64 sources RELEASE
echo Buil done.
echo #######################

echo #######################
echo Testing all...
bin/dev/test.sh all
echo Test all done.
echo #######################

echo #######################
echo Updating UML...
docker run --rm -v $PWD:/ws -w /ws plantuml/plantuml -tsvg images/architecture.puml
echo Update UML done.
echo #######################