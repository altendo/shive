##
# user ads report
# initialization script -- 
# initializes hive schema, mysql schema, and fills in the dates
source $(dirname $0)/../bootstrap.sh;

include hive;
include mysql;

# default start time
declare startDate='2013-01-01';

# if argument provided, use that start time
if [ ! -z ${1} ]; then
    startDate=${1};
fi

__msg info "Initializing reports from ${startDate}.";

# calulate number of days between dates
startTime=$(date +%s -d "${startDate}");
endTime=$(date +%s);
diffDays=$(( (endTime - startTime) / 3600 / 24 ));

# initialize databases + tables
hive -f $sqlpath/schema.sql;
#cat $sqlpath/mysqlSchema.sql | mysql.query;

# run script for each day
declare -i days=$diffDays;
while [ $days -gt 0 ]; do
    date=$(date +%Y-%m-%d -d "${days} days ago");
    __msg info "Initializing ${date}, ${days} days to go.";
    let days=days-1

    # launch cron for this date
    $projectPath/bin/userAdsCron.sh ${date};
    if [ "${?}" = "0" ]; then
        __msg info "Successfully initialized ${date}.";
    else
        __msg err "Error initializing ${date}!";
    fi
done

exit 0;
