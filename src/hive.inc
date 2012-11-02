##
# HIVE extention

##
# Query HIVE and assign the magical path to a query file, if found
#
function hive.query()
{
    ## find query file, if given, and replace query path for hive call
#    local queryCaller=$(caller 0 | cut -d ' ' -f 3)
#   local queryPath=${queryCaller:0:${#queryCaller}-4}
#   local args=''
#   local file=false
#   while [ -n "$1" ]
#   do
#       arg=$1
#       if [ "${file}" = true ]; then
#           arg="${queryPath}/${arg}"
#           if [ ! -f $arg ]; then
#               __msg err "Query File ${1} at ${queryPath} does not exist."
#               return 2;
#           fi
#           file=false
#       fi
#       if [ "${arg}" = "-f" ]; then
#           file=true
#       fi
#       args="${args} ${arg}"
#       shift
#   done
#   ## strip any newlines the calling function may have put
#   args=`echo -n $args | tr -d "\n"`
    local args="${@}"
    ## finally call hive
    __msg info "hive ${args}"
    hive\
        -hiveconf hive.root.logger=ERROR,console\
        -hiveconf transformPath=$queryPath\
        "${@}"\
        3>&1 1>&2 2>&3\
        | hive.verifyOutput
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
function hive.verifyOutput()
{
    local -i trippedError=0
    local message=$@
    while read; do
       messageArray+=( "${REPLY}" )
        local currentMessage="${REPLY}"
        if [[ ! -z `echo $currentMessage | grep -iE "EXCEPTION|FAIL|FATAL|Can't open|Could not open|No such file"` ]]; then
            trippedError=1
            __msg err "${currentMessage}"
        elif [[ $trippedError = 1 ]]; then
            __msg err "${currentMessage}"
        elif [[ ! -z `echo $currentMessage | grep -iE "error"` ]]; then
            __msg debug "${currentMessage}"
        elif [[ ! -z `echo $currentMessage | grep -iE "rows loaded|time taken|ok|loading data"` ]]; then
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


hive.getLatestPartition()
{
    echo `hive -e "show partitions db_replication_$1" 2> /dev/null | tail -n1 | cut -d "=" -f 2`
}
