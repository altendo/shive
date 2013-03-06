select
    userid,
    adid,
    numClicks,
    year,
    month,
    logdate
from
    user_ad_day_dump
where
    year = '${hiveconf:year}'
    and month = '${hiveconf:month}'
    and logdate = '${hiveconf:date}'
