#!/bin/bash
# Shive the Shell HIVE Framework
# Licensed under the GNU GPLv3

function initializeShive()
{
    ## Check if shive is initialize or calling host script
    if [ "$ShiveInitialized" = true ]; then
        return 0;
    fi
    ShiveInitialized=true;

    ## Initialize Bashinator application variables
    export ShiveLib=$(getShiveLib)
    export __ScriptFile=${0##*/} # thisscript.sh
    export __ScriptName=${__ScriptFile%.sh} # thisscript
    export __ScriptPath=${0%/*}; __ScriptPath=${__ScriptPath%/} # /path/to/this/script
    export __ScriptHost=$(hostname -f) # host.example.com

    ## Assumptions about the Shive project
    export ShiveProjectDir=$(dirname $(readlink -f $__ScriptFile))/$__ScriptPath/..
    export ShiveLocalBin=$ShiveProjectDir/bin
    export ShiveLocalLib=$ShiveProjectDir/lib
    export ShiveLocalSql=$ShiveProjectDir/sql
    export ShiveCallFile=$ShiveLocalBin/$__ScriptFile

    ## Source Bashinator framework
    export __BashinatorConfig="${ShiveLib}/framework/bashinator/config.inc"
    export __BashinatorLibrary="${ShiveLib}/framework/bashinator.inc"
    if ! source "${__BashinatorConfig}"; then
        echo "!!! FATAL: failed to source bashinator config '${__BashinatorConfig}'" 1>&2
        exit 2
    fi
    if ! source "${__BashinatorLibrary}"; then
        echo "!!! FATAL: failed to source bashinator library '${__BashinatorLibrary}'" 1>&2
        exit 2
    fi

    ## Initialize Bashinator framework
    __boot
    __dispatch "${@}"
}

##
# Decorator for sourcing library files
#
function include()
{
    local ShiveInc="$(echo $1 | sed -e 's@\.@/@g' -e 's@^shive/@@').inc"
    local ShiveIncAlt="$(echo $1 | sed -e 's@\.@/@g' -e 's@^shive/@@').sh"
    local ShiveClassname=$(basename "$ShiveInc" | cut -d. -f1)
    local ShiveTestVar="shive"$ShiveClassname;

    local ShiveTestExistence='if [ "$'$ShiveTestVar'" ]; then ShiveSourceExists=true; else ShiveSourceExists=false; '$ShiveTestVar'=1; fi';
    eval $ShiveTestExistence

    if [ "$ShiveSourceExists" = false ]; then
        if [ -e "$ShiveLocalLib/$ShiveInc" ]; then
            __msg debug "Attempting to open $ShiveLocalLib/$ShiveInc."
            source "$ShiveLocalLib/$ShiveInc"
        elif [ -e "$ShiveLocalLib/$ShiveIncAlt" ]; then
            __msg debug "Attempting to open $ShiveLocalLib/$ShiveIncAlt."
            source "$ShiveLocalLib/$ShiveIncAlt"
        else
            __msg debug "Attempting to open $ShiveLib/$ShiveInc."
            source "$ShiveLib/$ShiveInc"
        fi
        #    3>&1 1>&2 2>&3 | __msg err
    fi
}

function getShiveLib()
{
    echo "$(caller 0 | cut -d ' ' -f 3 | sed -e s@/shive.sh@@)"
}

# Bashinator functions
function __init()
{
    if [[ "${1}" = '-p' ]]; then
        shift;
        export ShiveHiveQueryPriority=${1}; shift;
    fi
    return 0;
}

function __main()
{
    # SOME HACKISH SHIT RIGHT HERE
    export include
    source $ShiveCallFile "${@}"
    return 0;
}
