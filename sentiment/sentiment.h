/******************************************************
*
* CS383 Project -- Social Media and Political Mood
* Team Vikings -- Gresham, Serendel, and Ryan
*
* Sentiment analysis powered by AlchemyAPI
* Learn more about this service at www.alchemyapi.com
*
* sentiment.h
*
******************************************************/

#ifndef SENTIMENT_H
#define SENTIMENT_H

#include <iostream>
#include <string>
#include <stdlib.h> // for atof
#include <queue>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>

#include "tweet/tweet.h"
#include "config/config.h"
#include "config/logger.h"

using namespace std;


class sentiment {

private:

	static size_t alchemy_write_function(char *data, size_t size, size_t nmemb, void *usrdata);
	static size_t sentiment140_write_function(char *data, size_t size, size_t nmemb, void *usrdata);

	static void push_tweet(tweet *t, double sentiment);

	CURLM *m_curl;
	int m_requests;
	
	static bool m_use_alchemy;
	static queue<tweet> m_processed_tweets;
	static vector<tweet*> m_tweet_vector;

public:

	void send_request(tweet t, string subject);
	bool pop_tweet(tweet &t);
	int remaining_requests();
	void cleanup_requests();

	sentiment();
	~sentiment();
};

#endif
