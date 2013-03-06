##
# user ads reports
# aggregate and summarize 1) ads users clicked on 2) targeted ad demos 
# over a) daily period b) weekly period c) monthly period
source $(dirname $0)/../bootstrap.sh;

include mysql;
include userAds;

# if date provided in argument, process cron for that date instead
declare date=$(date +%Y-%m-%d);
if [ ! -z ${1} ]; then
    date=${1};
fi

# initialize mysql
mysql.loadDSN "$MY_DSN";

# intialize dates for yesterday
declare yesterday=$(date +%Y-%m-%d -d"1 day ago ${date}");
declare today=$(date +%Y-%m-%d -d"${date}");
declare yesterweek=$(date +%V -d"1 day ago ${date}");
declare todweek=$(date +%V -d"${date}");
declare yestermonth=$(date +%m -d"1 day ago ${date}");
declare tomonth=$(date +%m -d"${date}");

__msg info "Generating page ads report for ${yesterday}.";

if ! checkAdsDateExists $yesterday; then
    __die err "ads table has no partition for ${yesterday}; exiting.";
fi

# dump yesterday's GUTS logs / extract radio logs
dumpAdsDate $yesterday;
if [ "${?}" = "2" ]; then
    __die err "ads table failed to dump; exiting";
fi

# summarize daily user tags
summarizeUserAdsOverPeriod "daily" "$yesterday";
if [ "${?}" = "2" ]; then
    __msg err "userAds for $yesterday failed to load.";
fi

summarizeAdDemosOverPeriod "daily" "$yesterday";
if [ "${?}" = "2" ]; then
    __msg err "adDemos for $yesterday failed to load.";
fi

# determine whether a week period has passed
# if so, export weekly summary
if [ "$yesterweek" != "$toweek" ]; then
    summarizeUserAdsOverPeriod "weekly" "$yesterday";
    summarizeAdDemosOverPeriod "weekly" "$yesterday";
fi

# determine whether a month period has passed
# if so, export monthly summary
if [ "$yestermonth" != "$tomonth" ]; then
    summarizeUserAdsOverPeriod "monthly" "$yesterday";
    summarizeAdDemosOverPeriod "monthly" "$yesterday";
fi

exit 0;
