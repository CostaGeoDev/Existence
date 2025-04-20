modded class MissionServer
{
    private ref MissionManager missionManager;

    override void OnInit()
    {
        GetMainSettingsConfig();
        GetHordeMissionConfig();
        GetBoatWreckMissionConfig();
        GetGarageGoodsMissionConfig();
        GetDeadCampMissionConfig();
        GetGraveyardStashMissionConfig();
        super.OnInit();

        Print("[MainSettingsConfig] OnInit - Loaded config successfully.");
        Print("[HordeMissionConfig] OnInit - Loaded config successfully.");
        Print("[BoatWreckMissionConfig] OnInit - Loaded config successfully.");
        Print("[GarageGoodsMissionConfig] OnInit - Loaded config successfully.");

        if (GetMainSettingsConfig().missionsActive)
        {
            missionManager = new MissionManager();

            int delay = GetMainSettingsConfig().initiateTimer;
            Print("[Existence Mod Active] Server started - Missions will start in " + delay + " seconds and wait for Trigger");

            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StartRandomMission, delay * 1000, false);
        }
        else
        {
            Print("[MissionServer] Missions are disabled via config.");
        }
    }

    void StartRandomMission()
    {
        if (missionManager)
        {
            missionManager.StartRandomMission();
        }
    }
}