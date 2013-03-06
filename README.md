Shive: Shell Hive framework
======

Shive is a BASH library that aims to help organize and provide logging for Hadoop Hive project.  Shive is written in BASH with [Bashinator](http://bashinator.org/), a framework that adds organization, console and mail logging and stack tracing to BASH projects.

I wrote Shive because organizing Hive projects is not straight-forward, and neither is setting up logging.  I wanted custom error reports e-mailed to me so I could leave my computer and come back when my jobs finished.  I wanted to import results into MySQL, too.  And a library import system to  write both a standard library and a local library.  And as little boilerplate as possible.  (And I secretly wanted to learn BASH.  This is a decision I later regretted.)

Shive can be treated as a standard library for Hive-specific BASH functions.  In your project directory, you should include a bootstrap script that points to Shive.  Any local library files should be included in a project-specific /lib folder.

A typical shive project is organized:

    .
    |-- shivelib/
    |-- project1/
        |-- bin/
        |-- lib/
        |-- sql/
        |-- bootstrap.sh
    |-- project2/
        |-- bin/
        |-- lib/
        |-- sql/
        |-- bootstrap.sh


BASH  is required to run shive.  Make scripts executable and run.

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
