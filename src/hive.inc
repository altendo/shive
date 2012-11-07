#!/bin/bash
# hive library

##
# Query HIVE and assign the magical path to a query file, if found
# Uses kind of an intense and confusing hack to separate stdout and stderr
function hive.query()
{
    local args="${@}"
    __msg info "hive args: ${args}"
    # inside subshell: swap hive's stdout (results) and stderr (messages)
    # so verifyOutput can read hive messages (reads stdout, outputs stderr);
    # redirect pipe messages to stdout so hive results and hive messages are separate
    # outside subshell: swap stdout and stderr again, so stdout=results, stderr=messages
    {
        hive\
            -hiveconf hive.root.logger=ERROR,console\
            "${@}"\
            3>&1 1>&2 2>&3\
            | hive.verifyOutput\
            2>&1
    } 3>&1 1>&2 2>&3
    return ${PIPESTATUS[1]};
}

##
# Interpret and correctly flag the severity of HIVE output 
function hive.verifyOutput()
{
    local -i trippedError=0
    local message=$@
    while read; do
        local message="${REPLY}"
        if [[ ! -z `echo $message | grep -iE "EXCEPTION|FAIL|FATAL|Can't open|Could not open|No such file"` ]]; then
            trippedError=1
            __msg err "${message}"
        elif [[ $trippedError = 1 ]]; then
            __msg err "${message}"
        elif [[ ! -z `echo $message | grep -iE "error"` ]]; then
            __msg debug "${message}"
        elif [[ ! -z `echo $message | grep -iE "rows loaded|time taken|ok|loading data"` ]]; then
            __msg info "${message}"
        else
            __msg debug "${message}"
        fi
    done
    if [[ $trippedError = 1 ]]; then
        return 2;
    else
        return 0;
    fi
}
