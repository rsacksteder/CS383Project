#ifndef CONFIG_H
#define CONFIG_H

// if false, it will use Alchemy
#define USE_SENTIMENT140 (true)
// and you will need an Alchemy API key
#define ALCHEMY_KEY "3ab48469cc894436ef2ea8bf8f7bf76d8abb7cab"

// randomly generated sentement
#define FAKE_SENTIMENT (false)

// user provided twitter username and password
#define TWITTER_ACCOUNT "vikings383:383vikings"

// number of tweets to be aggregated for the simple mood gauge
#define TWEET_CAP (250)
// average number of followers per twitter user; statistical used value for weighting
#define AVG_FOLLOWERS (208) // source: http://www.beevolve.com/twitter-statistics/
// defines the minimum weight required for a "popular tweet" output notification
#define POPULAR_LIMIT (250000)

// offset from GMT; -8 for Pacific Time
#define TIME_ZONE_OFFSET (-8)

// do you want csv output
#define CSV_OUTPUT (true)

// seconds between recalculating daily average, and saving current gauge value to csv
#define UPDATE_AVG_INTERVAL (30)

// seconds between webpage graph updates
#define GRAPH_UPDATE_INTERVAL (18)

#endif