select
    adid,
    country,
    age,
    gender,
    sum(numClicks),
    year,
    weekofyear
from
    ad_demographics_day_dump
where
    weekofyear = '${hiveconf:weekofyear}'
group by
    adid,
    country,
    age,
    gender,
    year,
    weekofyear
