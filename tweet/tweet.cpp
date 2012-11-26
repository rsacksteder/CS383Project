#include "tweet.h"

tweet::tweet() {}

tweet::tweet(const tweet &t)
{
	m_text = t.m_text;
	m_liberal = t.m_liberal;
	m_conservative = t.m_conservative;
	m_sentiment = t.m_sentiment;
	m_weight = t.m_weight;
	m_id = t.m_id;
	m_original_id = t.m_original_id;
	m_followers = t.m_followers;
	m_retweets = t.m_retweets;
	m_is_retweet = t.m_is_retweet;
	m_lang = t.m_lang;
}

// constructor (takes json data)
tweet::tweet(const char *json_data)
{
	Json::Value root;
	Json::Reader reader;

	if(json_data == NULL || !reader.parse(json_data, root))
	{
		ERROR_LOG << "failed to decode tweet JSON!\n" << reader.getFormattedErrorMessages() << json_data << "\n\n";
		return;
	}
	else
	{
		if(root["warning"].get("code", "").asString() == "FALLING_BEHIND")
		{
			ERROR_LOG << "lagging behind twitter (" << root["warning"].get("percent_full", 0).asInt() << ")\n";
			WEB_LOG("Uh-oh", "Looks like the application is having a hard time keeping up with all the tweets.", "warning");
			m_id = -1;
		}
		else
		{
			m_id = root.get("id", 0).asInt64();
			m_text = filter(root.get("text", "").asString());
			m_sentiment = 0;

			// is this json straight from twitter? (ie does it have the created_at field?)
			if(root.get("created_at", "").asString().length() > 0)
			{
				m_lang = root["user"].get("lang", "").asString();
				m_followers = root["user"].get("followers_count", 0).asInt();
				m_retweets = root.get("retweet_count", 0).asInt();
				m_is_retweet = !root["retweeted_status"].empty();
				if(m_is_retweet)
				{
					m_original_id = root["retweeted_status"].get("id", 0).asInt64();
				}
				else
				{
					m_original_id = 0;
				}
			}
			else // internal json
			{
				m_lang = root.get("language", "").asString();
				m_followers = root.get("followers", 0).asInt();
				m_retweets = root.get("retweets", 0).asInt();
				m_weight = root.get("weight", 0).asInt();
				m_conservative = root.get("conservative", 0).asDouble();
				m_liberal = root.get("liberal", 0).asDouble();
				m_is_retweet = root.get("is_retweet", false).asBool();
				m_original_id = root.get("original_id", 0).asInt64();
			}

		}
	}
}

// convert a string to lowercase, replace newlines with spaces,
// and replace single quotes with double quotes
string tweet::filter(string text)
{
	for(int i = 0; i < text.length(); ++i)
	{
		text[i] = tolower(text[i]);
		if(text[i] == '\n') text[i] = ' ';
		if(text[i] == '"') text[i] = '\'';
		if(text[i] == '\\') text[i] = '/';
	}

	return text;
}

// print the json of the tweet
void tweet::print(unsigned int fields)
{
	cout << "{";

	cout << "\"id\":" << m_id;

	if(fields & TEXT)
		cout << ",\"text\":\"" << m_text << "\"";
	if(fields & LIBERAL)
		cout << ",\"liberal\":" << m_liberal;
	if(fields & CONSERVATIVE)
		cout << ",\"conservative\":" << m_conservative;
	if(fields & WEIGHT)
		cout << ",\"weight\":" << m_weight;
	if(fields & FOLLOWERS)
		cout << ",\"followers\":" << m_followers;
	if(fields & RETWEETS)
		cout << ",\"retweets\":" << m_retweets;
	if(fields & IS_RETWEET)
		cout << ",\"is_retweet\":" << m_is_retweet;
	if(fields & ORIGINAL_ID)
		cout << ",\"original_id\":" << m_original_id;
	if(fields & LANGUAGE)
		cout << ",\"language\":\"" << m_lang << "\"";

	cout << "}\n" << flush;
}