//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : Unit tab of the control panel
// Id           : $Id$
//
//----------------------------------------------------------------------------
//
// Disclaimer
//
// THIS FILE IS NOT GENERATED OR SUPPORTED BY ACTIVISION.
//
// This material has been developed at apolyton.net by the Apolyton CtP2
// Source Code Project. Contact the authors at ctp2source@apolyton.net.
//
//----------------------------------------------------------------------------
//
// Compiler flags
//
// - None
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Option added to select which order buttons are displayed for an army.
// - Added unit display name.
// - Standardized code (May 21st 2006 Martin Gühmann)
// - Added a custom status bar text for the upgrade order. (13-Sep-2008 Martin Gühmann)
// - Changed occurrences of UnitRecord::GetMaxHP to
//   UnitData::CalculateTotalHP. (Aug 3rd 2009 Maq)
//
//----------------------------------------------------------------------------

#include "c3.h"
#include "UnitControlPanel.h"

#include "ArmyData.h"
#include "ArmyPool.h"
#include "aui_ldl.h"
#include "c3ui.h"
#include "Cell.h"
#include "controlpanelwindow.h"
#include "ctp2_button.h"
#include "ctp2_Static.h"
#include "IconRecord.h"
#include "OrderRecord.h"
#include "pixeltypes.h"
#include "player.h"
#include "profileDB.h"
#include "SelItem.h"
#include "UnitRecord.h"
#include "World.h"
#include "StrDB.h"
#include "aui_tipwindow.h"
#include "UnitData.h"
#include "Unit.h"
#include "ctp2_Switch.h"

extern C3UI *g_c3ui;

static MBCHAR * ARMY_SYMBOL  = "UPIC21.tga";
static MBCHAR * CARGO_SYMBOL = "UPC045.tga";

UnitControlPanel::UnitControlPanel(MBCHAR * ldlBlock) :
m_unitDisplayGroup(static_cast<ctp2_Static*>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay"))),
m_unitListPreviousButton(static_cast<ctp2_Button*>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.UnitSelect.Previous"))),
m_unitListLabel(static_cast<ctp2_Static*>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.UnitSelect.Label"))),
m_unitListNextButton(static_cast<ctp2_Button*>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.UnitSelect.Next"))),
m_singleSelectionDisplay(static_cast<ctp2_Static*>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.SingleSelect"))),
m_singleSelectionIcon(static_cast<ctp2_Static*>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.SingleSelect.Icon"))),
m_singleSelectionArmySymbol(static_cast<ctp2_Static*>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.SingleSelect.Icon.ArmySymbol"))),
m_singleSelectionCargoSymbol(static_cast<ctp2_Static*>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.SingleSelect.Icon.CargoSymbol"))),
m_singleSelectionAttack(static_cast<ctp2_Static*>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.SingleSelect.Attack.Value"))),
m_singleSelectionDefend(static_cast<ctp2_Static*>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.SingleSelect.Defend.Value"))),
m_singleSelectionMove(static_cast<ctp2_Static*>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.SingleSelect.Move.Value"))),
m_singleSelectionRange(static_cast<ctp2_Static*>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.SingleSelect.Range.Value"))),
m_singleSelectionArmor(static_cast<ctp2_Static*>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.SingleSelect.Armor.Value"))),
m_singleSelectionFirepower(static_cast<ctp2_Static *>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.SingleSelect.Firepower.Value"))),
m_singleSelectionHealth(static_cast<ctp2_Static*>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.SingleSelect.Health"))),
m_singleSelectionFuel(static_cast<ctp2_Static *>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.SingleSelect.Fuel"))),
m_multipleSelectionDisplay(static_cast<ctp2_Static*>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.MultipleSelect"))),
m_armySelectionDisplay(static_cast<ctp2_Static*>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.ArmySelect"))),
m_armySelectionIcon(static_cast<ctp2_Static*>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.ArmySelect.Background.Icon"))),
m_transportSelectionDisplay(static_cast<ctp2_Static *>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.TransportSelect"))),
m_transportSelectionIcon(static_cast<ctp2_Static *>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.TransportSelect.Background.Icon"))),
m_transportSelectionIconArmySymbol(static_cast<ctp2_Static *>(aui_Ldl::GetObject(ldlBlock,
		"UnitTab.TabPanel.UnitSelectionDisplay.TransportSelect.Background.Icon.ArmySymbol"))),
m_cellUnitList(),
m_cellArmyList()
{
	Assert(m_unitDisplayGroup);
	Assert(m_unitListPreviousButton);
	Assert(m_unitListLabel);
	Assert(m_unitListNextButton);
	Assert(m_singleSelectionDisplay);
	Assert(m_singleSelectionIcon);
	Assert(m_multipleSelectionDisplay);
	Assert(m_armySelectionDisplay);
	Assert(m_armySelectionIcon);
	Assert(m_transportSelectionDisplay);
	Assert(m_transportSelectionIcon);

	m_armySelectionUnit = -1;

	m_curSingleArmy.m_id = 0;
	m_curSingleUnit.m_id = 0;
	m_curMovement = -1;

	m_curHealth = -1;
	m_curCargo = -2;

	for (sint32 multiIndex = 0; multiIndex < NUMBER_OF_MULTIPLE_SELECTION_BUTTONS; multiIndex++)
	{
		MBCHAR multiButtonName[128];
		sprintf(multiButtonName,"UnitTab.TabPanel.UnitSelectionDisplay.MultipleSelect.Unit%d", multiIndex);

		m_multipleSelectionButton[multiIndex] =
				static_cast<ctp2_Button*>(aui_Ldl::GetObject(ldlBlock, multiButtonName));
		m_multipleSelectionHealth[multiIndex] =
				(ctp2_Static *)m_multipleSelectionButton[multiIndex]->GetChildByIndex(0);

		Assert(m_multipleSelectionButton[multiIndex]);
	}

	for (sint32 armyIndex = 0; armyIndex < NUMBER_OF_ARMY_SELECTION_BUTTONS; armyIndex++)
	{
		MBCHAR armyButtonName[128];
		sprintf(armyButtonName,"UnitTab.TabPanel.UnitSelectionDisplay.ArmySelect.Unit%d", armyIndex);

		m_armySelectionButton[armyIndex] = static_cast<ctp2_Button*>(aui_Ldl::GetObject(ldlBlock, armyButtonName));
		m_armySelectionHealth[armyIndex] = (ctp2_Static *)m_armySelectionButton[armyIndex]->GetChildByIndex(0);

		Assert(m_armySelectionButton[armyIndex]);

		m_armySelectionButton[armyIndex]->SetActionFuncAndCookie(ArmyButtonActionCallback, this);
	}

	for (sint32 transIndex = 0; transIndex < k_MAX_CP_CARGO; transIndex++)
	{
		MBCHAR transportButtonName[256];
		sprintf(transportButtonName, "UnitTab.TabPanel.UnitSelectionDisplay.TransportSelect.Cargo%d", transIndex);
		m_transportSelectionButton[transIndex] = (ctp2_Switch *)aui_Ldl::GetObject(ldlBlock, transportButtonName);
		m_transportSelectionHealth[transIndex] =
				(ctp2_Static *)m_transportSelectionButton[transIndex]->GetChildByIndex(0);
	}

	for (sint32 orderIndex = 0; orderIndex < NUMBER_OF_ORDER_BUTTONS; orderIndex++)
	{
		MBCHAR orderButtonName[128];
		sprintf(orderButtonName,"UnitTab.TabPanel.UnitOrderButtonGrid.Order%d", orderIndex);

		m_orderButton[orderIndex] = static_cast<ctp2_Button*>(aui_Ldl::GetObject(ldlBlock, orderButtonName));

		Assert(m_orderButton[orderIndex]);

		m_orderButton[orderIndex]->Enable(false);
	}

	m_singleSelectionArmySymbol->SetImageMapCallback(SingleSelectionArmySymbolImageCallback, this);
	m_transportSelectionIcon->SetImageMapCallback(TransportSelectionImageCallback, this);

	m_unitListPreviousButton->SetActionFuncAndCookie(PrevUnitButtonActionCallback, this);
	m_unitListNextButton->SetActionFuncAndCookie(NextUnitButtonActionCallback, this);

	SetSelectionMode(MULTIPLE_SELECTION);

	m_unitDisplayGroup->SetShowCallback(UnitDisplayGroupCallback, this);
}

void UnitControlPanel::Update()
{
	switch(m_currentMode) {
		case SINGLE_SELECTION:
			UpdateSingleSelectionDisplay();
			break;
		case MULTIPLE_SELECTION:
			UpdateMultipleSelectionDisplay();
			break;
		case ARMY_SELECTION:
			UpdateArmySelectionDisplay();
			break;
		case TRANSPORT_SELECTION:
			UpdateTransportSelectionDisplay();
			break;
		default:
			Assert(false);
			break;
	}

	if (!m_unitDisplayGroup->IsHidden()) {
		m_unitDisplayGroup->ShouldDraw();
	}

	UpdateOrderButtons();
}

void UnitControlPanel::SelectedUnit()
{
	Army army;
	if (g_selected_item->GetSelectedArmy(army) && (!army.IsValid() || (army.Num() < 2))) {
		SetSelectionMode(SINGLE_SELECTION);
	} else {
		SetSelectionMode(ARMY_SELECTION);
	}

	m_lastSelectedArmy.m_id = 0;
	m_lastSelectedArmyCount = -1;
}

void UnitControlPanel::SetSelectionMode(UnitSelectionMode mode)
{
	m_armySelectionUnit = -1;

	DoSetSelectionMode(mode);
}

void UnitControlPanel::DoSetSelectionMode(UnitSelectionMode mode)
{
	m_currentMode = mode;

	if (m_unitDisplayGroup->IsHidden()) {
		return;
	}

	switch(m_currentMode) {
		case SINGLE_SELECTION:
			m_singleSelectionDisplay->Show();
			updateSingleSelectionSymbols();
			m_multipleSelectionDisplay->Hide();
			m_armySelectionDisplay->Hide();
			m_transportSelectionDisplay->Hide();
			break;
		case MULTIPLE_SELECTION:
			m_singleSelectionDisplay->Hide();
			m_multipleSelectionDisplay->Show();
			m_armySelectionDisplay->Hide();
			m_transportSelectionDisplay->Hide();
			break;
		case ARMY_SELECTION:
			m_singleSelectionDisplay->Hide();
			m_multipleSelectionDisplay->Hide();
			m_armySelectionDisplay->Show();
			m_transportSelectionDisplay->Hide();
			break;
		case TRANSPORT_SELECTION:
			m_singleSelectionDisplay->Hide();
			m_multipleSelectionDisplay->Hide();
			m_armySelectionDisplay->Hide();
			UnsetCargoButtons();
			m_transportSelectionDisplay->Show();
			break;
		default:
			Assert(false);
			break;
	}

	if (g_selected_item) {
		Update();
	}
}

void UnitControlPanel::UpdateSingleSelectionDisplay()
{
	Army army = GetSelectedArmy();
	Unit unit;
	double movement = -1;
	double health = -1;
	sint32 cargo = -1;
	sint32 fuel = -1;
	sint32 maxFuel = -1;

	if (army.IsValid())
	{
		if (m_armySelectionUnit >= 0 && m_armySelectionUnit < army.Num()) {
			unit.m_id = army[m_armySelectionUnit].m_id;
		} else {
			unit.m_id = army[0].m_id;
		}
		if (unit.IsValid())
		{
			movement = unit.GetMovementPoints();
			health = unit.GetHP();
			if (!unit.GetUsedFuel(fuel, maxFuel))
			{
				fuel = -1;
				maxFuel = -1;
			}
			cargo = unit.GetDBRec()->HasCargoData() ? unit->GetNumCarried() : -1;
		}
	}

	m_unitListLabel->SetText(unit.IsValid() ? unit.GetDisplayName().c_str() : "");

	if ((m_curSingleArmy.m_id == army.m_id)
		&& (m_curSingleUnit.m_id == unit.m_id)
		&& (m_curMovement == movement)
		&& (m_curHealth == health)
		&& (m_curCargo == cargo)
		&& (m_curFuel == fuel))
	{
		return;
	}

	m_curSingleArmy.m_id = army.m_id;
	m_curSingleUnit.m_id = unit.m_id;
	m_curMovement = movement;
	m_curHealth = health;
	m_curCargo = cargo;
	m_curFuel = fuel;

	if (!unit.IsValid())
	{
		m_singleSelectionIcon->SetImage(NULL);
		m_singleSelectionArmySymbol->Hide();
		m_singleSelectionCargoSymbol->Hide();
		m_singleSelectionAttack->SetText("");
		m_singleSelectionDefend->SetText("");
		m_singleSelectionMove->SetText("");
		m_singleSelectionRange->SetText("");
		m_singleSelectionArmor->SetText("");
		m_singleSelectionFirepower->SetText("");

		m_singleSelectionHealth->SetDrawCallbackAndCookie(NULL, NULL);
		m_singleSelectionIcon->SetDrawCallbackAndCookie(NULL, NULL);
		m_singleSelectionIcon->SetImageMapCallback(NULL, NULL);
		m_singleSelectionFuel->SetDrawCallbackAndCookie(NULL, NULL);
		return;
	}

	const IconRecord * iconRecord   = unit.GetDBRec()->GetDefaultIcon();
	const MBCHAR     * unitIconName = iconRecord->GetLargeIcon();

	if (unitIconName && strcmp(unitIconName, "NULL"))
	{
		m_singleSelectionIcon->SetImage((char *) unitIconName);
		updateSingleSelectionSymbols();
	}

	MBCHAR valueString[16];
	sprintf(valueString, "%d", static_cast<sint32>(unit.GetAttack()));
	m_singleSelectionAttack->SetText(valueString);
	sprintf(valueString, "%d", static_cast<sint32>(unit.GetDefense()));
	m_singleSelectionDefend->SetText(valueString);
	sprintf(valueString, "%d", static_cast<sint32>(ceil(unit.GetMovementPoints() / 100.0)));
	m_singleSelectionMove->SetText(valueString);
	sprintf(valueString, "%d", static_cast<sint32>(unit.GetZBRange()));
	m_singleSelectionRange->SetText(valueString);
	sprintf(valueString, "%d", (sint32)(unit.GetDBRec()->GetArmor()));
	m_singleSelectionArmor->SetText(valueString);
	sprintf(valueString, "%d", (sint32)(unit.GetDBRec()->GetFirepower()));
	m_singleSelectionFirepower->SetText(valueString);

	m_singleSelectionFuel->SetDrawCallbackAndCookie(FuelBarDrawCallback, (void *)unit.m_id);
	m_singleSelectionHealth->SetDrawCallbackAndCookie(HealthBarActionCallback, reinterpret_cast<void*>(unit.m_id));

	if (m_curCargo >= 0)
	{
		m_singleSelectionIcon->SetDrawCallbackAndCookie(DrawCargoCallback, (void *)unit.m_id, false);
		m_singleSelectionIcon->SetImageMapCallback(TransportImageCallback, (void *)this);
	}
	else
	{
		m_singleSelectionIcon->SetDrawCallbackAndCookie(NULL, NULL, false);
		m_singleSelectionIcon->SetImageMapCallback(NULL, NULL);
	}
}

void UnitControlPanel::UpdateMultipleSelectionDisplay()
{
	if (!g_selected_item) {
		return;
	}

	CellUnitList newUnitList;
    g_theWorld->GetCell(g_selected_item->GetCurSelectPos())->GetArmy(newUnitList);

    std::vector<Army> newArmyList;
	for (sint32 i = 0; i < newUnitList.Num(); ++i)
    {
		newArmyList.push_back(newUnitList[i].GetArmy());
	}

	m_unitListLabel->SetText("");

	bool changed = false;
	if (static_cast<size_t>(newUnitList.Num()) == m_cellUnitList.size())
    {
		for (sint32 i = 0; i < newUnitList.Num(); ++i)
        {
			if (newUnitList[i].m_id != m_cellUnitList[i].m_id || newArmyList[i].m_id != m_cellArmyList[i].m_id)
            {
				changed = true;
				break;
			}
		}
	}
    else
    {
		changed = true;
	}

	if (!changed)
    {
		return;
	}

	sint32 multiIndex = 0;
	sint32 unitIndex  = 0;

	while (multiIndex < NUMBER_OF_MULTIPLE_SELECTION_BUTTONS)
    {
		if (unitIndex < newUnitList.Num())
        {
			Unit & unit = newUnitList[unitIndex++];
			Army   army = unit.GetArmy();

			bool armyAlreadyShown = false;
			for (sint32 testIndex = unitIndex - 2; testIndex >= 0; testIndex--)
			{
				if (newUnitList[testIndex].GetArmy().m_id == army.m_id) {
					armyAlreadyShown = true;
                }
			}

			if (!armyAlreadyShown)
            {
				m_multiPair[multiIndex].first   = this;
				m_multiPair[multiIndex].second  = army.m_id;

				m_multipleSelectionButton[multiIndex]->SetActionFuncAndCookie(MultiButtonActionCallback,
						&m_multiPair[multiIndex]);
				if (army.IsValid() && army.Num() == 1)
				{
					m_multipleSelectionHealth[multiIndex]->SetDrawCallbackAndCookie(HealthBarActionCallback,
							(void *)army[0].m_id);
				}
				else {
					m_multipleSelectionHealth[multiIndex]->SetDrawCallbackAndCookie(NULL, NULL);
				}
				m_multipleSelectionButton[multiIndex]->Enable(true);

				m_multipleSelectionButton[multiIndex]->ExchangeImage(0, 0,
						unit.GetDBRec()->GetDefaultIcon()->GetIcon());
				m_multipleSelectionButton[multiIndex]->ExchangeImage(1, 0,
						(army.IsValid() && army.Num() > 1) ?  ARMY_SYMBOL : NULL);
				multiIndex++;
			}
		}
        else
        {
			m_multipleSelectionButton[multiIndex]->SetActionFuncAndCookie(NULL, NULL);
			m_multipleSelectionHealth[multiIndex]->SetDrawCallbackAndCookie(NULL, NULL);
			m_multipleSelectionButton[multiIndex]->Enable(false);
			m_multipleSelectionButton[multiIndex]->ExchangeImage(0, 0, NULL);
			m_multipleSelectionButton[multiIndex]->ExchangeImage(1, 0, NULL);
			multiIndex++;
		}
	}

    m_cellArmyList.swap(newArmyList);
    m_cellUnitList.clear();
	for (sint32 newUnitIndex = 0; newUnitIndex < newUnitList.Num(); ++newUnitIndex)
    {
        m_cellUnitList.push_back(newUnitList[newUnitIndex]);
    }
}

void UnitControlPanel::UpdateArmySelectionDisplay()
{
	m_armySelectionUnit = -1;

	Army army = GetSelectedArmy();
	if (!army.IsValid() || (army.Num() <= 1))
    {
		SetSelectionMode(SINGLE_SELECTION);
		return;
	}

	Unit unit = army[0];
	if (unit.IsValid())
	{
		const MBCHAR * armyIconName = unit.GetDBRec()->GetDefaultIcon()->GetLargeIcon();
		if (!armyIconName || strcmp(armyIconName, "NULL") == 0) {
			armyIconName = unit.GetDBRec()->GetDefaultIcon()->GetIcon();
		}
		if (armyIconName && strcmp(armyIconName, "NULL") != 0) {
			m_armySelectionIcon->ExchangeImage(0, 0, armyIconName);
		}
	}

	for (sint32 armyIndex = 0; armyIndex < NUMBER_OF_ARMY_SELECTION_BUTTONS; armyIndex++)
	{
		if (armyIndex < army.Num())
		{
			const MBCHAR *unitIconName = army[armyIndex].GetDBRec()->GetDefaultIcon()->GetSmallIcon();
			if (unitIconName && strcmp(unitIconName, "NULL"))
			{
				m_armySelectionButton[armyIndex]->ExchangeImage(0, 0, unitIconName);
				m_armySelectionHealth[armyIndex]->SetDrawCallbackAndCookie(HealthBarActionCallback,
						(void *)army[armyIndex].m_id);
			}
			else
			{
				m_armySelectionButton[armyIndex]->ExchangeImage(0, 0, NULL);
				m_armySelectionHealth[armyIndex]->SetDrawCallbackAndCookie(NULL, NULL);
			}
			m_armySelectionButton[armyIndex]->Enable(true);
		}
		else
		{
			m_armySelectionButton[armyIndex]->ExchangeImage(0, 0, NULL);
			m_armySelectionHealth[armyIndex]->SetDrawCallbackAndCookie(NULL, NULL);
			m_armySelectionButton[armyIndex]->Enable(false);
		}
	}

	m_unitListLabel->SetText(army.GetData()->GetName());
}

void UnitControlPanel::UpdateTransportSelectionDisplay()
{
	if (m_transportSelectionDisplay->IsHidden()) {
		return;
	}

	Army army = GetSelectedArmy();
	Unit unit;

	if (army.IsValid())
	{
		if (m_armySelectionUnit >= 0 && m_armySelectionUnit < army.Num()) {
			unit.m_id = army[m_armySelectionUnit].m_id;
		} else {
			unit.m_id = army[0].m_id;
		}
	}

	if (unit.IsValid())
	{
		UnitDynamicArray *cargoList = unit->GetCargoList();

		const MBCHAR * transportIconName = unit.GetDBRec()->GetDefaultIcon()->GetLargeIcon();
		if (!transportIconName || strcmp(transportIconName, "NULL") == 0) {
			transportIconName = unit.GetDBRec()->GetDefaultIcon()->GetIcon();
		}
		if (transportIconName && strcmp(transportIconName, "NULL") != 0) {
			m_transportSelectionIcon->ExchangeImage(0, 0, transportIconName);
			if (m_armySelectionUnit >= 0) {
				m_transportSelectionIconArmySymbol->Show();
			} else {
				m_transportSelectionIconArmySymbol->Hide();
			}
		}

		Assert(cargoList);
		if (!cargoList) {
			SetSelectionMode(SINGLE_SELECTION);
			return;
		}

		sint32 capacity = unit.GetDBRec()->GetCargoDataPtr()->GetMaxCargo();

		for (sint32 index = 0; index < k_MAX_CP_CARGO; index++)
		{
			ctp2_Switch * button = m_transportSelectionButton[index];
			if (index >= capacity)
			{
				button->Hide();
				m_transportSelectionCargo[index] = 0;
				m_transportSelectionHealth[index]->SetDrawCallbackAndCookie(NULL, NULL);
			}
			else
			{
				button->Show();
				if (index >= cargoList->Num())
				{
					if (m_transportSelectionCargo[index] != 0)
					{
						button->SetImage(NULL, 0);
						button->SetImage(NULL, 1);
						button->Enable(FALSE);
						m_transportSelectionCargo[index] = 0;
						m_transportSelectionHealth[index]->SetDrawCallbackAndCookie(NULL, NULL);
					}
				}
				else
				{
					if (m_transportSelectionCargo[index] != cargoList->Access(index).m_id)
					{
						const char * icon = cargoList->Access(index).GetDBRec()->GetDefaultIcon()->GetIcon();
						button->SetImage(icon, 0);
						button->SetImage(icon, 1);
						button->Enable(true);
						m_transportSelectionCargo[index] = cargoList->Access(index).m_id;
						m_transportSelectionHealth[index]->SetDrawCallbackAndCookie(HealthBarActionCallback,
								(void *)cargoList->Access(index).m_id);
					}
				}
			}
		}
	}
	else
	{
		for(sint32 index = 0; index < k_MAX_CP_CARGO; index++)
		{
			ctp2_Switch * button = m_transportSelectionButton[index];
			if (!button->IsHidden()) {
				button->Hide();
			}
		}

		SetSelectionMode(SINGLE_SELECTION);
	}
}

void UnitControlPanel::UpdateOrderButtons()
{
	static bool enableState[NUMBER_OF_ORDER_BUTTONS];

	Army army = GetSelectedArmy();
	if ((m_lastSelectionUnit == m_armySelectionUnit) && (army.m_id == m_lastSelectedArmy.m_id)
		&& ((army.IsValid() && army.Num() == m_lastSelectedArmyCount)
			|| (!army.IsValid() && !m_lastSelectedArmy.IsValid())))
	{
		return;
	}

	m_lastSelectionUnit     = m_armySelectionUnit;
	m_lastSelectedArmy      = army;
	m_lastSelectedArmyCount = army.IsValid() ? army.Num() : 0;

	for (sint32 orderIndex = 0; orderIndex < NUMBER_OF_ORDER_BUTTONS; orderIndex++)
	{
		m_orderButton[orderIndex]->ExchangeImage(4, 0, NULL);
		m_orderButton[orderIndex]->ShouldDraw();
		m_orderButton[orderIndex]->SetActionFuncAndCookie(NULL, NULL);

		enableState[orderIndex] = false;

		aui_TipWindow * tipWindow = (aui_TipWindow *)m_orderButton[orderIndex]->GetTipWindow();
		if (tipWindow) {
			tipWindow->SetTipText("");
		}
	}

	if (army.IsValid())
	{
		ArmyData * armyData = army.AccessData();
		if (armyData)
		{
			bool isShowOrderIntersection = !g_theProfileDB->GetValueByName("ShowOrderUnion");
			for (sint32 index = 0; index < g_theOrderDB->NumRecords(); index++)
			{
				const OrderRecord * orderRecord = g_theOrderDB->Get(index);
				if (stricmp(orderRecord->GetIDText(), "ORDER_ENSLAVE_SETTLER") == 0) {
					continue;
				}

				bool isSelectedCapable = (m_armySelectionUnit >= 0)
						&& armyData->TestOrderUnit(orderRecord, m_armySelectionUnit);
				bool isArmyCapable = isSelectedCapable || (isShowOrderIntersection
						? armyData->TestOrderAll(orderRecord) : armyData->TestOrderAny(orderRecord));

				if (isArmyCapable)
				{
					sint32 orderButtonIndex = orderRecord->GetButtonLocation();
					m_orderButton[orderButtonIndex]->ExchangeImage(4, 0, orderRecord->GetCPIcon());

					bool   full          = true;
					sint32 costs         = 0;
					sint32 fullCosts     = 0;
					sint8  numUpgrade    = 0;
					sint8  numUpgradeAll = 0;

					if (stricmp(orderRecord->GetEventName(), "UpgradeOrder") == 0
					    && armyData->UpgradeTypeAndCosts(full, costs, fullCosts, numUpgrade, numUpgradeAll))
					{
						char buffer[1024];
						sprintf(buffer, "%s, %d/%d/%d, %s%d/%d",
								g_theStringDB->GetNameStr(orderRecord->GetStatusText()), numUpgrade, numUpgradeAll,
								army->Num(), g_theStringDB->GetNameStr("str_ldl_Gold_COLON_"), costs, fullCosts);
						m_orderButton[orderButtonIndex]->SetStatusTextCopy(buffer);

						aui_TipWindow * tipWindow = (aui_TipWindow *)m_orderButton[orderButtonIndex]->GetTipWindow();
						if (tipWindow)
						{
							sprintf(buffer, "%s, %d/%d/%d, %s%d/%d",
									g_theStringDB->GetNameStr(orderRecord->GetLocalizedName()), numUpgrade,
									numUpgradeAll, army->Num(), g_theStringDB->GetNameStr("str_ldl_Gold_COLON_"),
									costs, fullCosts);
							tipWindow->SetTipText(buffer);
						}
					}
					else
					{
						m_orderButton[orderButtonIndex]->SetStatusText(
								g_theStringDB->GetNameStr(orderRecord->GetStatusText()));

						aui_TipWindow * tipWindow = (aui_TipWindow *)m_orderButton[orderButtonIndex]->GetTipWindow();
						if (tipWindow) {
							tipWindow->SetTipText((char *)g_theStringDB->GetNameStr(orderRecord->GetLocalizedName()));
						}
					}

					ORDER_TEST orderTest = armyData->TestOrder(orderRecord);

					sint32 range;
					if (!orderRecord->GetRange(range)) {
						range = 0;
					}

					if ((orderTest == ORDER_TEST_OK)
						|| (orderTest == ORDER_TEST_NEEDS_TARGET)
						|| (orderTest == ORDER_TEST_NO_MOVEMENT)
						|| (orderTest == ORDER_TEST_INVALID_TARGET))
					{
						m_orderPair[orderButtonIndex].first  = this;
						m_orderPair[orderButtonIndex].second = const_cast<OrderRecord*>(orderRecord);

						m_orderButton[orderButtonIndex]->SetActionFuncAndCookie(OrderButtonActionCallback,
								&m_orderPair[orderButtonIndex]);

						enableState[orderButtonIndex] = true;
					}
				}
			}
		}
	}

	for (sint32 orderIndex = 0; orderIndex < NUMBER_OF_ORDER_BUTTONS; orderIndex++)
	{
		if (m_orderButton[orderIndex]->IsDisabled())
		{
			if (enableState[orderIndex]) {
				m_orderButton[orderIndex]->Enable(true);
			}
		}
		else if (!enableState[orderIndex]) {
			m_orderButton[orderIndex]->Enable(false);
		}
	}
}


Army UnitControlPanel::GetSelectedArmy()
{
	if (!g_selected_item) {
		return Army();
	}

	Player * player = g_player[g_selected_item->GetVisiblePlayer()];

	if (!player) {
		return Army();
	}

	ID id;
	PLAYER_INDEX playerIndex;
	SELECT_TYPE selectionType;
	g_selected_item->GetTopCurItem(playerIndex, id, selectionType);
	if(selectionType == SELECT_TYPE_LOCAL_ARMY) {
		return id;
	}

	if (selectionType == SELECT_TYPE_LOCAL_CITY)
	{
		Unit city(id);
		Cell * cell = g_theWorld->GetCell(city.RetPos());
		if (cell->UnitArmy())
		{
			Unit top = cell->UnitArmy()->GetTopVisibleUnit(player->m_owner);
			return top.IsValid() ? top.GetArmy() : cell->AccessUnit(0).GetArmy();
		}
	}

	return Army();
}

void UnitControlPanel::GiveOrder(OrderRecord * order)
{
	g_controlPanel->BeginOrderDelivery(order);
}

void UnitControlPanel::UnitDisplayGroupCallback(aui_Region * region, void * userData)
{
	UnitControlPanel * panel = static_cast<UnitControlPanel*>(userData);
	panel->SetSelectionMode(panel->m_currentMode);
}

void UnitControlPanel::PrevUnitButtonActionCallback(aui_Control * control, uint32 action, uint32 data, void * cookie)
{
	if (action != static_cast<uint32>(AUI_BUTTON_ACTION_EXECUTE)) {
		return;
	}

	UnitControlPanel * panel = static_cast<UnitControlPanel*>(cookie);
	switch (panel->m_currentMode)
	{
		case SINGLE_SELECTION:
		case ARMY_SELECTION:
			panel->SetSelectionMode(MULTIPLE_SELECTION);
			break;

		case MULTIPLE_SELECTION:
		default:
			g_selected_item->NextUnmovedUnit();
			break;
	}
}

void UnitControlPanel::NextUnitButtonActionCallback(aui_Control * control, uint32 action, uint32 data, void * cookie)
{
	if (action != static_cast<uint32>(AUI_BUTTON_ACTION_EXECUTE)) {
		return;
	}

	UnitControlPanel * panel = static_cast<UnitControlPanel*>(cookie);
	switch (panel->m_currentMode)
	{
		case SINGLE_SELECTION:
		case ARMY_SELECTION:
			if (SelectionContainsMultipleArmies()) {
				panel->SetSelectionMode(MULTIPLE_SELECTION);
			} else {
				g_selected_item->NextUnmovedUnit();
			}
			break;

		case MULTIPLE_SELECTION:
		default:
			g_selected_item->NextUnmovedUnit();
			break;
	}
}

bool UnitControlPanel::SelectionContainsMultipleArmies()
{
	CellUnitList unitList;
	g_theWorld->GetCell(g_selected_item->GetCurSelectPos())->GetArmy(unitList);
	if (unitList.Num() <= 0) {
		return false;
	}

	Army army = unitList.Get(0).GetArmy();
	for (sint32 i = 1; i < unitList.Num(); i++)
	{
		if (unitList.Get(i).GetArmy() != army) {
			return true;
		}
	}
	return false;
}

AUI_ERRCODE UnitControlPanel::HealthBarActionCallback(ctp2_Static * control, aui_Surface * surface, RECT & rect,
		void * cookie)
{
	Unit unit(reinterpret_cast<uint32>(cookie));
	AUI_ERRCODE errorCode = g_c3ui->TheBlitter()->ColorBlt(surface, &rect, RGB(0,0,0), 0);
	if (errorCode != AUI_ERRCODE_OK) {
		return errorCode;
	}

	if (!unit.IsValid()) {
		return AUI_ERRCODE_OK;
	}

	double healthPercent = (unit->CalculateTotalHP() > 0)
			? static_cast<double>(unit.GetHP()) / static_cast<double>(unit->CalculateTotalHP()) : 1.0;

	Pixel16 color = RGB(255, 0, 0);
	if (healthPercent > 0.666) {
		color = RGB(0, 255, 0);
	} else if (healthPercent > 0.333) {
		color = RGB(255, 255, 0);
	}

	RECT healthRectangle = rect;
	sint32 width = healthRectangle.right - healthRectangle.left;
	healthRectangle.right = healthRectangle.left +
		static_cast<sint32>(static_cast<double>(width) * healthPercent);

	if (healthRectangle.right <= healthRectangle.left) {
		return AUI_ERRCODE_OK;
	}

	return g_c3ui->TheBlitter()->ColorBlt(surface, &healthRectangle, color, 0);
}

AUI_ERRCODE UnitControlPanel::FuelBarDrawCallback(ctp2_Static * control, aui_Surface * surface, RECT & rect,
		void * cookie)
{
	Unit unit (reinterpret_cast<uint32>(cookie));
	AUI_ERRCODE errCode = g_c3ui->TheBlitter()->ColorBlt(surface, &rect, RGB(0,0,0), 0);
	if (errCode != AUI_ERRCODE_OK) {
		return errCode;
	}

	if (!unit.IsValid()) {
		return AUI_ERRCODE_OK;
	}

	sint32 fuel, maxFuel;
	if (unit->GetUsedFuel(fuel, maxFuel))
	{
		double fuelPercent;
		if (maxFuel != 0) {
			fuelPercent = (double)fuel / (double)maxFuel;
		} else {
			fuelPercent = 0;
		}

		RECT fuelRect = rect;
		fuelRect.left   += 2;
		fuelRect.right  -= 2;
		fuelRect.top    += 2;
		fuelRect.bottom -= 2;

		sint32 width = fuelRect.right - fuelRect.left;
		width = sint32(fuelPercent * double(width));
		fuelRect.right = fuelRect.left + width;

		Pixel16 color = (fuelPercent < 0.2) ? RGB(255,0,0) : ((fuelPercent < 0.5) ? RGB(255,255,0)
				: RGB(0,255,0));
		errCode = g_c3ui->TheBlitter()->ColorBlt(surface, &fuelRect, color, 0);
	}
	return errCode;
}

void UnitControlPanel::MultiButtonActionCallback(aui_Control * control, uint32 action, uint32 data, void * cookie)
{
	if (action != static_cast<uint32>(AUI_BUTTON_ACTION_EXECUTE)) {
		return;
	}

	std::pair<UnitControlPanel*, uint32> *multiPair = static_cast<std::pair<UnitControlPanel*, uint32>*>(cookie);
	Army army(multiPair->second);
	if (army.IsValid())
	{
		Unit unit = army[0];
		if (unit.IsValid())
		{
			g_selected_item->SetSelectUnit(unit);
			multiPair->first->SetSelectionMode(ARMY_SELECTION);
		}
	}
}

void UnitControlPanel::ArmyButtonActionCallback(aui_Control * control, uint32 action, uint32 data, void * cookie)
{
	if (action != static_cast<uint32>(AUI_BUTTON_ACTION_EXECUTE)) {
		return;
	}

	UnitControlPanel * panel = static_cast<UnitControlPanel*>(cookie);
	for (sint32 armyIndex = 0; armyIndex < NUMBER_OF_ARMY_SELECTION_BUTTONS; armyIndex++)
	{
		if (panel->m_armySelectionButton[armyIndex] == control)
		{
			panel->SetSelectionMode(SINGLE_SELECTION);
			panel->m_curSingleArmy = 0; // Reset: single selection display
			panel->m_armySelectionUnit = armyIndex;
			break;
		}
	}

	panel->Update();
}

void UnitControlPanel::OrderButtonActionCallback(aui_Control * control, uint32 action, uint32 data, void * cookie)
{
	if (action != static_cast<uint32>(AUI_BUTTON_ACTION_EXECUTE)) {
		return;
	}

	std::pair<UnitControlPanel*, OrderRecord*> *orderPair =
			static_cast<std::pair<UnitControlPanel*, OrderRecord*>*>(cookie);
	orderPair->first->GiveOrder(orderPair->second);
}

void UnitControlPanel::Activated()
{
	m_lastSelectedArmy.m_id = 0;

	Army army;
	if (g_selected_item->GetSelectedArmy(army)) {
		return;
	}

	army = GetSelectedArmy();
	if (army.IsValid()) {
		g_selected_item->SetSelectUnit(army[0]);
	}
}

AUI_ERRCODE UnitControlPanel::DrawCargoCallback(ctp2_Static * control, aui_Surface * surface, RECT & rect,
		void * cookie)
{
	Unit transport(reinterpret_cast<uint32>(cookie));
	if (!transport.IsValid()) {
		return AUI_ERRCODE_OK;
	}

	sint32 numCarried = transport.GetNumCarried();
	sint32 capacity   = transport.GetDBRec()->GetCargoDataPtr()->GetMaxCargo();
	if (capacity > k_MAX_CP_CARGO) {
		capacity = k_MAX_CP_CARGO;
	}

	RECT box = rect;
	box.left   += 2;
	box.right  -= 2;
	box.bottom -= 2;
	box.top     = box.bottom - k_CP_CARGO_HEIGHT;

	sint32 boxWidth       = box.right - box.left;
	sint32 tickSpaceWidth = boxWidth / k_MAX_CP_CARGO;
	sint32 tickWidth      = tickSpaceWidth - 2;

	sint32 i;
	for (sint32 i = 0; i < capacity; i++)
	{
		RECT tickRect = { box.left + i * tickSpaceWidth, box.top, box.left + i * tickSpaceWidth + tickWidth,
				box.bottom };
		Pixel16 color = (i < numCarried) ? RGB(0,255,0) : RGB(0,0,0);
		g_c3ui->TheBlitter()->ColorBlt(surface, &tickRect, color, 0);
	}
	return AUI_ERRCODE_OK;
}

class UnitControlPanel::SetSelectionAction : public aui_Action
{
public:
	SetSelectionAction(UnitControlPanel * panel, UnitControlPanel::UnitSelectionMode mode)
	: aui_Action(),
		m_panel(panel),
		m_mode(mode)
	{
	}

	virtual void Execute(aui_Control * control, uint32 action, uint32 data)
	{
		// transition to TRANSPORT_SELECTION is only valid from SINGLE_SELECTION
		if (m_mode == TRANSPORT_SELECTION && m_panel->m_currentMode != SINGLE_SELECTION) {
			return;
		}
		m_panel->DoSetSelectionMode(m_mode);
	}

private:
	UnitControlPanel                    * m_panel;
	UnitControlPanel::UnitSelectionMode   m_mode;
};

void UnitControlPanel::SingleSelectionArmySymbolImageCallback(ctp2_Static * control, aui_MouseEvent * event,
		void * cookie)
{
	UnitControlPanel * panel = static_cast<UnitControlPanel *>(cookie);
	g_c3ui->AddAction(new SetSelectionAction(panel, ARMY_SELECTION));
}

void UnitControlPanel::TransportImageCallback(ctp2_Static * control, aui_MouseEvent * event, void * cookie)
{
	UnitControlPanel * panel = static_cast<UnitControlPanel *>(cookie);
	g_c3ui->AddAction(new SetSelectionAction(panel, TRANSPORT_SELECTION));
}

void UnitControlPanel::TransportSelectionImageCallback(ctp2_Static * control, aui_MouseEvent * event, void * cookie)
{
	UnitControlPanel * panel = static_cast<UnitControlPanel *>(cookie);
	g_c3ui->AddAction(new SetSelectionAction(panel, SINGLE_SELECTION));
}

bool UnitControlPanel::GetSelectedCargo(CellUnitList & cargoList)
{
	cargoList.Clear();
	if (m_transportSelectionDisplay->IsHidden()) {
		return false;
	}

	for(sint32 index = 0; index < k_MAX_CP_CARGO; index++)
	{
		Unit cargo(m_transportSelectionCargo[index]);
		if (m_transportSelectionButton[index]->GetState() && cargo.IsValid()) {
			cargoList.Insert(cargo);
		}
	}
	return true;
}

void UnitControlPanel::updateSingleSelectionSymbols()
{
	if (m_armySelectionUnit >= 0)
	{
		m_singleSelectionArmySymbol->Show();
		m_singleSelectionCargoSymbol->Hide();
	}
	else
	{
		m_singleSelectionArmySymbol->Hide();
		if (m_curCargo >= 0) {
			m_singleSelectionCargoSymbol->Show();
		} else {
			m_singleSelectionCargoSymbol->Hide();
		}
	}
}

void UnitControlPanel::UnsetCargoButtons()
{
	for(sint32 index = 0; index < k_MAX_CP_CARGO; index++) {
		m_transportSelectionButton[index]->SetState(false);
	}
}
