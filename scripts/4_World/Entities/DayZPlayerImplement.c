/*
    @AdvBankingTraderConfig
    Author: Krypton91
	Credits: DrJones
    License: You are allowed to repack this and edit this, but you need this text with my name in the files!
*/
modded class DayZPlayerImplement
{
    override void handleBuyRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		Param3<int, int, string> rpb = new Param3<int, int, string>( -1, -1, "" );
		ctx.Read(rpb);

		int traderUID = rpb.param1;
		int itemID = rpb.param2;
		itemDisplayNameClient = rpb.param3;

		m_Trader_IsSelling = false;

		if (GetGame().GetTime() - m_Trader_LastBuyedTime < m_Trader_BuySellTimer * 1000)
			return;

		m_Trader_LastBuyedTime = GetGame().GetTime();

		if (itemID >= m_Trader_ItemsClassnames.Count() || itemID < 0 || traderUID >= m_Trader_TraderPositions.Count() || traderUID < 0)
			return;

		string itemType = m_Trader_ItemsClassnames.Get(itemID);
		int itemQuantity = m_Trader_ItemsQuantity.Get(itemID);
		int itemCosts = m_Trader_ItemsBuyValue.Get(itemID);

		vector playerPosition = this.GetPosition();	

		if (vector.Distance(playerPosition, m_Trader_TraderPositions.Get(traderUID)) > 1.7)
		{
			traderServerLog("tried to access the Trader out of Range! This could be an Hacker!");
			return;
		}

		m_Player_CurrencyAmount = getPlayerCurrencyAmount();

		if (itemCosts < 0)
		{
			TraderMessage.PlayerWhite("#tm_cant_be_bought", this);
			return;
		}

		if (!CanPayCosts(m_Player_CurrencyAmount))
		{
			TraderMessage.PlayerWhite("#tm_cant_afford", this);
			return;
		}

		int vehicleKeyHash = 0;

		bool isDuplicatingKey = false;
		if (itemQuantity == -7) // is Key duplication
		{
			VehicleKeyBase vehicleKeyinHands = VehicleKeyBase.Cast(this.GetHumanInventory().GetEntityInHands());

			if (!vehicleKeyinHands)
			{
				TraderMessage.PlayerWhite("Put the Key you\nwant to duplicate\nin your Hands!", this);
				return;
			}

			isDuplicatingKey = true;
			vehicleKeyHash = vehicleKeyinHands.GetHash();
			itemType = vehicleKeyinHands.GetType();
			itemQuantity = 1;
		}

		bool isLostKey = false;
		if (itemQuantity == -8) // is Lost Key
		{
			array<Transport> foundVehicles = GetVehicleToGetKeyFor(traderUID);

			if (foundVehicles.Count() < 1)
			{
				TraderMessage.PlayerWhite("There is no Vehicle\nin the Spawn Area!\nMake sure you was the last Driver!", this);
				return;
			}

			if (foundVehicles.Count() > 1)
			{
				TraderMessage.PlayerWhite("Multiple Vehicles found\nin the Spawn Area!", this);
				return;
			}

			CarScript carScript;
			Class.CastTo(carScript, foundVehicles.Get(0));

			vehicleKeyHash = carScript.m_Trader_VehicleKeyHash

			if (canCreateItemInPlayerInventory("VehicleKeyBase", 1))
			{
				TraderMessage.PlayerWhite(getItemDisplayName("VehicleKey") + "\n " + "#tm_added_to_inventory", this);
				vehicleKeyHash = createVehicleKeyInPlayerInventory(vehicleKeyHash);
			}
			else
			{
				TraderMessage.PlayerWhite("#tm_inventory_full" + "\n" + getItemDisplayName("VehicleKey") + "\n" + "#tm_was_placed_on_ground", this);
				vehicleKeyHash = spawnVehicleKeyOnGround(vehicleKeyHash);
				GetGame().RPCSingleParam(this, TRPCs.RPC_SEND_MENU_BACK, new Param1<bool>(false), true, this.GetIdentity());
			}

			deductPlayerCurrency(itemCosts);
			
			carScript.m_Trader_HasKey = true;
			carScript.m_Trader_VehicleKeyHash = vehicleKeyHash;
			carScript.SynchronizeValues();

			isLostKey = true;
			itemType = "VehicleKeyLost";
			itemQuantity = 1;
		}

		traderServerLog("bought " + getItemDisplayName(itemType) + "(" + itemType + ")");

		if (itemQuantity == -2 || itemQuantity == -6) // Is a Vehicle
		{
			string blockingObject = isVehicleSpawnFree(traderUID);

			if (blockingObject != "FREE")
			{
				TraderMessage.PlayerWhite(getItemDisplayName(blockingObject) + " " + "#tm_way_blocked", this);
				return;
			}

			if (itemQuantity == -2)
			{
				if (canCreateItemInPlayerInventory("VehicleKeyBase", 1))
				{
					TraderMessage.PlayerWhite(getItemDisplayName("VehicleKey") + "\n " + "#tm_added_to_inventory", this);
					
					vehicleKeyHash = createVehicleKeyInPlayerInventory();
				}
				else
				{
					TraderMessage.PlayerWhite("#tm_inventory_full" + "\n" + getItemDisplayName("VehicleKey") + "\n" + "#tm_was_placed_on_ground", this);
										
					vehicleKeyHash = spawnVehicleKeyOnGround();
					
					GetGame().RPCSingleParam(this, TRPCs.RPC_SEND_MENU_BACK, new Param1<bool>( false ), true, this.GetIdentity());
				}
				//TraderMessage.PlayerWhite("KeyHash:\n" + vehicleKeyHash, this);
			}

			deductPlayerCurrency(itemCosts);

			TraderMessage.PlayerWhite("" + itemDisplayNameClient + "\n" + "#tm_parked_next_to_you", this);

			spawnVehicle(traderUID, itemType, vehicleKeyHash);

			GetGame().RPCSingleParam(this, TRPCs.RPC_SEND_MENU_BACK, new Param1<bool>( false ), true, this.GetIdentity());
		}
		else if (itemType != "VehicleKeyLost") // Is not a Vehicle
		{		
			deductPlayerCurrency(itemCosts);

			if (canCreateItemInPlayerInventory(itemType, itemQuantity))
			{
				TraderMessage.PlayerWhite("" + itemDisplayNameClient + "\n" + "#tm_added_to_inventory", this);
				
				if (isDuplicatingKey)
					createVehicleKeyInPlayerInventory(vehicleKeyHash, itemType);
				else
					createItemInPlayerInventory(itemType, itemQuantity);
			}
			else
			{
				TraderMessage.PlayerWhite("#tm_inventory_full" + "\n " + itemDisplayNameClient + "\n" + "#tm_was_placed_on_ground", this);
									
				if (isDuplicatingKey)
					spawnVehicleKeyOnGround(vehicleKeyHash, itemType);
				else			
					spawnItemOnGround(itemType, itemQuantity, playerPosition);
				
				GetGame().RPCSingleParam(this, TRPCs.RPC_SEND_MENU_BACK, new Param1<bool>( false ), true, this.GetIdentity());
			}
		}

		//deductPlayerCurrency(itemCosts);
	}

    bool CanPayCosts(int costsToPay)
    {
        PlayerBase player = PlayerBase.Cast(this);
        if(costsToPay < getPlayerCurrencyAmount())
        {
            KR_JsonDatabaseHandler playerdata = KR_JsonDatabaseHandler.LoadPlayerData(player.GetIdentity().GetPlainId(), player.GetIdentity().GetName());
            if(playerdata)
            {
                if(costsToPay < playerdata.GetBankCredit())
                    return true;
            }
        }
        else
        {
            return true;
        }
        return false;
    }


    override void deductPlayerCurrency(int currencyAmount)
	{		
		if (currencyAmount == 0)
			return;

        int currencyAmountOnPlayer = getPlayerCurrencyAmount();
        if(currencyAmountOnPlayer < currencyAmount)
        {
            //use banking
            KR_JsonDatabaseHandler playerdata = KR_JsonDatabaseHandler.LoadPlayerData(this.GetIdentity().GetPlainId(), this.GetIdentity().GetName());
            if(playerdata)
            {
                Print("playerdata was loadet! deduct money now!");
                if(playerdata.GetBankCredit() >= currencyAmount)
                    playerdata.WitdrawMoney(currencyAmount);
            }
        }
        else
        {
            //Use Tradercode.
            array<EntityAI> itemsArray = new array<EntityAI>;
            ItemBase item;
            this.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);
            
            for (int i = 0; i < m_Trader_CurrencyClassnames.Count(); i++)
            {
                for (int j = 0; j < itemsArray.Count(); j++)
                {
                    Class.CastTo(item, itemsArray.Get(j));
                    
                    if (!item)
                        continue;

                    if(item.GetType() == m_Trader_CurrencyClassnames.Get(i))
                    {
                        int itemAmount = getItemAmount(item);

                        if(itemAmount * m_Trader_CurrencyValues.Get(i) > currencyAmount)
                        {
                            if (currencyAmount >= m_Trader_CurrencyValues.Get(i))
                            {
                                SetItemAmount(item, itemAmount - (currencyAmount / m_Trader_CurrencyValues.Get(i)));

                                this.UpdateInventoryMenu(); // RPC-Call needed?
                                
                                currencyAmount -= (currencyAmount / m_Trader_CurrencyValues.Get(i)) * m_Trader_CurrencyValues.Get(i);
                            }


                            if (currencyAmount < m_Trader_CurrencyValues.Get(i))
                            {
                                exchangeCurrency(item, currencyAmount, m_Trader_CurrencyValues.Get(i));

                                return;
                            }
                        }
                        else
                        {
                            deleteItem(itemsArray.Get(j));
                            
                            this.UpdateInventoryMenu(); // RPC-Call needed?
                            
                            currencyAmount -= itemAmount * m_Trader_CurrencyValues.Get(i);
                        }
                    }
                }
            }
        }
	}
};