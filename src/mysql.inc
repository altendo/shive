#!/bin/bash
# mysql library

function mysql.useDSN()
{
    if [[ -z $dsnConfigs ]]; then
        __msg err "MySQL DSNs config path not defined.";
        return 2;
    fi
    local dsn=$1;
    if [[ -z $dsn ]]; then
        __msg err "DSN not defined.  Please supply a DSN";
        return 2;
    fi
    export _mysqlDSNUser=$(cat $dsnConfigs | $ApplicationPath/jq -r ".${dsn} | .user");
    export _mysqlDSNPass=$(cat $dsnConfigs | $ApplicationPath/jq -r ".${dsn} | .password");
    export _mysqlDSNHost=$(cat $dsnConfigs | $ApplicationPath/jq -r ".${dsn} | .host");
    export _mysqlDSNDatabase=$(cat $dsnConfigs | $ApplicationPath/jq -r ".${dsn} | .db");
    return 0;
}

function mysql.loadDSN()
{
    # deprecated
    #if [[ -z "${1}" || "${1}" == "" ]]; then
    #    __msg err "MySQL DSN not set.";
    #   return 2;
    #fi
    #local dsn=${1};
    #IFS=':' read -ra DSN <<< "$dsn";
    #export _mysqlDSNUser="${DSN[0]}";
    #export _mysqlDSNPass="${DSN[1]}";
    #export _mysqlDSNHost="${DSN[2]}";
    #export _mysqlDSNDatabase="${DSN[3]}";
    #return 0;
    mysql.useDSN $1;
    return $?;
}

##
# load csv data to mysql table.  columns MUST match.
# params:
#   0:  -f  force (optional).  forces csv loaded to mysql if 0 rows
#   1:  destination (required).  target MySQL table to load csv data to.
function mysql.load()
{
    let force=0;
    if [[ "${1}" = "-f" ]]; then
        let force=1; shift;
    fi

    local destination=${1}
    local tmpfile=$(mktemp /tmp/shive.XXXXXXXXXX)
    local -i n=0
    while read; do
        echo -e $REPLY >> $tmpfile
        n=n+1
    done

    # check we have rows before continuing
    if [[ "${n}" = "0" && "${force}" = "0" ]]; then
        rm -rf $tmpfile;
        __msg err "No data to load to MySQL.";
        return 2;
    fi

    # note: echo $REPLY converts HIVE \t to whitespace. need to reconvert to tabs.
    sed -i 's/\s/\t/g' $tmpfile
    mysql.query "
        LOAD DATA LOCAL INFILE '${tmpfile}' REPLACE INTO TABLE ${destination}
        FIELDS TERMINATED BY '\t' LINES TERMINATED BY '\n';
        " || __msg err;
    local ret=${PIPESTATUS[0]};

    rm -rf $tmpfile;
    if [ "$ret" = "0" ]; then
        __msg info "Loaded ${n} rows to ${destination}."
        return 0;
    else
        return $ret;
    fi
}

function mysql.truncate()
{
    local table=${1};
    mysql.query "truncate ${table}" || return 2;
    return 0;
}

function mysql.query()
{
    local query="${1}";
    {
        echo "$query" | mysql\
            --skip-column-names\
            -h "${_mysqlDSNHost}"\
            -D "${_mysqlDSNDatabase}"\
            -u "${_mysqlDSNUser}"\
            --password="${_mysqlDSNPass}"\
            3>&1 1>&2 2>&3\
            | mysql.verifyOutput\
            2>&1
    } 3>&1 1>&2 2>&3
    return $?;
}

function mysql.verifyOutput()
{
    local -i trippedError=0;
    local message=$@;
    while read; do
        local message="${REPLY}"
        if [[ ! -z $(echo $message | grep -iE "ERROR") ]]; then
            __msg err "${message}";
            trippedError=2;
        fi
    done
    return $trippedError;
}
