class CfgPatches 
{
	class AdvancedBankingTraderAddin 
	{
		units[] = 
		{
            "AdvancedBankingTraderAddin"
		};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = 
		{
			"DZ_Data",
			"KR_Banking"
		};
	};
};

class CfgMods 
{
	class AdvancedBankingTraderAddin 
	{
		dir = "AdvancedBankingTraderAddin";
		picture = "";
		action = "https://DeutscheBohrmaschine.de";
		hideName = 1;
		hidePicture = 1;
		name = "Advanced Banking";
		credits = "Krypton91";
		author = "Krypton91";
		authorID = "76561198796326626";
		version = "1.11";
		extra = 0;
		type = "mod";
		dependencies[] = {"Mission", "World", "Game"};

	    class defs
	    {
			class gameScriptModule
            {
                value = "";
                files[] = {"AdvancedBankingTraderAddin/scripts/3_Game"};
            };
			class worldScriptModule
            {
                value = "";
                files[] = {"AdvancedBankingTraderAddin/scripts/4_World"};
            };
			class missionScriptModule
            {
                value = "";
                files[] = {"AdvancedBankingTraderAddin/scripts/5_Mission"};
            };
        };
	};
};