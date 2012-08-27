Shiver: Shell HIVE framework
======

Shiver is a HIVE project organization framework built on [Bashinator](http://www.bashinator.org/) to leverage its logging, mailing and stack trace features.  The goal of Shiver is to write light-weight HIVE jobs with as little boilerplate as possible.

Executing Shiver jobs
-----
Shiver jobs can be as simple as

    #!/bin/bash
    . "/path/to/shiver"

    hive.shiver\
        -e "explain hello_world"

Or as complex as

    #!/bin/bash
    . "/path/to/shiver"

    include utils

    utils.describe hello_world

with library file utils.inc.

    function utils.describe() {
        local $tableName=${1}
        if ! hive.query\
            -hiveconf tableName=$tableName\
            -f describe.ql; then
            return 2;
        return 0;
    }

with query file utils/describe.ql

    describe
        ${hiveconf:tableName};

Conventions
-----
Shiver naming conventions are based on [Bashinator](http://www.bashinator.org/docs/bashinator-20090610.pdf) conventions.

* Functions are named in lowerCamelCase
* Global variables are named in UpperCamelCase
* Local variables are named in lowerCamelCase
* Bashinator functions and global variables begin with a double underscore
* Functions that print their results to stdout are named “printSomething”
* Return codes of functions and exit codes of scripts based on Bashinator:
    * 0, positive check result (for check functions only) or successful task, completion (for all other functions)
    * 1, negative check result (for check functions only, undefined for others)
    * 2, error (for any function)
* Variables are always initialized (with type where possible) before use
* Functions shouldn't ever terminate the script execution on their own, instead, they should return with an error return code so the calling function can take appropriate action.

References
-----
* [Bashinator documentation](http://www.bashinator.org/docs/bashinator-20090610.pdf) for style guide 
* [HIVE variable substitution](http://hive.apache.org/docs/r0.9.0/language_manual/var_substitution.html)
