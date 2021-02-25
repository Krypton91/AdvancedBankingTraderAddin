/*
    @AdvBankingTraderConfig
    Author: Krypton91
	Credits: DrJones
    License: You are allowed to repack this and edit this, but you need this text with my name in the files!
*/
modded class TraderMenu extends UIScriptedMenu
{
    override int getPlayerCurrencyAmount() // duplicate
	{
		PlayerBase m_Player = PlayerBase.Cast(GetGame().GetPlayer());
		
		int currencyAmount = 0;
		
		array<EntityAI> itemsArray = new array<EntityAI>;
		m_Player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);

		ItemBase item;
		
		for (int i = 0; i < itemsArray.Count(); i++)
		{
			Class.CastTo(item, itemsArray.Get(i));

			if (!item)
				continue;

			for (int j = 0; j < m_Player.m_Trader_CurrencyClassnames.Count(); j++)
			{
				if(item.GetType() == m_Player.m_Trader_CurrencyClassnames.Get(j))
				{
					currencyAmount += getItemAmount(item) * m_Player.m_Trader_CurrencyValues.Get(j);
				}
			}
		}
		
		if(NeedsBankCardInHandToPayOnTrader)
		{
			EntityAI itemInHands = m_Player.GetHumanInventory().GetEntityInHands();
			KeyCard_Base atmcard;
			if(itemInHands && Class.CastTo(atmcard, item))
			{
				return GetBankingClientManager().GetBankCredits();//we just do this when player has 0 Money in Inventory!
			}
		}

        if(currencyAmount <= 0 && CanOnlyUseIfCurrencyOnPlayerIsZero)
        {
            currencyAmount = GetBankingClientManager().GetBankCredits();//we just do this when player has 0 Money in Inventory!
        }

		return currencyAmount;
	}


	override void OnShow()
	{
		GetRPCManager().SendRPC("KR_BANKING", "PlayerDataRequest", null, true); //Request Banking data (planed to implement this in GetBankingClientManager())


		super.OnShow();

		PPEffects.SetBlurMenu(0.5);

		GetGame().GetInput().ChangeGameFocus(1);

		SetFocus( layoutRoot );
	}
};