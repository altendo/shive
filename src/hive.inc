#!/bin/bash
# hive library

##
# Query HIVE and assign the magical path to a query file, if found
# Uses kind of an intense and confusing hack to separate stdout and stderr
function hive.query()
{
    local priority=${ShiveHiveQueryPriority:-NORMAL};
    if [[ "${1}" = '-p' ]]; then
        shift;
        local priority=${1}; shift;
    fi
    local args="${@}"
    __msg debug "priority=${priority} ${args}"
    # inside subshell: swap hive's stdout (results) and stderr (messages)
    # so verifyOutput can read hive messages (reads stdout, outputs stderr);
    # redirect pipe messages to stdout so hive results and hive messages are separate
    # outside subshell: swap stdout and stderr again, so stdout=results, stderr=messages
    {
        hive\
            -hiveconf hive.root.logger=ERROR,console\
            -hiveconf mapred.job.priority=${priority:-NORMAL}\
            -hiveconf projectDir=$ShiveProjectDir\
            "${@}"\
            3>&1 1>&2 2>&3\
            | hive.verifyOutput\
            2>&1
    } 3>&1 1>&2 2>&3

    if [[ "${PIPESTATUS[1]}" = '0' ]]; then
        __msg debug "Hive query executed successfully."
        return 0;
    else
        __msg err "Error executing hive query."
        return ${PIPESTATUS[1]};
    fi
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
            __msg debug "${message}"
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

##
# dump hive query to csv
# @echo tmpfile
function hive.queryToCSV()
{
    local tmpfile=$(mktemp /tmp/shive.XXXXXXXXXXX);
    hive.query "$@" | {
        while read; do
            echo -e $REPLY >> $tmpfile;
        done;
    }

    # note: echo $REPLY converts HIVE \t to whitespace. need to reconvert to tabs.
    sed -i 's/\s/\t/g' $tmpfile;

    echo $tmpfile;
}

function hive.tableModified()
{
    local table=${1};
    local stat=$(hadoop fs -stat "/user/${USER}/${table}" 2>/dev/null);
    local ret=${?};
    echo $stat;
    return $ret;
}

function hive.isTableFresh()
{
    local checkTS=$(date +%s --date="${1}");
    local table=${2};
    local tableModified=$(date +%s --date="$(hive.tableModified $table)");
    return $(( !( $tableModified - $checkTS > 0 ) ));
}
