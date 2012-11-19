#!/bin/bash

SED=sed
FILE=$1

if [[ $FILE == *.h ]]; then
  $SED -e 's|#include "rant/op.h"|#include "op.h"|g' $FILE
  #$SED -e 's|#include "rant/\(.*\).h"|#include "\1.h"|g' $FILE
else
  cat $FILE
fi
