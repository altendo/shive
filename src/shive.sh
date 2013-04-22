#!/bin/bash
# Shive the Shell HIVE Framework
# Licensed under the GNU GPLv3

function getApplicationPath()
{
    echo "$(caller 0 | cut -d ' ' -f 3 | sed -e s@/shive.sh@@)"
}

{
    ## Check if shive is initialize or calling host script
    if [ "$ShiveInitialized" = true ]; then
        return 0;
    fi
    ShiveInitialized=true;

    ## Determine running correct shell environment
    #if [ -z $(echo $BASH | grep -iE "bash") ]; then
    #    echo "!!! FATAL: bash 4.x (/bin/bash) required to use shive.";
    #    exit 2;
    #fi

    ## Initialize Bashinator application variables
    export __ScriptFile=${0##*/} # thisscript.sh
    export __ScriptName=${__ScriptFile%.sh} # thisscript
    export __ScriptPath=${0%/*}; __ScriptPath=${__ScriptPath%/} # /path/to/this/script
    export __ScriptHost=$(hostname -f) # host.example.com
    export ApplicationPath=$(getApplicationPath);
    export ApplicationLibrary="${ApplicationPath}/framework/shivelib.inc";

    ## Source Bashinator framework
    export __BashinatorConfig="${ApplicationPath}/framework/bashinator/config.inc";
    export __BashinatorLibrary="${ApplicationPath}/framework/bashinator.inc";
    if ! source "${__BashinatorConfig}"; then
        echo "!!! FATAL: failed to source bashinator config '${__BashinatorConfig}'" 1>&2
        exit 2
    fi
    if ! source "${__BashinatorLibrary}"; then
        echo "!!! FATAL: failed to source bashinator library '${__BashinatorLibrary}'" 1>&2
        exit 2
    fi
    if ! source "${ApplicationLibrary}"; then
        echo "!!! FATAL: failed to source application library '${ApplicationLibrary}'" 1>&2;
        exit 2;
    fi

    ## Project variables
    export ShiveProjectDir=$(dirname $(readlink -f $(caller 0 | cut -d ' ' -f 3)));
    export ShiveCallFile=$(readlink -f $__ScriptPath/$__ScriptFile);
    export ShiveLocalLib=$ShiveProjectDir/lib
    export ShiveLocalSql=$ShiveProjectDir/sql

    ## Initialize Bashinator framework
    __boot
    __dispatch "${@}"
}
