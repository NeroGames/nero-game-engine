////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/cpp/utility/String.h>
//Cpp
#include <fstream>
////////////////////////////////////////////////////////////
namespace nero
{
	namespace string
	{
		std::string formatString(const std::string& input, Format format)
		{
			using namespace boost::algorithm;

			std::string result = input;

			switch (format)
			{
				case Format::SNAKE_CASE_LOWER:
				{
					trim_left(result);
					trim_right(result);
					to_lower(result);
					replace_all(result, string::StringPool.SPACE, string::StringPool.UNDERSCORE);

				}break;

				case Format::COMPACT_LOWER:
				{
					trim_left(result);
					trim_right(result);
					to_lower(result);
					replace_all(result, string::StringPool.SPACE, string::StringPool.BLANK);

				}break;
			}

			return result;
		}

		std::vector<std::string> &splitString(const std::string &s, char delim,std::vector<std::string> &elems)
		{
			std::stringstream ss(s);
			std::string item;
			while (std::getline(ss, item, delim))
			{
				if (item.length() > 0)
				{
					elems.push_back(item);
				}
			}

			return elems;
		}

		std::vector<std::string> splitString(const std::string &s, char delim)
		{
			std::vector<std::string> elems;
			splitString(s, delim, elems);

			return elems;
		}

		void fillCharTable(const char** charTab, const std::vector<std::string>& stringTab)
		{
			for(std::size_t i = 0; i < stringTab.size(); i++)
			{
				charTab[i] = const_cast<const char*>(stringTab[i].c_str());
			}
		};

		void fillCharTable(char** charTab, const std::vector<std::string>& stringTab)
		{
			for(std::size_t i = 0; i < stringTab.size(); i++)
			{
				charTab[i] = const_cast<char*>(stringTab[i].c_str());
			}
		};

		void fillCharArray(char* charArray, int arraySize, const std::string& string)
		{
			strncpy(charArray, string.c_str(), arraySize - 1);
			charArray[arraySize - 1] = 0;
		}

		std::vector<std::string> getWordTable(std::string string)
		{
			using namespace boost::algorithm;

			trim_left(string);
			trim_right(string);
			replace_all(string, string::StringPool.NEW_LINE, string::StringPool.BLANK);

			auto result = splitString(string, string::StringPool.SPACE_CHAR);

			std::remove_if(result.begin(), result.end(), [](std::string word){return word == string::StringPool.BLANK;});

			return result;
		}

		std::string wrapString(const std::string& message, size_t maxLetter)
		{
			if(message.size() <= maxLetter)
			{
				return message;
			}

			std::string result = string::StringPool.BLANK;

			auto wordTable = getWordTable(message);

			std::string line = string::StringPool.BLANK;
			for(const std::string& word : wordTable)
			{
				if((line.size() + word.size() + 1) <= maxLetter)
				{
					line += word + string::StringPool.SPACE;
				}
				else
				{
					result += line + string::StringPool.NEW_LINE;
					line = word + string::StringPool.SPACE;
				}
			}

			result += line;

			return result;
		};

		std::string escapeAntiSlash(const std::string& word)
		{
			return boost::algorithm::replace_all_copy(word, "\\", "\\\\");
		}
	}
}
