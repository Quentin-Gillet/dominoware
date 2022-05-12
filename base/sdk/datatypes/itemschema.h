#pragma once

// uses: std::byte
#include <vector>
// uses: ultmap
#include "utlstring.h"
#include "utlstack.h"
#include "utlmap.h"

struct EconItemQualityDefinition {
    int value;
    const char* name;
    unsigned weight;
    bool explicitMatchesOnly;
    bool canSupportSet;
    const char* hexColor;
};

struct PaintKit {
    int id;
    CUtlString name;
    CUtlString description;
    CUtlString itemName;
    CUtlString sameNameFamilyAggregate;
    CUtlString pattern;
    CUtlString normal;
    CUtlString logoMaterial;
    bool baseDiffuseOverride;
    int rarity;
    std::byte pad0[40];
    float wearRemapMin;
    float wearRemapMax;
};

struct StickerKit {
    int id;
    int rarity;
    CUtlString name;
    CUtlString description;
    CUtlString itemName;
    std::byte pad0[2 * sizeof(CUtlString)];
    CUtlString inventoryImage;
    int tournamentID;
    int tournamentTeamID;
    int tournamentPlayerID;
};

struct EconMusicDefinition {
    int id;
    const char* name;
    const char* nameLocalized;
    std::byte pad0[2 * sizeof(const char*)];
    const char* inventoryImage;
};

struct AlternateIconData {
    CUtlString simpleName;
    CUtlString largeSimpleName;
    CUtlString iconURLSmall;
    CUtlString iconURLLarge;
    std::byte pad0[28];
};

class EconItemDefinition;

class ItemSchema
{
public:
	std::byte pad0[0x88];
    UtlMap<int, EconItemQualityDefinition> qualities;
    std::byte pad1[0x48];
    UtlMap<int, EconItemDefinition*> itemsSorted;
    std::byte pad2[0x60];
    UtlMap<int, const char*> revolvingLootLists;
    std::byte pad3[0x80];
    UtlMap<std::uint64_t, AlternateIconData> alternateIcons;
    std::byte pad4[0x48];
    UtlMap<int, PaintKit*> paintKits;
    UtlMap<int, StickerKit*> stickerKits;
    std::byte pad5[0x11A];
    UtlMap<int, EconMusicDefinition*> musicKits;

    EconItemDefinition* GetItemDefinitionInterface(int id)
    {
        return MEM::CallVFunc<EconItemDefinition*>(this, 4, id);
    }
};

class ItemSystem
{
public:
	virtual ItemSchema* GetItemSchema() = 0;
};