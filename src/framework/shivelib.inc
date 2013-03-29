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

    local includeSuccess=0;
    if [ "$ShiveSourceExists" = false ]; then
        if [ -e "$ShiveLocalLib/$ShiveInc" ]; then
            __msg debug "Attempting to open $ShiveLocalLib/$ShiveInc."
            if source "$ShiveLocalLib/$ShiveInc"; then
                includeSuccess=1;
            fi
        elif [ -e "$ShiveLocalLib/$ShiveIncAlt" ]; then
            __msg debug "Attempting to open $ShiveLocalLib/$ShiveIncAlt."
            if source "$ShiveLocalLib/$ShiveIncAlt"; then
                includeSuccess=1;
            fi
        elif [ -e "$ApplicationPath/$ShiveInc" ]; then
            __msg debug "Attempting to open $ApplicationPath/$ShiveInc."
            if source "$ApplicationPath/$ShiveInc"; then
                includeSuccess=1;
            fi
        else
            __die err "$ShiveClassname library could not be found; exiting.";
        fi
        #    3>&1 1>&2 2>&3 | __msg err
    fi
    return $includeSuccess;
}

# Bashinator functions
function __init()
{
    if [[ "${1}" = '-p' ]]; then
        shift;
        export ShiveHiveQueryPriority=${1}; shift;
    fi
    if [[ "${1}" = '-q' ]]; then
        shift;
        export __MsgQuiet=1;
    fi
    return 0;
}

function __main()
{
    # SOME HACKISH SHIT RIGHT HERE
    source $ShiveCallFile "${@}"
    return 0;
}
