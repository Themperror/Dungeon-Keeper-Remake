#include "svars.h"
#include "print.h"
#include <assert.h>
#include <fstream>
#include <unordered_set>
namespace Utility
{
	const char* const s_SVarStr[static_cast<size_t>(SVar::COUNT)] =
	{
		"iWindowSizeX",
		"iWindowSizeY",
		"iFullscreen",
		"iWindowPosX",
		"iWindowPosY",
		"iAnisotropic_Filtering",
		"iNumBackBuffers",
		"iVsyncEnabled",
		"iDebugShaders",
		"iMaxSRVs",
		"iMaxDSVs",
		"iMaxRTVs",
		"iMaxSamplers",
		"iMaxImGuiSRVs",
		"iMaxVerticesInBuffer",
		"iMaxIndicesInBuffer"
	};

	const char* const SVars::GetStringSVar(SVar svar) const
	{
		return s_SVarStr[static_cast<size_t>(svar)];
	}


	SVars::SVars()
	{
		assert(m_SVars.size() == static_cast<size_t>(SVar::COUNT));

		std::ifstream configFile("config.ini");
		std::string line;
		if (configFile.is_open())
		{
			std::unordered_set<SVar> touchedSVars;

			while (std::getline(configFile, line))
			{
				size_t cIndex = line.find(' ', 0);
				if (cIndex != std::string::npos)
				{
					std::string svarName = line.substr(0, cIndex);
					for (int i = 0; i < static_cast<size_t>(SVar::COUNT); i++)
					{
						if (s_SVarStr[i] == svarName)
						{
							touchedSVars.emplace(static_cast<SVar>(i));

							if (svarName[0] == 'i')
							{
								SetSVarInt((SVar)i, std::stoi(line.substr(cIndex + 1, line.size() - (cIndex + 1))));
							}
							else
							{
								SetSVarFloat((SVar)i, std::stof(line.substr(cIndex + 1, line.size() - (cIndex + 1))));
							}
							break;
						}
					}
				}
			}
			configFile.close();

			if (touchedSVars.size() != static_cast<size_t>(SVar::COUNT))
			{
				std::ofstream nConfig("config.ini", std::ofstream::out | std::ofstream::app);
				if (nConfig.is_open())
				{
					for (auto& it : m_SVars)
					{
						if(touchedSVars.find(it.first) == touchedSVars.end())
						{
							nConfig << GetStringSVar(it.first) << " ";
							if (std::holds_alternative<int>(it.second))
							{
								nConfig << std::get<int>(it.second);
							}
							else
							{
								nConfig << std::get<float>(it.second);
							}
							nConfig << '\n';
						}
					}
					nConfig.close();
				}
			}
		}
		else
		{
			Store();
		}
	}

	void SVars::Store()
	{
		std::ofstream nConfig("config.ini");
		if (nConfig.is_open())
		{
			for (auto& it : m_SVars)
			{
				nConfig << GetStringSVar(it.first) << " ";
				if (std::holds_alternative<int>(it.second))
				{
					nConfig << std::get<int>(it.second);
				}
				else
				{
					nConfig << std::get<float>(it.second);
				}
				nConfig << '\n';
			}
			nConfig.close();
		}
	}

	int SVars::GetSVarInt(SVar svar)
	{
		auto& var = m_SVars[svar];
		if (std::holds_alternative<int>(var))
		{
			return std::get<int>(var);
		}
		Print("Attempting to get svar: %s as integer!", GetStringSVar(svar));
		return 0;
	}

	float SVars::GetSVarFloat(SVar svar)
	{
		auto& var = m_SVars[svar];
		if (std::holds_alternative<float>(var))
		{
			return std::get<float>(var);
		}
		Print("Attempting to get svar: %s as float!", GetStringSVar(svar));
		return 0.0f;
	}

	void SVars::SetSVarInt(SVar svar, int val)
	{
		auto it = m_SVars.find(svar);
		if (it != m_SVars.end())
		{
			if (std::holds_alternative<int>(it->second))
			{
				it->second = val;
			}
			else
			{
				Print("Attempting to set svar: %s as int!", GetStringSVar(svar));
			}
		}
		else
		{
			m_SVars[svar] = val;
		}
	}
	void SVars::SetSVarFloat(SVar svar, float val)
	{
		auto it = m_SVars.find(svar);
		if (it != m_SVars.end())
		{
			if (std::holds_alternative<float>(it->second))
			{
				it->second = val;
			}
			else
			{
				Print("Attempting to set svar: %s as float!", GetStringSVar(svar));
			}
		}
		else
		{
			m_SVars[svar] = val;
		}
	}
}
