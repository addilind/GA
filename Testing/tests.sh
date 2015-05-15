#!/bin/bash
#set -x
if cat "$2.input" | $1 --nobanner 2>&1 | grep -Pzqf "$2.expected" ; then
    exit 0 #Success
else
    exit 1 #Failure
fi