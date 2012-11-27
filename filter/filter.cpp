#include "filter.h"

namespace filter {

	bool has_one_keyword(string text)
	{
		static vector<string> keywords;
		if(keywords.size() == 0)
		{
			keywords::load_liberal(keywords);
			keywords::load_conservative(keywords);
		}

		vector<string> found_keywords;

		for(int i = 0; i < keywords.size(); i++)
		{
			if(string::npos != text.find(keywords[i])) found_keywords.push_back(keywords[i]);
		}

		if(found_keywords.size() == 0)
		{
			return false;
		}
		else if(found_keywords.size() == 1)
		{
			return true;
		}
		else
		{
			string longest_keyword = found_keywords[0];
			for(int i = 1; i < found_keywords.size(); i++)
			{
				if(found_keywords[i].length() > longest_keyword.length())
				{
					longest_keyword = found_keywords[i];
				}
			}

			for(int i = 0; i < found_keywords.size(); i++)
			{
				if(string::npos == longest_keyword.find(found_keywords[i]))
				{
					return false;
				}
			}

			return true;
		}
	}

	bool in_deque(int64_t id, deque<int64_t> &ids)
	{
		for(int i = 0; i < ids.size(); i++)
		{
			if(id == ids[i]) return true;
		}
		return false;
	}

	void store_in_deque(int64_t id, deque<int64_t> &ids)
	{
		if(ids.size() >= TWEET_CAP)
		{
			ids.pop_back();
		}

		ids.push_front(id);
	}

}