#include "skinchanger.h"

// used: create entity
#include "../utilities.h"

void CSkinChanger::Run()
{
	CBaseEntity* pLocal = CBaseEntity::GetLocalPlayer();

	if (pLocal == nullptr || !pLocal->IsAlive())
		return;

	// it will help you here my sweetest candy <3
	// https://www.unknowncheats.me/wiki/Counter_Strike_Global_Offensive:Skin_Changer
}

void CSkinChanger::Event(IGameEvent* pEvent, const FNV1A_t uNameHash)
{
	if (!I::Engine->IsInGame())
		return;
}

ItemSystem* GetItemSystem()
{
	static auto fn = reinterpret_cast<ItemSystem * (__stdcall*)()>(MEM::FindPattern(CLIENT_DLL, "A1 ? ? ? ? 85 C0 75 53 A1 ? ? ? ? 56 68 ? ? ? ? 8B 08"));
	return fn();
}

void CSkinChanger::Init()
{
	const auto pItemSchema = GetItemSystem()->GetItemSchema();
	ToUtf8Converter pConverter{ *I::Localize };
	Dump(pItemSchema, pConverter);
	L::Print("[SKINS] All skins dumped");
}

std::vector<KitWeapon> CSkinChanger::GetKitsWeapons(const UtlMap<std::uint64_t, AlternateIconData>& alternateIcons) noexcept
{
	std::vector<KitWeapon> kitsWeapons;
	kitsWeapons.reserve(alternateIcons.iNumElements);

	for (const auto& node : alternateIcons) {
		// https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/shared/econ/econ_item_schema.cpp#L325-L329
		if (const auto encoded = node.kKey; (encoded & 3) == 0)
			kitsWeapons.emplace_back(int((encoded & 0xFFFF) >> 2), EItemDefinitionIndex(encoded >> 16), node.vValue.simpleName.Data());
	}
	std::ranges::sort(kitsWeapons, {}, &KitWeapon::iPaintKit);
	return kitsWeapons;
}

void CSkinChanger::Dump(ItemSchema* pItemSchema, ToUtf8Converter<>& pConverter)
{
	const auto kitsWeapons = GetKitsWeapons(pItemSchema->alternateIcons);

	for (const auto& node : pItemSchema->paintKits) {
		const auto paintKit = node.vValue;

		if (paintKit->id == 0 || paintKit->id == 9001) // ignore workshop_default
			continue;

		const auto paintKitName = pConverter.ConvertUnicodeToAnsi(I::Localize->FindSafe(paintKit->itemName.Data()));
		pPaintKits.emplace_back(CPaintKit(paintKit->id, paintKitName, paintKit->wearRemapMin, paintKit->wearRemapMax ));

		const auto isGlove = (paintKit->id >= 10000);
		for (auto it = std::ranges::lower_bound(kitsWeapons, paintKit->id, {}, &KitWeapon::iPaintKit); it != kitsWeapons.end() && it->iPaintKit == paintKit->id; ++it) {
			const auto itemDef = pItemSchema->GetItemDefinitionInterface(it->weaponId);
			if (!itemDef)
				continue;

			if (isGlove) {
				pGlovePaintKits.emplace_back(PaintKitItem(it->weaponId, pPaintKits.size() -1, it->iconPath));
			}
			else {
				pWeaponPaintKits.emplace_back(PaintKitItem(it->weaponId, pPaintKits.size() - 1, it->iconPath));
			}
		}
	}
}

std::vector<KitItem> CSkinChanger::GetSkinsForAWeapon(EItemDefinitionIndex weaponId)
{
	std::vector<KitItem> items;

	for (const auto& item : pWeaponPaintKits)
	{
		if (item.weaponID == weaponId) 
		{
			auto skin = pPaintKits[item.dataIndex];
			items.emplace_back(KitItem(item.iconPath, item.weaponID, skin.id, skin.name));
		}
	}
	return items;
}
