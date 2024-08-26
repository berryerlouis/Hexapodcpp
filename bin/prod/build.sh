#! /bin/sh

bin/dev/uncrustify-all.sh
bin/dev/build.sh RELEASE
bin/dev/test.sh
