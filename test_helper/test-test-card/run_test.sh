#!/bin/bash

[ -r ../shell_helper.sh ] || { echo "Cannot source shell_helper.sh"; exit -1; }
source ../shell_helper.sh

($INSTR send_card 40:a0:83:80
    sleep 3
    kill `pidof leosac`
)&

#start leosac and wait for return value
(./install/bin/leosac -k $TMP_DIR/this_test/test-test-card.xml > leosac-log &
    echo $! > pid-file;  wait $! && echo $? > exit-status)

## check test sequence was run.
if ! grep "\[debug\]" $TMP_DIR/leosac-log | grep "Running test sequence..."; then
    fail "Test sequence wasn't run";
fi

die 0
