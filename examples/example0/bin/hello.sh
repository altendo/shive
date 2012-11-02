#!/bin/bash
# hello world

# include the library you want to use
# retrieves from shive library
# or local library (ie project_dir/lib)
include hive

# execute hive queries inline
hive.query\
    -e "describe hello_world";

# execute hive queries with hiveconf vars (hive 0.60 required)
hive.query\
    -hiveconf tablename='hello_world'\
    -e "describe ${hiveconf:tablename}";

# execute hive queries with hiveconf vars from file
hive.query\
    -hiveconf tablename='hello_world'\
    -f sql/hello_world.sql;

# invoke local library file (from project_dir/lib)
include hello

# call function from library file
hello.world 'hello_world'
