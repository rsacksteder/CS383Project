/******************************************************
*
* CS383 Project -- Social Media and Political Mood
* Team Vikings -- Gresham, Serendel, and Ryan
*
* Sentiment analysis powered by AlchemyAPI
* Learn more about this service at www.alchemyapi.com
*
* sentiment.cpp
*
******************************************************/

#include "sentiment.h"

bool sentiment::m_use_alchemy = !USE_SENTIMENT140;
queue<tweet> sentiment::m_processed_tweets;
vector<tweet*> sentiment::m_tweet_vector;

size_t sentiment::alchemy_write_function(char *data, size_t size, size_t nmemb, void *usrdata)
{
	double sentiment;

	Json::Value root;
	Json::Reader reader;
	if(data == NULL || !reader.parse(data, root))
	{
		ERROR_LOG << "failed to decode alchemy JSON!\n" << reader.getFormattedErrorMessages() << data << "\n\n";
		sentiment = 0;
	}
	else if(root["status"].asString() == "ERROR")
	{
		if(root["statusInfo"].asString() == "unsupported-text-language")
		{
			ERROR_LOG << "alchemy error: unsupported-text-language (" << root["language"].asString() << ")\n";
		}
		else if(root["statusInfo"].asString() == "daily-transaction-limit-exceeded")
		{
			ERROR_LOG << "alchemy error: " << root["statusInfo"].asString() << "\n";
			WEB_LOG("Uhoh...", "Looks like you hit the daily Alchemy API request limit.", "error");
		}
		else
		{
			ERROR_LOG << "alchemy error: " << root["statusInfo"].asString() << "\n";
		}

		sentiment = 0;
	}
	else
	{
		INFO_LOG << "alchemy success\n";
		sentiment = atof(root["docSentiment"]["score"].asString().c_str());
	}

	tweet *t = (tweet*)usrdata;
	push_tweet(t, sentiment);

	return size * nmemb;
}

size_t sentiment::sentiment140_write_function(char *data, size_t size, size_t nmemb, void *usrdata)
{
	double sentiment;

	Json::Value root;
	Json::Reader reader;
	if(data == NULL || !reader.parse(data, root))
	{
		ERROR_LOG << "failed to decode sentiment140 JSON!\n" << reader.getFormattedErrorMessages() << data << "\n\n";
		sentiment = 0;
	}
	else
	{
		INFO_LOG << "sentiment140 success\n";
		sentiment = root["results"]["polarity"].asDouble();
		sentiment -= 2.0;
		sentiment /= 2.0;
	}

	tweet *t = (tweet*)usrdata;
	push_tweet(t, sentiment);

	return size * nmemb;
}


sentiment::sentiment()
{
	m_curl = curl_multi_init();
	m_requests = 0;
}

sentiment::~sentiment()
{
	curl_multi_cleanup(m_curl);
}

void sentiment::push_tweet(tweet* t_ptr, double sentiment)
{
	tweet t = tweet(*t_ptr);
	
	for(int i = 0; i < m_tweet_vector.size(); i++)
	{
		if(t_ptr == m_tweet_vector[i])
		{
			m_tweet_vector.erase(m_tweet_vector.begin() + i);
		}
	}
	delete t_ptr;
	t.m_sentiment = sentiment;
	m_processed_tweets.push(t);
}

void sentiment::send_request(tweet t, string subject)
{
	#if FAKE_SENTIMENT

	double sentiment;
	sentiment = (double) ((rand() % 200000) - 100000) / 100000.0;

	tweet *t_ptr = new tweet(t);
	push_tweet(t_ptr, sentiment);

	#else

	tweet *t_ptr = new tweet(t);
	m_tweet_vector.push_back(t_ptr);

	CURL* curl = curl_easy_init();

	if(curl)
	{
		string url = "";

		if(m_use_alchemy)
		{
			url += "http://access.alchemyapi.com/calls/text/TextGetTargetedSentiment?apikey="ALCHEMY_KEY"&outputMode=json&text=";
			url += curl_easy_escape(curl, t.m_text.c_str(), t.m_text.length());
			url += "&target=";
		}
		else // use sentiment140
		{
			url += "http://www.sentiment140.com/api/classify?text=";
			url += curl_easy_escape(curl, t.m_text.c_str(), t.m_text.length());
			url += "&query=";
		}

		url += curl_easy_escape(curl, subject.c_str(), subject.length());

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, t_ptr);

		if(m_use_alchemy)
		{
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, alchemy_write_function);
		}
		else
		{
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, sentiment140_write_function);
		}

		curl_multi_add_handle(m_curl, curl);
		curl_multi_perform(m_curl, &m_requests);
	}

	#endif
}

bool sentiment::pop_tweet(tweet &t)
{
	if(m_processed_tweets.size() > 0)
	{
		t = m_processed_tweets.front();	// grabs the tweet at the front
		m_processed_tweets.pop();		// removes that tweet from the queue
		return true;
	}
	else
	{
		return false;
	}
}

int sentiment::remaining_requests()
{
	curl_multi_perform(m_curl, &m_requests);

	cleanup_requests();

	return m_requests;
}

void sentiment::cleanup_requests()
{
	int msg_count;

	CURLMsg *msg = curl_multi_info_read(m_curl, &msg_count);

	while(msg != NULL)
	{
		curl_multi_remove_handle(m_curl, msg->easy_handle);
		curl_easy_cleanup(msg->easy_handle);
		msg = curl_multi_info_read(m_curl, &msg_count);
	}
}