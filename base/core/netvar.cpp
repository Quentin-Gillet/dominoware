#include "netvar.h"

#include <filesystem>

// used: client interface
#include "interfaces.h"

bool CNetvarManager::Setup(const std::string_view szDumpFileName)
{
	// clear values
	mapProps.clear();
	iStoredProps = 0;
	iStoredTables = 0;

	// format time
	const std::string szTime = std::vformat(XorStr("[{:%d-%m-%Y %X}] "), std::make_format_args(std::chrono::system_clock::now()));

#ifdef _DEBUG
	//std::filesystem::path fsWorkingPath;
	//fsWorkingPath.append("dominoware").append(szDumpFileName);
	//// open our dump file to write in (here is not exception handle because dump is not critical)
	//fsDumpFile.open(fsWorkingPath, std::ios::out | std::ios::trunc);

	//if (fsDumpFile.good())
	//	// write current date, time and info
	//	fsDumpFile << szTime << XorStr("dominoware | netvars dump\n\n");
#endif

	for (auto pClass = I::Client->GetAllClasses(); pClass != nullptr; pClass = pClass->pNext)
	{
		if (pClass->pRecvTable == nullptr)
			continue;

		StoreProps(pClass->szNetworkName, pClass->pRecvTable, 0U, 0);
	}

#ifdef _DEBUG
	// close dump file
	//fsDumpFile.close();
#endif

	return !mapProps.empty();
}

void CNetvarManager::StoreProps(const char* szClassName, RecvTable_t* pRecvTable, const std::uintptr_t uOffset, const int iDepth)
{
#ifdef _DEBUG
	std::string szDepth = { };

	for (int i = 0; i < iDepth; i++)
		szDepth.append("\t");

	if (fsDumpFile.good())
		fsDumpFile << std::format("{0}[{1}]\n", szDepth, pRecvTable->szNetTableName);
#endif

	const FNV1A_t uClassHash = FNV1A::Hash(szClassName);
	const FNV1A_t uDelimiterHash = FNV1A::Hash("->", uClassHash);
	const FNV1A_t uBaseClassHash = FNV1A::Hash("baseclass", uDelimiterHash);

	for (int i = 0; i < pRecvTable->nProps; ++i)
	{
		const auto pCurrentProp = &pRecvTable->pProps[i];

		// base tables filter
		if (pCurrentProp == nullptr || isdigit(pCurrentProp->szVarName[0]))
			continue;

		// concat to our netvar format just by hash
		const FNV1A_t uTotalHash = FNV1A::Hash(pCurrentProp->szVarName, uDelimiterHash);

		// skip baseclass
		if (uTotalHash == uBaseClassHash)
			continue;

		// has child table
		if (const auto pChildTable = pCurrentProp->pDataTable; pChildTable != nullptr &&
			// has props
			pChildTable->nProps > 0 &&
			// first char is 'D' ("DT" - "DataTable")
			pChildTable->szNetTableName[0] == 'D' &&
			// type is data table
			pCurrentProp->iRecvType == DPT_DATATABLE)
			// recursively get props in all child tables
			StoreProps(szClassName, pChildTable, static_cast<std::uintptr_t>(pCurrentProp->iOffset) + uOffset, iDepth + 1);

		const std::uintptr_t uTotalOffset = static_cast<std::uintptr_t>(pCurrentProp->iOffset) + uOffset;

		// check if we have already grabbed property
		if (!mapProps[uTotalHash].uOffset)
		{
#ifdef _DEBUG
			if (fsDumpFile.good())
				fsDumpFile << std::format("{0}\t{1} {2} = 0x{3:04X};\n", szDepth, GetPropertyType(pCurrentProp), pCurrentProp->szVarName, uTotalOffset);
#endif

			// write values to map entry
			mapProps[uTotalHash] = { pCurrentProp, uTotalOffset };

			// count total stored props
			iStoredProps++;
		}
	}

	// count total stored tables
	iStoredTables++;
}

std::string CNetvarManager::GetPropertyType(const RecvProp_t* pRecvProp) const
{
	static CStandartRecvProxies* pStandartRecvProxies = I::Client->GetStandardRecvProxies();

	if (pRecvProp == nullptr)
		return "";

	RecvVarProxyFn pProxyFn = pRecvProp->oProxyFn;

	switch (pRecvProp->iRecvType)
	{
	case DPT_INT:
		// @credits: hinnie
		if (pProxyFn == pStandartRecvProxies->pInt32ToInt8)
			return XorStr("byte");
		else if (pProxyFn == pStandartRecvProxies->pInt32ToInt16)
			return XorStr("short");

		return XorStr("int");
	case DPT_FLOAT:
		return XorStr("float");
	case DPT_VECTOR:
		return XorStr("Vector");
	case DPT_VECTOR2D:
		return XorStr("Vector2D");
	case DPT_STRING:
		return std::vformat(XorStr("char[{:d}]"), std::make_format_args(pRecvProp->nStringBufferSize));
	case DPT_ARRAY:
		return std::vformat(XorStr("array[{:d}]"), std::make_format_args(pRecvProp->iElements));
	case DPT_DATATABLE:
		return XorStr("void*");
	case DPT_INT64:
		return XorStr("std::int64_t");
	default:
		break;
	}

	return "";
}

std::uintptr_t CNetvarManager::FindInDataMap(DataMap_t* pMap, const FNV1A_t uFieldHash)
{
	while (pMap != nullptr)
	{
		for (int i = 0; i < pMap->nDataFields; i++)
		{
			if (pMap->pDataDesc[i].szFieldName == nullptr)
				continue;

			if (FNV1A::Hash(pMap->pDataDesc[i].szFieldName) == uFieldHash)
				return pMap->pDataDesc[i].iFieldOffset[TD_OFFSET_NORMAL];

			if (pMap->pDataDesc[i].iFieldType == FIELD_EMBEDDED)
			{
				if (pMap->pDataDesc[i].pTypeDescription != nullptr)
				{
					if (const auto uOffset = FindInDataMap(pMap->pDataDesc[i].pTypeDescription, uFieldHash); uOffset != 0U)
						return uOffset;
				}
			}
		}

		pMap = pMap->pBaseMap;
	}

	return 0U;
}
