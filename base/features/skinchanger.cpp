#include "skinchanger.h"

// used: create entity
#include "../utilities.h"

// RUNNING
void CSkinChanger::Run()
{
	if (!I::Engine->IsConnected() && !I::Engine->IsInGame())
		return;

	CBaseEntity* pLocal = CBaseEntity::GetLocalPlayer();

	if (pLocal == nullptr || !pLocal->IsAlive())
		return;

	if (W::Widgets::cActiveSkinchanger->GetState())
	{
		PlayerInfo_t playerInfo;
		I::Engine->GetPlayerInfo(pLocal->GetIndex(), &playerInfo);

		auto uWeapons = pLocal->GetWeaponsHandle();
		for (size_t i = 0; uWeapons[i] != INVALID_EHANDLE_INDEX; i++) {
			CBaseCombatWeapon* pWeapon = reinterpret_cast<CBaseCombatWeapon*>(I::ClientEntityList->GetClientEntityFromHandle(uWeapons[i]));
			if (!pWeapon || pWeapon->GetOwnerXuidLow() != playerInfo.nXuidLow)
				continue;

			auto eItemDef = pWeapon->GetItemDefinitionIndexEnum();
			if (W::Widgets::cKnifeModel->GetValue() != 0 && pWeapon->GetClientClass()->nClassID == EClassIndex::CKnife)
			{
				int knifeModelIndex = W::Widgets::cKnifeModel->GetValue();
				if (weaponSkins.contains(static_cast<EItemDefinitionIndex>(knifeModelIndex)))
				{
					const char* szModel = mapItemListKnife.at(knifeModelIndex).szModel;
					ApplyKnifeModel(pWeapon, szModel, knifeModelIndex, weaponSkins[static_cast<EItemDefinitionIndex>(knifeModelIndex)], I::ModelInfo->GetModelIndex(szModel), 0.00, pLocal);
				}
				else
				{
					const char* szModel = mapItemListKnife.at(knifeModelIndex).szModel;
					ApplyKnifeModel(pWeapon, szModel, knifeModelIndex, 0, I::ModelInfo->GetModelIndex(szModel), 0.00, pLocal);
					ApplyKnifeModel(pWeapon, szModel, pWeapon->GetItemDefinitionIndex(), 0, I::ModelInfo->GetModelIndex(szModel), 0.00, pLocal);
				}
			}

			if (weaponSkins.contains(eItemDef) && pWeapon->GetClientClass()->nClassID != EClassIndex::CKnife)
			{
				ApplySkin(pWeapon, pWeapon->GetItemDefinitionIndex(), weaponSkins[eItemDef], 0.00);
			}

			pWeapon->GetAccountID() = playerInfo.nXuidLow;
		}

	}
}

void CSkinChanger::ApplyKnifeModel(CBaseCombatWeapon* pWeapon, const char* cModel, int iItemDefinitionIndex, int iPaintKit, int iModelIndex, float flWear, CBaseEntity* pLocal)
{
	pWeapon->GetItemDefinitionIndex() = iItemDefinitionIndex;
	pWeapon->GetFallbackPaintKit() = iPaintKit;
	pWeapon->GetEntityQuality() = 3;
	pWeapon->GetFallbackWear() = flWear;
	//pWeapon->GetModelIndex() = iModelIndex;

	CBaseViewModel* pViewModel = reinterpret_cast<CBaseViewModel*>(I::ClientEntityList->GetClientEntityFromHandle(pLocal->GetViewModelHandle()));
	if (!pViewModel)
		return;

	CBaseCombatWeapon* pViewModelWeapon = reinterpret_cast<CBaseCombatWeapon*>(I::ClientEntityList->GetClientEntityFromHandle(pViewModel->GetWeaponHandle()));
	if (!pViewModelWeapon)
		return;

	if (pViewModelWeapon != pWeapon)
		return;

	CBaseCombatWeapon* pWorldModel = reinterpret_cast<CBaseCombatWeapon*>(I::ClientEntityList->GetClientEntityFromHandle(pWeapon->GetWorldModelHandle()));
	if (!pWorldModel)
		return;

	pViewModel->SetWeaponModel(cModel, pWeapon);
	pWeapon->SetModelIndex(iModelIndex);
	pWorldModel->SetModelIndex(iModelIndex + 1);

	pWeapon->GetItemIDHigh() = -1;
	pWeapon->GetItemIDLow() = -1;
}

void CSkinChanger::ApplySkin(CBaseCombatWeapon* pWeapon, int iItemDefinitionIndex, int iPaintKit, float flWear)
{
	pWeapon->GetItemDefinitionIndex() = iItemDefinitionIndex;
	pWeapon->GetFallbackPaintKit() = iPaintKit;
	pWeapon->GetFallbackWear() = flWear;
}

// EVENT
void CSkinChanger::Event(IGameEvent* pEvent, const FNV1A_t uNameHash)
{
	if (!I::Engine->IsInGame())
		return;
}

// DUMPING SKINS
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
	L::Print(XorStr("[SKINS] All skins dumped"));
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

		const auto isGlove = (paintKit->id >= 10000);
		for (auto it = std::ranges::lower_bound(kitsWeapons, paintKit->id, {}, &KitWeapon::iPaintKit); it != kitsWeapons.end() && it->iPaintKit == paintKit->id; ++it) {
			const auto itemDef = pItemSchema->GetItemDefinitionInterface(it->weaponId);
			if (!itemDef)
				continue;

			if (isGlove) {
				pGlovePaintKits.emplace_back(CPaintKitItem(paintKit->id, paintKitName, it->weaponId, it->iconPath));
			}
			else {
				pWeaponPaintKits.emplace_back(CPaintKitItem(paintKit->id, paintKitName, it->weaponId, it->iconPath));
			}
		}
	}
}

std::vector<CPaintKitItem> CSkinChanger::GetSkinsForAWeapon(EItemDefinitionIndex weaponId)
{
	std::vector<CPaintKitItem> items;

	for (const auto& item : pWeaponPaintKits)
	{
		if (item.weaponID == weaponId)
		{
			items.emplace_back(item);
		}
	}
	return items;
}
