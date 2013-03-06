##
# user ads lib
# calculates # clicks per ad for each user (userAds)
# calculates # clicks per ad for each demographic (adDemos)

include hive;
include mysql;
include util;

##
# streams functions

# check streams partitions
function checkAdDateExists()
{
    local date=${1};
    local year=$(date +%Y -d "${date}");
    local month=$(date +%-m -d "${date}");
    local result=$(hive "select * from ads where year=$year and month=$month and date='${date}' limit 1" 2> /dev/null);
    if [ "$result" = "" ]; then
        return 2;
    fi
    return 0;
}

# dump ads table to userAdsDay and adDemoDay
function dumpAdsDate()
{
    local date=${1};
    __msg info "dumping ads for ${date}";
    hive.query\
        -hiveconf year=$(date +%Y -d "${date}")\
        -hiveconf month=$(date +%-m -d "${date}")\
        -hiveconf weekofyear=$(date +%V -d "${date}")\
        -hiveconf date=$date\
        -f $sqlpath/userAds/dumpAdsDate.sql || return 2;
    if ! checkUserAdsDayExists $date; then
        __msg err "saving userAds to dump table failed.";
        return 2;
    fi
    return 0;
}

##
# userAds functions

# check userAds partition
function checkUserAdsDayExists()
{
    local date=${1};
    local year=$(date +%Y -d "${date}");
    local month=$(date +%-m -d "${date}");
    local weekofyear=$(date +%V -d "${date}");
    local table=user_ads_day_dump/year=$year/month=$month/weekofyear=$weekofyear/logdate=$date;
    local modDate=$(hive.tableModified $table);
    if [ "$modDate" = "" ]; then
        return 2;
    fi
    return 0;
}

# sum userAds over a period
function summarizeUserAdsOverPeriod()
{
    local period=$(util.ucfirst ${1});  # [daily, weekly, monthly]
    local date=${2};                    # YYYY-MM-DD
    __msg info "summarizing userAds over ${period} for ${date}";
    local sqlfile=$sqlpath/userAds/summarizeUserAds${period}.sql;
    if [ ! -f $sqlfile ]; then
        __msg err "SQL process userAds over ${period} not found: ${sqlfile}";
        return 2;
    fi
    local mysqlTable="UserAds${period}Summary";
    hive.query\
        -hiveconf year=$(date +%Y -d "${date}")\
        -hiveconf month=$(date +%-m -d "${date}")\
        -hiveconf weekofyear=$(date +%V -d "${date}")\
        -hiveconf date=$date\
        -f $sqlfile | mysql.load $mysqlTable;
    if [ "${PIPESTATUS[1]}" = "2" ]; then
        __msg err "Loading userAds over ${period} summary to MySQL failed.";
        return 2;
    elif [ "${PIPESTATUS[0]}" = "2" ]; then
        __msg err "Running hive query for userAds over {$period} summary failed.";
    fi
    return 0;
}

##
# adDemo functions

# check tagDemo partition
function checkAdDemosExists()
{
    local date=${1};
    local year=$(date +%Y -d "${date}");
    local month=$(date +%-m -d "${date}");
    local weekofyear=$(date +%V -d "${date}");
    local table=user_ad_day_dump/year=$year/month=$month/weekofyear=$weekofyear/logdate=$date;
    local modDate=$(hive.tableModified $table);
    if [ "$modDate" = "" ]; then
        return 2;
    fi
    return 0;
}

# sum tagDemos over a period
function summarizeAdDemosOverPeriod()
{
    local period=$(util.ucfirst ${1});  # [daily, weekly, monthly]
    local date=${2};                    # YYYY-MM-DD
    __msg info "summarizing adDemos over ${period} for ${date}";
    local sqlfile=$sqlpath/userAds/summarizeAdDemographics${period}.sql;
    if [ ! -f $sqlfile ]; then
        __msg err "SQL process adDemos over ${period} not found: ${sqlfile}";
        return 2;
    fi
    local mysqlTable="AdDemographics${period}Summary";
    hive.query\
        -hiveconf year=$(date +%Y -d "${date}")\
        -hiveconf month=$(date +%-m -d "${date}")\
        -hiveconf weekofyear=$(date +%V -d "${date}")\
        -hiveconf date=$date\
        -f $sqlfile | mysql.load $mysqlTable;
    if [ "${PIPESTATUS[1]}" = "2" ]; then
        __msg err "Loading adDemos over ${period} summary to MySQL failed.";
        return 2;
    elif [ "${PIPESTATUS[0]}" = "2" ]; then
        __msg err "Running hive query for adDemos over {$period} summary failed.";
    fi
    return 0;
}
