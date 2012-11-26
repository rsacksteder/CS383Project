/******************************************************
*
* CS383 Project -- Social Media and Political Mood
* Team Vikings -- Gresham, Serendel, and Ryan
*
* Sentiment analysis powered by AlchemyAPI
* Learn more about this service at www.alchemyapi.com
*
* get_sentiment.cpp
*
******************************************************/

#include <vector>
#include <string>

#include "sentiment/sentiment.h"
#include "tweet/tweet.h"
#include "keywords/keywords.h"
#include "config/config.h"
#include "config/logger.h"

using namespace std;

void print_tweet(tweet t)
{
	static vector<string> l_keywords;
	if(l_keywords.size() == 0)
	{
		keywords::load_liberal(l_keywords);
	}

	bool has_liberal_keyword = false;

	for (int i = 0; i < l_keywords.size(); i++)
	{
		if (string::npos != t.m_text.find(l_keywords[i]))
		{
			t.m_liberal = t.m_sentiment * t.m_weight;
			has_liberal_keyword = true;
			break;
		}
	}
	
	if(!has_liberal_keyword)
	{
		t.m_conservative = t.m_sentiment * t.m_weight;
	}

	t.print(tweet::LIBERAL | tweet::CONSERVATIVE);
}

int main(int argc, char **argv)
{
	INFO_LOG << "starting get_sentiment process\n";

	vector<string> all_keywords;

	keywords::load_conservative(all_keywords);
	keywords::load_liberal(all_keywords);

	string line = "";
	sentiment s = sentiment();

	tweet t_done;

	while(getline(cin, line))
	{
		tweet t_in = tweet(line.c_str());

		for(int i = 0; i < all_keywords.size(); i++)
		{
			if(string::npos != t_in.m_text.find(all_keywords[i]))
			{
				s.send_request(t_in, all_keywords[i]);
			}
		}

		s.remaining_requests();

		while(s.pop_tweet(t_done))
		{
			print_tweet(t_done);
		}
	}

	while(s.remaining_requests() > 0)
	{
		while(s.pop_tweet(t_done))
		{
			print_tweet(t_done);
		}
	}

	while(s.pop_tweet(t_done))
	{
		print_tweet(t_done);
	}

	s.cleanup_requests();

	return 0;
}
