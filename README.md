Shive: Shell HIVE framework
======

Shive is a Hadoop/HIVE library framework built on [Bashinator](http://www.bashinator.org/) to bring organization, standardized logging, mailing and stack tracing to HIVE projects.  The goal of shive is to write light-weight HIVE jobs with as little boilerplate as possible.  Currently, shive includes utility functions to query HIVE and MySQL.  See examples directory to use shive in a project.

Shive *must* be executed with BASH.  To initialize and run a shive script, be sure to chmod and execute:

    chmod +x project/bin/script.sh
    ./project/bin/script.sh

Conventions
-----
Shive naming conventions are based on [Bashinator](http://www.bashinator.org/docs/bashinator-20090610.pdf) conventions.

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
