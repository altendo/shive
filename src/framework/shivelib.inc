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
            __msg debug "Attempting to open $ApplicationPath/$ShiveInc."
            source "$ApplicationPath/$ShiveInc"
        fi
        #    3>&1 1>&2 2>&3 | __msg err
    fi
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
