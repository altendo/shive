#!/bin/bash
# mysql library

function mysql.loadDSN()
{
    local dsn=${1};
    IFS=':' read -ra DSN <<< "$dsn";
    export _mysqlDSNUser="${DSN[0]}";
    export _mysqlDSNPass="${DSN[1]}";
    export _mysqlDSNHost="${DSN[2]}";
    export _mysqlDSNDatabase="${DSN[3]}";
    return 0;
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
        TRUNCATE TABLE ${destination};
        LOAD DATA LOCAL INFILE '${tmpfile}' REPLACE INTO TABLE ${destination}
        FIELDS TERMINATED BY '\t' LINES TERMINATED BY '\n';
        ";

    rm -rf $tmpfile
    __msg info "Loaded ${n} rows to ${destination}."
    return 0;
}

function mysql.query()
{
    local query=${1}
    echo $query | mysql\
        -h "${_mysqlDSNHost}"\
        -D "${_mysqlDSNDatabase}"\
        -u "${_mysqlDSNUser}"\
        --password="${_mysqlDSNPass}"
    return 0;
}
