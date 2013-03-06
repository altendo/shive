select
    userid,
    adid,
    sum(numClicks),
    year,
    weekofyear
from
    user_ad_day_dump
where
    weekofyear = '${hiveconf:weekofyear}'
group by
    userid,
    adid,
    year,
    weekofyear
