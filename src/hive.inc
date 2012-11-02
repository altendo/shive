#!/bin/bash
# hive library

##
# Query HIVE and assign the magical path to a query file, if found
function hive.query()
{
    local args="${@}"
    __msg info "hive args: ${args}"
    hive\
        -hiveconf hive.root.logger=ERROR,console\
        -hiveconf transformPath=$queryPath\
        "${@}"\
        3>&1 1>&2 2>&3\
        | hive.verifyOutput
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


hive.getLatestPartition()
{
    echo $(hive -e "show partitions db_replication_$1" 2> /dev/null | tail -n1 | cut -d "=" -f 2)
}
