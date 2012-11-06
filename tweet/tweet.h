#ifndef TWEET_H
#define TWEET_H

#include <iostream>
#include <string>
#include <stdint.h>
#include <stdlib.h>
#include <jsoncpp/json/json.h>

#include "config/config.h"

using namespace std;

class tweet {
private:
	string filter(string text);

public:

	enum fields {
		NONE			= 0,
		ID 				= (1u << 0),
		TEXT 			= (1u << 1),
		LIBERAL 		= (1u << 2),
		CONSERVATIVE 	= (1u << 3),
		WEIGHT			= (1u << 4),
		FOLLOWERS 		= (1u << 5),
		RETWEETS 		= (1u << 6),
		IS_RETWEET 		= (1u << 7),
		ORIGINAL_ID 	= (1u << 8)
	};

	string m_text;
	double m_liberal;
	double m_conservative;
	int m_weight;
	int64_t m_id;
	int64_t m_original_id;
	int m_followers;
	int m_retweets;
	bool m_is_retweet;

	tweet(const char *json_data);
	void print(unsigned int fields);
};

#endif