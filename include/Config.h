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
*/
#pragma once
#include <vendor/supereasyjson/json.h>

class IConfig
{
public:
	json::Object _ref;
	std::string _conffile;

	IConfig() { }
	IConfig(const std::string& file)
	{
		this->_conffile = file;
	}

	virtual json::Object& Object() 
	{
		return this->_ref;
	}

	virtual std::string GetString(const std::string& str)
	{
		if (this->_ref.HasKey(str))
			return this->_ref[str].ToString();
		else
			return "";
	}

	virtual bool GetBoolean(const std::string& str)
	{
		if (this->_ref.HasKey(str))
			return this->_ref[str].ToBool();
		else
			return false;
	}

	virtual int GetInteger(const std::string& str)
	{
		if (this->_ref.HasKey(str))
			return this->_ref[str].ToInt();
		else
			return -1;
	}

	virtual void SetString(std::string str, std::string val)
	{
		this->_ref[str] = val;
	}

	virtual void SetBoolean(const std::string& str, bool bl)
	{
		this->_ref[str] = bl;
	}

	virtual void SetInteger(const std::string& str, int i)
	{
		this->_ref[str] = i;
	}

	virtual std::string Serialize()
	{
		return json::Serialize(this->_ref);
	}

	virtual void SaveConfig();
};

class IConfigArray
{
public:
	json::Array _ref;
	std::string _conffile;

	IConfigArray() { }
	IConfigArray(const std::string& file)
	{
		this->_conffile = file;
	}

	virtual json::Array& Object()
	{
		return this->_ref;
	}

	virtual std::string Serialize()
	{
		return json::Serialize(this->_ref);
	}
};

class Config : public IConfig
{
public:
	Config() : IConfig() { };
	Config(const std::string& file) : IConfig(file) { this->_ref = this->Parse(std::string(file)); };

	json::Object Parse(std::string& file);
};

class ConfigArray : public IConfigArray
{
public:
	ConfigArray() : IConfigArray() { };
	ConfigArray(const std::string& file) : IConfigArray(file) { this->_ref = this->Parse(std::string(file)); };

	json::Array Parse(std::string& file);
};

class LanguageConfig : public IConfig
{
public:
	std::string _lang = "en";

	LanguageConfig() : IConfig() { };
	LanguageConfig(const std::string& file) : IConfig(file) { this->_ref = this->Parse(std::string(file) /*const_cast<std::String&>(file)*/); };

	json::Object Parse(std::string& file);
	virtual void SaveConfig() override
	{
		; // override so we cannot save a language file(would be corrupted)
	}
};