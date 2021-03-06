/*
    @AdvBankingTraderConfig
    Author: Krypton91
	Credits: DrJones
    License: You are allowed to repack this and edit this, but you need this text with my name in the files!
*/
modded class MissionServer
{
    protected ref AdvBankingTraderConfig m_TraderAddinServerConfig;
    override void OnInit()
    {
        super.OnInit();
        m_TraderAddinServerConfig = AdvBankingTraderConfig.LoadConfig();
        if(m_TraderAddinServerConfig)
            Print("[Advanced Banking Trader Addin] -> Server Config Loadet!");
        //assign static stuff from cfg
        NeedsBankCardInHandToPayOnTrader = m_TraderAddinServerConfig.NeedsBankCardInHandToPayOnTrader;
        GetRPCManager().AddRPC("KR_BANKINGTRADERADDIN", "ServerConfigRequest", this, SingleplayerExecutionType.Server);
    }

    void ServerConfigRequest(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if(type == CallType.Server)
        {
            GetRPCManager().SendRPC("KR_BANKINGTRADERADDIN", "ConfigResponse", new Param2< bool, bool>( m_TraderAddinServerConfig.NeedsBankCardInHandToPayOnTrader, m_TraderAddinServerConfig.CanOnlyUseIfCurrencyOnPlayerIsZero ), true, sender);
        }
    }
};