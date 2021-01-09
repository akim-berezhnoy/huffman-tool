#!/bin/bash

valgrind --tool=memcheck --gen-suppressions=all --leak-check=full --leak-resolution=med --track-origins=yes --vgdb=no --error-exitcode=1 cmake-build-RelWithDebInfo/main
