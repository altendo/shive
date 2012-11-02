#!/bin/bash
# mysql library

function mysql.loadDSN()
{
    local dsn=${1}
    IFS=':' read -ra DSN <<< "$dsn"
    export _mysqlDSNUser="${DSN[0]}"
    export _mysqlDSNPass="${DSN[1]}"
    export _mysqlDSNHost="${DSN[2]}"
    export _mysqlDSNDatabase="${DSN[3]}"
}

mysql.load()
{
    local destination=${1}
    local tmpfile=$(mktemp shive.XXXXXXXXXX)
    local -i n=0
    while read; do
        echo -e $REPLY >> $tmpfile
        n=n+1
    done
    # note: echo $REPLY converts HIVE \t to whitespace. need to reconvert to tabs.
    sed -i 's/\s/\t/g' $tmpfile
    mysql.query "
        TRUNCATE TABLE ${destination};
        LOAD DATA LOCAL INFILE '${tmpfile}' REPLACE INTO TABLE ${destination}
        FIELDS TERMINATED BY '\t' LINES TERMINATED BY '\n';
        ";
    __msg info "Loaded ${n} rows to ${destination}."
    rm -rf $tmpfile
}

mysql.query()
{
    local query=${1}
    echo $query | mysql\
        -h "${_mysqlDSNHost}"\
        -D "${_mysqlDSNDatabase}"\
        -u "${_mysqlDSNUser}"\
        --password="${_mysqlDSNPass}"
}
