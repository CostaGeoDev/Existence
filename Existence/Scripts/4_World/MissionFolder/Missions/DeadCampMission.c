class DeadCampMission : BaseMission
{
    TentBase MissionTent;
    private D_CustomTrigger MissionTrigger;
    private ref array<PlayerBase> playersInside;
    private float missionStartTime;
    private ref DeadCampMissionConfig DCconfig;
    private ref MainSettingsConfig MSconfig;

    void DeadCampMission(string name)
    {
        BaseMission(name);
    }

    override vector GetMissionTriggerCoordinates()
    {
        // Return the specific trigger coordinates for this mission
        return GetDeadCampMissionConfig().tentLocationCoordinates;
    }

    // If the activatedMission flag is set to true, the mission will be started, if false, do nothing
    override void StartMission()
    {
        MSconfig = GetMainSettingsConfig();

        if (!MSconfig)
        {
            Print("[DeadCampMission] MSconfig is NULL! Check GetMainSettingsConfig().");
            Print("[DeadCampMission] MSconfig Object: " + MSconfig);
            Print("[DeadCampMission] RadioFrequency: " + MSconfig.RadioFrequency);
            return;
        }

        Print("[DeadCampMission] Initializing mission setup.");

        playersInside = new array<PlayerBase>();
        DCconfig = GetDeadCampMissionConfig();
        missionStartTime = GetGame().GetTime();

        // Spawn mission-related entities
        if (!SpawnMissionTent()) return;
        if (!SpawnMissionAsset_1()) return;
        if (!SpawnMissionAsset_2()) return;
        if (!SpawnMissionAsset_3()) return;
        if (!SpawnInfected()) return;
        

        // Setup mission trigger
        SetupMissionTrigger(DCconfig.tentLocationCoordinates, DCconfig.missionOuterRadius);

        // Schedule mission timer
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CheckMissionTime, DCconfig.missionTime * 1000, true);
        Print("[DeadCampMission] Mission successfully started.");
    }

    private void CheckMissionTime()
    {
        float elapsedTime = (GetGame().GetTime() - missionStartTime) / 1000.0; // Convert to seconds
        Print("[DeadCampMission] Mission elapsed time: " + elapsedTime + " seconds.");

        if (elapsedTime >= DCconfig.missionTime)
        {
            EndMission();
        }
    }

    protected void EndMission()
    {
        Print("[DeadCampMission] Mission time has expired. Ending mission.");
        if (MissionTrigger)
        {
            MissionTrigger.Delete(); // Clean up trigger
        }

        // Additional cleanup or rewards logic can be added here
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(CheckMissionTime);
    }

    private bool SpawnMissionTent()
    {
        Print("[DeadCampMission] Spawning mission tent at " + DCconfig.tentLocationCoordinates + "...");

        MissionTent = TentBase.Cast(GetGame().CreateObject(DCconfig.tentTypeName, DCconfig.tentLocationCoordinates));

        if (!MissionTent)
        {
            Print("[DeadCampMission] Error: Failed to spawn mission tent.");
            return false;
        }

        MissionTent.SetPosition(DCconfig.tentLocationCoordinates);
        MissionTent.SetOrientation(DCconfig.tentOrientation);
        MissionTent.Pitch( true , true );
        MissionTent.PlaceOnSurface();

        for (int d = 0; d < 20; d++)
        {
            string randomItem = DCconfig.tentLootItems.GetRandomElement();
            MissionTent.GetInventory().CreateInInventory(randomItem);
            Print("[DeadCampMission] Added " + randomItem + " to the mission tent.");
        }

        Print("[DeadCampMission] Mission tent spawned successfully.");
        return true;
    }

    private bool SpawnMissionAsset_1()
    {
        Print("[DeadCampMission] Spawning mission asset 1 at " + DCconfig.assetLocationCoordinates_1 + "...");

        Object MissionAsset_1 = GetGame().CreateObject(DCconfig.assetTypeName_1, DCconfig.assetLocationCoordinates_1);

        if (!MissionAsset_1)
        {
            Print("[DeadCampMission] Error: Failed to spawn mission asset 1.");
            return false;
        }

        MissionAsset_1.SetPosition(DCconfig.assetLocationCoordinates_1);
        MissionAsset_1.SetOrientation(DCconfig.assetOrientation_1);
        MissionAsset_1.PlaceOnSurface();

        Print("[DeadCampMission] Mission asset 1 spawned successfully.");
        return true;
    }

    private bool SpawnMissionAsset_2()
    {
        Print("[DeadCampMission] Spawning mission asset 2 at " + DCconfig.assetLocationCoordinates_2 + "...");

        Object MissionAsset_2 = GetGame().CreateObject(DCconfig.assetTypeName_2, DCconfig.assetLocationCoordinates_2);

        if (!MissionAsset_2)
        {
            Print("[DeadCampMission] Error: Failed to spawn mission asset 2.");
            return false;
        }

        MissionAsset_2.SetPosition(DCconfig.assetLocationCoordinates_2);
        MissionAsset_2.SetOrientation(DCconfig.assetOrientation_2);
        MissionAsset_2.PlaceOnSurface();

        Print("[DeadCampMission] Mission asset 2 spawned successfully.");
        return true;
    }

    private bool SpawnMissionAsset_3()
    {
        Print("[DeadCampMission] Spawning mission asset 3 at " + DCconfig.assetLocationCoordinates_3 + "...");

        Object MissionAsset_3 = GetGame().CreateObject(DCconfig.assetTypeName_3, DCconfig.assetLocationCoordinates_3);

        if (!MissionAsset_3)
        {
            Print("[DeadCampMission] Error: Failed to spawn mission asset 3.");
            return false;
        }

        MissionAsset_3.SetPosition(DCconfig.assetLocationCoordinates_3);
        MissionAsset_3.SetOrientation(DCconfig.assetOrientation_3);
        MissionAsset_3.PlaceOnSurface();

        Print("[DeadCampMission] Mission asset 3 spawned successfully.");
        return true;
    }

    private bool SpawnInfected()
    {
        Print("[DeadCampMission] Spawning infected AI...");
        for (int i = 0; i < DCconfig.infectedAmount; i++)
        {
            vector InfectedPosition = DCconfig.tentLocationCoordinates + Vector(Math.RandomFloat(-15, 15), 0, Math.RandomFloat(-15, 15));
            DayZInfected Infected = DayZInfected.Cast(GetGame().CreateObject(DCconfig.infectedTypes.GetRandomElement(), InfectedPosition, false, true));

            if (Infected)
            {
                Infected.SetPosition(InfectedPosition);
                Infected.PlaceOnSurface();
                Print("[DeadCampMission] Spawned infected '" + Infected.GetType() + "' at " + InfectedPosition + ".");
            }
        }

        Print("[DeadCampMission] Infected AI spawned successfully.");
        return true;
    }

    private void SetupMissionTrigger(vector position, float radius)
    {
        MissionTrigger = D_CustomTrigger.Cast(GetGame().CreateObject("D_CustomTrigger", position));
        if (MissionTrigger)
        {
            MissionTrigger.InitializeTrigger(radius, this);
            Print("[DeadCampMission] Trigger created at " + position + " with radius " + radius + ".");
        }
        else
        {
            Print("[DeadCampMission] Error: Failed to create mission trigger.");
        }
    }

    override void OnEnterMissionTrigger(PlayerBase player)
    {
        if (playersInside.Find(player) == -1)
        {
            playersInside.Insert(player);
            Print("[DeadCampMission] Player " + player.GetIdentity().GetName() + " entered the mission trigger.");
            NotifyPlayer (player, "You've entered the mission area! Good luck!");
        }
    }

    override void OnExitMissionTrigger(PlayerBase player)
    {
        int index = playersInside.Find(player);
        if (index != -1)
        {
            playersInside.Remove(index);
            Print("[DeadCampMission] Player " + player.GetIdentity().GetName() + " exited the mission trigger.");
            NotifyPlayer(player, "You've left the mission area! Are you scared?");
        }
    }

    private void NotifyPlayer(PlayerBase player, string message)
    {
        TransmitterBase transmitter;
        if (Class.CastTo(transmitter, player.GetItemInHands()) && transmitter.GetCompEM() && transmitter.GetCompEM().IsWorking())
        {
            float frequency = transmitter.GetTunedFrequency();
            string frequencyStr = frequency.ToString();  // Convert float to string

            // Check if frequency is in the allowed list
            array<string> allowedFrequencies = MSconfig.RadioFrequency;
            if (allowedFrequencies.Find(frequencyStr) > -1)
            {
                transmitter.GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>(message), true, player.GetIdentity());
            }
        }
    }
}

class D_CustomTrigger : Trigger
{
    private ref array<PlayerBase> m_CurrentPlayers;
    private ref array<PlayerBase> m_PreviousPlayers;
    private float m_Radius;
    private BaseMission m_Mission;

    void D_CustomTrigger()
    {
        m_CurrentPlayers = new array<PlayerBase>();
        m_PreviousPlayers = new array<PlayerBase>();
    }

    void InitializeTrigger(float radius, BaseMission mission)
    {
        m_Radius = radius;
        m_Mission = mission;
        SetExtents(Vector(-radius, 0, -radius), Vector(radius, 1, radius));
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckForPlayerInside, 1000, true);
    }

    private void CheckForPlayerInside()
    {
        m_CurrentPlayers.Clear();
        array<Object> entities = new array<Object>();
        GetGame().GetObjectsAtPosition(GetPosition(), m_Radius, entities, NULL);

        foreach (Object obj : entities)
        {
            PlayerBase player = PlayerBase.Cast(obj);
            if (player)
            {
                m_CurrentPlayers.Insert(player);
                if (m_PreviousPlayers.Find(player) == -1)
                {
                    m_Mission.OnEnterMissionTrigger(player);
                }
            }
        }

        foreach (PlayerBase previousPlayer : m_PreviousPlayers)
        {
            if (m_CurrentPlayers.Find(previousPlayer) == -1)
            {
                m_Mission.OnExitMissionTrigger(previousPlayer);
            }
        }

        m_PreviousPlayers.Copy(m_CurrentPlayers);
    }
}