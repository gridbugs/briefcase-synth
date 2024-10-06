#!/bin/sh
set -euo pipefail

# Generate a ctags database including system include paths for avr.

CC=avr-gcc
$CC -E -Wp,-v - < /dev/null 2>&1 | \
  # select the list of include paths known by the compiler
  sed -n '/#include <...> search starts here:/,/End of search list./p' | \
  # remove the first line of output which will be a heading
  tail -n+2 | \
  # remove the last line of output which will say "End of search list."
  head -n-1 | \
  # remove leading whitespace on each line
  xargs -n1 echo | \
  # put each include directory on a single line
  tr '\n' ' ' | \
  # generate the tags
  xargs ctags -R .
