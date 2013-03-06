select
    adid,
    country,
    age,
    gender,
    numClicks,
    year,
    month,
    logdate
from
    ad_demographics_day_dump
where
    year = '${hiveconf:year}'
    and month = '${hiveconf:month}'
    and logdate = '${hiveconf:date}'
