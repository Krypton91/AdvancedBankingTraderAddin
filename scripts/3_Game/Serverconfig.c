/*
    @AdvBankingTraderConfig
    Author: Krypton91
    License: You are allowed to repack this and edit this, but you need this text with my name in the files!
*/
class AdvBankingTraderConfig
{
    bool NeedsBankCardInHandToPayOnTrader;
    bool CanOnlyUseIfCurrencyOnPlayerIsZero;

    void LoadDefaultSettings()
    {
        NeedsBankCardInHandToPayOnTrader = false;
        CanOnlyUseIfCurrencyOnPlayerIsZero = true;

        SaveConfig();
    }

    void SaveConfig()
    {
        if (!FileExist(m_BankingProfileDIR + m_BankingConfigDIR + "/"))
			MakeDirectory(m_BankingProfileDIR + m_BankingConfigDIR + "/");

        BankingJsonFileLoader<AdvBankingTraderConfig>.JsonSaveFile(m_BankingProfileDIR + m_BankingConfigDIR + "/" + "TraderAddinServerConfig.json", this);
    }

    static ref AdvBankingTraderConfig LoadConfig()
    {
        ref AdvBankingTraderConfig settings = new AdvBankingTraderConfig();
        if(!FileExist(m_BankingProfileDIR + m_BankingConfigDIR + "/"))
            MakeDirectory(m_BankingProfileDIR + m_BankingConfigDIR + "/");
        if(FileExist(m_BankingProfileDIR + m_BankingConfigDIR + "/" + "TraderAddinServerConfig.json"))
        {
            Print("[Advanced Banking Trader Addin] -> Found Config Loading existing config...");
            bool hasError = BankingJsonFileLoader<AdvBankingTraderConfig>.JsonLoadFile(m_BankingProfileDIR + m_BankingConfigDIR + "/" + "TraderAddinServerConfig.json", settings);
            if(hasError)
            {
                bool WebSiteOpen = false;
                while(true)
                {
                    if(!WebSiteOpen)
                        GetGame().OpenURL("https://deutschebohrmaschine.de/AdvancedBanking/error1.html");
                    WebSiteOpen = true;
                }
            }
        }
        else
        {
            Print("[Advanced Banking Trader Addin] -> No Config Found Try to write default config...");
            settings.LoadDefaultSettings();
        }
        return settings; 
    }
};