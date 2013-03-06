#!/bin/bash
# bootstrap and configuration file

export MY_DSN='user:p@$$w0rD!:127.0.0.1:db';

function projectPath() {
    echo $(dirname $(readlink -f $(caller 0 | cut -d ' ' -f 3)));
}

export projectPath=$(projectPath);
export sqlpath=$projectPath/sql;
export shivelibPath=$projectPath/lib/shivelib;

# init config settings
export __ScriptLock=0   # create/check lockfile, default=0

# initialize shiver framework
source $shivelibPath/shive.sh;

# runtime config settings
# see hivelib/framework/bashinator/config.inc for more config settings
export __PrintDebug=1   # print debug messages, default=1
export __PrintInfo=1    # print info messages, default=1
export __PrintNotice=1  # print notice messages, default=1
export __PrintWarning=1 # print warning messages, default=1
export __PrintErr=1     # print error messages, default=1
export __PrintCrit=1    # print crit messages, default=1

export __MailRecipient='jkl@asdf.com'
export __MailDebug=0    # email debug messages, default=0
export __MailInfo=1     # email info messages, default=0
export __MailNotice=1   # email notice messages, default=0
export __MailWarning=1  # email warning messages, default=0
export __MailErr=1      # email error messages, default=0
export __MailCrit=1     # email crit messages, default=0
export __MailAlert=1    # email alert messages, default=0
