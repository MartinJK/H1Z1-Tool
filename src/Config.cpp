/*
	* Copyright (C) GTA:Multiplayer Team (https://wiki.gta-mp.net/index.php/Team)
	*
	* Redistribution and use in source and binary forms, with or without
	* modification, are permitted provided that the following conditions are
	* met:
	*
	*     * Redistributions of source code must retain the above copyright
	* notice, this list of conditions and the following disclaimer.
	*     * Redistributions in binary form must reproduce the above
	* copyright notice, this list of conditions and the following disclaimer
	* in the documentation and/or other materials provided with the
	* distribution.
	*     * Neither the name of GTA-Network nor the names of its
	* contributors may be used to endorse or promote products derived from
	* this software without specific prior written permission.
	*
	* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES INCLUDING, BUT NOT
	* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	* DATA, OR PROFITS; OR BUSINESS INTERRUPTION HOWEVER CAUSED AND ON ANY
	* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	* INCLUDING NEGLIGENCE OR OTHERWISE ARISING IN ANY WAY OUT OF THE USE
	* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	*
	* MODIFIED: Original subroutines are written by their owner
*/
#include <Windows.h>
#include <fstream>
#include <include/main.h>

void IConfig::SaveConfig()
{
	std::string strconf = this->Serialize();

	FILE * file = fopen(std::string(GetWorkingDirectory() + this->_conffile).c_str(), "w");
	if (file)
	{
		fwrite(strconf.c_str(), strconf.length(), sizeof(char), file);

		fclose(file);
	}
}

json::Object Config::Parse(const std::string& file)
{
	std::ifstream t(std::string(GetWorkingDirectory() + file).c_str());

	if (t.fail())
		return json::Object();

	std::string str;

	t.seekg(0, std::ios::end);
	str.reserve(static_cast<size_t>(t.tellg()));
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	auto value = json::Deserialize(str);

	if (value.GetType() == json::ValueType::NULLVal)
		return json::Object();
	else if (value.GetType() != json::ValueType::ObjectVal)
	{
		FILE * _file = fopen(std::string(GetWorkingDirectory() + file + ".default").c_str(), "w");
		if (_file)
		{
			fwrite("https://github.com/MartinJK/H1Z1Tool/data/system.json.default", strlen("https://github.com/MartinJK/H1Z1Tool/data/system.json.default"), sizeof(char), _file);

			fclose(_file);
		}

		delete _file;
	}
	else
	{
		auto object = value.ToObject();
		return object;
	}
	return json::Object();
}

json::Array ConfigArray::Parse(const std::string& file)
{
	std::ifstream t(std::string(GetWorkingDirectory() + file).c_str());

	if (t.fail())
		return json::Array();

	std::string str;

	t.seekg(0, std::ios::end);
	str.reserve(static_cast<size_t>(t.tellg()));
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	json::Value value = json::Deserialize(str);

	if (value.GetType() == json::ValueType::NULLVal)
		return json::Array();
	else if (value.GetType() != json::ValueType::ArrayVal)
	{
		FILE * _file = fopen(std::string(GetWorkingDirectory() + file + ".default").c_str(), "w");
		if (_file)
		{
			fwrite("https://github.com/MartinJK/H1Z1Tool/data/entity_color.json.default", strlen("https://github.com/MartinJK/H1Z1Tool/data/entity_color.json.default"), sizeof(char), _file);

			fclose(_file);
		}

		delete _file;
	}
	else
	{
		auto array = value.ToArray();
		return array;
	}
	return json::Array();
}

json::Object LanguageConfig::Parse(const std::string& file)
{
	std::string f(std::string(GetWorkingDirectory() + "\\data\\localization\\" + file + ".language.json").c_str());
	std::ifstream t(f);

	if (t.fail())
		return json::Object();

	std::string str;

	t.seekg(0, std::ios::end);
	str.reserve(static_cast<size_t>(t.tellg()));
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	auto value = json::Deserialize(str);

	if (value.GetType() == json::ValueType::NULLVal)
		return json::Object();
	else if (value.GetType() != json::ValueType::ObjectVal)
		return json::Object();
	else
	{
		auto object = value.ToObject();
		return object;
	}
	return json::Object();
}
