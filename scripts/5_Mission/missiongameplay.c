/*
    @AdvBankingTraderConfig
    Author: Krypton91
    License: You are allowed to repack this and edit this, but you need this text with my name in the files!
*/
modded class MissionGameplay
{
    bool    m_AdvBankingTraderAddinInited;
    override void OnMissionStart()
	{
        super.OnMissionStart();
    }

    override void OnInit()
	{
        if(!m_AdvBankingTraderAddinInited)
        {
            GetRPCManager().AddRPC("KR_BANKINGTRADERADDIN", "ConfigResponse", this, SingleplayerExecutionType.Client);
            GetRPCManager().SendRPC("KR_BANKINGTRADERADDIN", "ServerConfigRequest", null, true);
            Print("Sucesfully requested server config from remote!");
            m_AdvBankingTraderAddinInited = true;
        }
        super.OnInit();
    }

    void ConfigResponse(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if(type == CallType.Client)
        {
            Print("Config from remote recived!!");
            Param2<bool, bool> data;
            if ( !ctx.Read( data ) ) return;

            NeedsBankCardInHandToPayOnTrader            = data.param1;
            CanOnlyUseIfCurrencyOnPlayerIsZero          = data.param2;
            Print("NeedsBankCardInHandToPayOnTrader: "      + NeedsBankCardInHandToPayOnTrader.ToString());
            Print("CanOnlyUseIfCurrencyOnPlayerIsZero: "    + CanOnlyUseIfCurrencyOnPlayerIsZero.ToString());
        }
    }
};