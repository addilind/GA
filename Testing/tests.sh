#!/bin/bash
let TESTNO=1
while ls | grep -q "$TESTNO.input"; do
    if cat "$TESTNO.input" | $1 --nobanner | grep -q -f "$TESTNO.expected" ; then
        echo -e "\xE2\x9C\x94 Test $TESTNO succeeded";
    else
        echo -e -n "\xE2\x9C\x98 Test $TESTNO failed: ";
        cat "$TESTNO.message";
    fi

    let TESTNO=TESTNO+1;
done