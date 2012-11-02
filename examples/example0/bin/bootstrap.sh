#!/bin/bash
# bootstrap and configuration file

export TAGS_DSN='username:password:hostname:database'
export SHIVELIB_PATH='/home/path/to/shivelib'

export PROJECT_PATH=$(dirname $(readlink -f $0))/..

# init config settings
export __ScriptLock=0   # create/check lockfile, default=0

# initialize shiver framework
source $SHIVELIB_PATH/shive.sh
initializeShive

# runtime config settings
# see hivelib/framework/bashinator/config.inc for more config settings
export __PrintDebug=1   # print debug messages, default=1
export __PrintInfo=1    # print info messages, default=1
export __PrintNotice=1  # print notice messages, default=1
export __PrintWarning=1 # print warning messages, default=1
export __PrintErr=1     # print error messages, default=1
export __PrintCrit=1    # print crit messages, default=1
export __MailRecipient='me@example.com'
