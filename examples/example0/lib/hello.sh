#!/bin/bash
# hello library

include hive

function hello.world()
{
    local tablename=${1};
    hive.query\
        -hiveconf tablename=$tablename\
        -f sql/hello_world.sql;
}
