////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SETTING_H
#define SETTING_H
///////////////////////////HEADERS///////////////////////////
//JSON
#include <json/json.hpp>
//STD
#include <string>
#include <vector>
#include <memory>
/////////////////////////////////////////////////////////////

namespace nero
{
	class Setting
	{
		public:
			typedef std::shared_ptr<Setting>    Ptr;

		public: //constructor
											Setting();

		public:
			void							setDirectory(const std::string& directory);
			void							loadSetting(const std::string& setting);
			void							load();
			//get primitive
			unsigned int					getUInt(const std::string& setting) const;
			int								getInt(const std::string& setting) const;
			float							getFloat(const std::string& setting) const;
			bool							getBool(const std::string& setting) const;
			std::string						getString(const std::string& setting) const;
			//get table
			std::vector<std::string>		getStringTable(const std::string& setting)  const;
			//get sub setting
			Setting							getSetting(const std::string& setting)  const;
			//set primitive
			void							setString(const std::string& setting, const std::string& value);
			void							setFloat(const std::string& setting, const float value);
			void							setInt(const std::string& setting, const int value);
			void							setBool(const std::string& setting, const int value);
			void							setSetting(const std::string& name, const Setting& setting);

			//dump
			std::string						toString() const;

		private:
			nlohmann::json					m_Setting;
			std::string						m_Directory;
	};
}

#endif // SETTING_H
