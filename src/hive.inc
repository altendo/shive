##
# HIVE extention

##
# Query HIVE and assign the magical path to a query file, if found
#
function query()
{
    ## find query file, if given, and replace query path for hive call
    local queryCaller=$(caller 0 | cut -d ' ' -f 3)
    local queryPath=${queryCaller:0:${#queryCaller}-4}
    local args=''
    local file=false
    while [ -n "$1" ]
    do
        arg=$1
        if [ "${file}" = true ]; then
            arg="${queryPath}/${arg}"
            if [ ! -f $arg ]; then
                __msg err "Query File ${1} at ${queryPath} does not exist."
                return 2;
            fi
            file=false
        fi
        if [ "${arg}" = "-f" ]; then
            file=true
        fi
        args="${args} ${arg}"
        shift
    done
    ## strip any newlines the calling function may have put
    args=`echo -n $args | tr -d "\n"`
    ## finally call hive
    __msg debug "Calling HIVE: ${args}"
    hive\
        -hiveconf hive.root.logger=ERROR,console\
        ${args}\
        3>&1 1>&2 2>&3\
        | hiveOutput
    ## return status of hive job -- return with error if hiveoutput saw an error
    if [[ "${PIPESTATUS[1]}" = 2 ]]; then
        return 2;
    else
        return 0;
    fi
}

##
# Interpret and correctly flag the severity of HIVE output 
#
function hiveOutput()
{
    local message=$@
    local -a messageArray=()
    if [[ -z ${message} ]]; then
        while read; do
            messageArray+=( "${REPLY}" )
        done
    else
        messageArray=( "${message}" )
    fi
    local -i m=0
    local -i trippedError=0
    for ((m = 0; m < ${#messageArray[@]}; m++)); do
        local currentMessage=${messageArray[m]}
        if [[ ! -z `echo $currentMessage | grep -iE "EXCEPTION|FAIL|FATAL|Could not open|No such file"` ]]; then
            trippedError=1
            __msg err "${currentMessage}"
        elif [[ $trippedError = 1 ]]; then
            __msg err "${currentMessage}"
        elif [[ ! -z `echo $currentMessage | grep -iE "error"` ]]; then
            __msg debug "${currentMessage}"
        elif [[ ! -z `echo $currentMessage | grep -iE "rows loaded|time taken|ok"` ]]; then
            __msg info "${currentMessage}"
        else
            __msg debug "${currentMessage}"
        fi
    done
    if [[ $trippedError = 1 ]]; then
        return 2;
    else
        return 0;
    fi
}
