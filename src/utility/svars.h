#pragma once

#include <variant>
#include <map>

namespace Utility
{
	enum class SVar
	{
		iWindowWidth,
		iWindowHeight,
		iFullScreen,
		iWindowPosX,
		iWindowPosY,
		iAnisotropic_Filtering,
		iNumBackBuffers,
		iVSyncEnabled,
		iDebugShaders,
		iMaxSRVs,
		iMaxDSVs,
		iMaxRTVs,
		iMaxSamplers,
		iMaxImGuiSRVs,
		iMaxVerticesInBuffer,
		iMaxIndicesInBuffer,
		COUNT,
	};

	class SVars
	{
	public:
		SVars();
		int GetSVarInt(SVar svar);
		float GetSVarFloat(SVar svar);
		void SetSVarInt(SVar svar, int val);
		void SetSVarFloat(SVar svar, float val);
		void Store();

		std::map<SVar, std::variant<int, float>> m_SVars = {
			{SVar::iFullScreen, 0},
			{SVar::iWindowPosX, 0},
			{SVar::iWindowPosY, 0},
			{SVar::iWindowWidth, 800},
			{SVar::iWindowHeight, 600},
			{SVar::iAnisotropic_Filtering, 1},
			{SVar::iNumBackBuffers, 3},
			{SVar::iVSyncEnabled, 1},
			{SVar::iDebugShaders, 0},
			{SVar::iMaxSRVs, 32782},
			{SVar::iMaxDSVs, 1024},
			{SVar::iMaxRTVs, 1024},
			{SVar::iMaxSamplers, 128},
			{SVar::iMaxImGuiSRVs, 32},
			{SVar::iMaxVerticesInBuffer, 50'000'000},
			{SVar::iMaxIndicesInBuffer, 100'000'000},
		};
	private:
		const char* const GetStringSVar(SVar svar) const;
	};
}