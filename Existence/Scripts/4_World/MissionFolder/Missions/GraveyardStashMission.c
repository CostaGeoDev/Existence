class GraveyardStashMission : BaseMission
{
    ItemBase MissionObject;
    UndergroundStash stash;

    private GY_CustomTrigger MissionTrigger;
    private ref array<PlayerBase> playersInside;
    private float missionStartTime;
    private ref GraveyardStashMissionConfig GYconfig;
    private ref MainSettingsConfig MSconfig;

    void GraveyardStashMission(string name)
    {
        BaseMission(name);
    }

    override vector GetMissionTriggerCoordinates()
    {
        return GetGraveyardStashMissionConfig().stashCoordinates;
    }

    override void StartMission()
    {
        MSconfig = GetMainSettingsConfig();

        if (!MSconfig)
        {
            Print("[GraveyardStashMission] MSconfig is NULL! Check GetMainSettingsConfig().");
            return;
        }

        Print("[GraveyardStashMission] Initializing mission setup.");

        playersInside = new array<PlayerBase>();
        GYconfig = GetGraveyardStashMissionConfig();
        missionStartTime = GetGame().GetTime();

        if (!SpawnStashContainer()) return;
        if (!SpawnInfected()) return;

        // Setup mission trigger
        SetupMissionTrigger(GYconfig.stashCoordinates, GYconfig.missionOuterRadius);

        // Schedule mission timer
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CheckMissionTime, GYconfig.missionTime * 1000, false);
        Print("[GraveyardStashMission] Mission successfully started.");
    }

    private void CheckMissionTime()
    {
        float elapsedTime = (GetGame().GetTime() - missionStartTime) / 1000.0;
        Print("[GraveyardStashMission] Mission elapsed time: " + elapsedTime + " seconds.");

        if (elapsedTime >= GYconfig.missionTime)
        {
            EndMission();
        }
    }

    protected void EndMission()
    {
        Print("[GraveyardStashMission] Mission time expired. Cleaning up...");

        if (MissionTrigger)
        {
            MissionTrigger.Delete();
        }

        if (stash)
        {
            stash.Delete();
            stash = null;
        }

        if (MissionObject)
        {
            MissionObject.Delete();
            MissionObject = null;
        }

        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(CheckMissionTime);
        Print("[GraveyardStashMission] Mission ended successfully.");
    }

    private bool SpawnStashContainer()
    {
        if (!GYconfig.stashContainerItem || GYconfig.stashCoordinates == vector.Zero)
        {
            Print("[GraveyardStashMission] Error: Invalid stash configuration.");
            return false;
        }

        Print("[GraveyardStashMission] Spawning stash at " + GYconfig.stashCoordinates + "...");

        stash = UndergroundStash.Cast(GetGame().CreateObject("UndergroundStash", GYconfig.stashCoordinates, false, true));
        if (!stash)
        {
            Print("[GraveyardStashMission] Error: Failed to spawn graveyard stash.");
            return false;
        }

        stash.PlaceOnSurface();
        stash.SetPosition(GYconfig.stashCoordinates);
        //stash.SetOrientation(GYconfig.stashOrientation);

        vector pos = stash.GetPosition();
        vector ori = stash.GetOrientation();
        if (ori[2] == -180 || ori[2] == 180)
        {   //Bugfix for invisible Undergroundstashes
            ori[2] = 0;
            stash.SetOrientation(ori);
        }

        MissionObject = ItemBase.Cast(stash.GetInventory().CreateInInventory(GYconfig.stashContainerItem));
        if (!MissionObject)
        {
            Print("[GraveyardStashMission] Error: Failed to create stash container item in Underground Stash.");
            return false;
        }

        for (int w = 0; w < 10; w++)
        {
            string randomItem = GYconfig.stashLoot.GetRandomElement();
            MissionObject.GetInventory().CreateInInventory(randomItem);
            Print("[GraveyardStashMission] Added item '" + randomItem + "' to stash container.");
        }

        Print("[GraveyardStashMission] Spawn Stash Container setup completed successfully.");
        return true;
    }

    private bool SpawnInfected()
    {
        Print("[GraveyardStashMission] Spawning infected AI...");
        for (int f = 0; f < GYconfig.infectedAmount; f++)
        {
            vector InfectedPosition = GYconfig.stashCoordinates + Vector(Math.RandomFloat(-15, 15), 0, Math.RandomFloat(-15, 15));
            DayZInfected Infected = DayZInfected.Cast(GetGame().CreateObject(GYconfig.infectedTypes.GetRandomElement(), InfectedPosition, false, true));

            if (Infected)
            {
                Infected.SetPosition(InfectedPosition);
                Infected.PlaceOnSurface();
                Print("[GraveyardStashMission] Spawned infected '" + Infected.GetType() + "' at " + InfectedPosition + ".");
            }
        }

        Print("[GraveyardStashMission] Infected AI spawned successfully.");
        return true;
    }

    private void SetupMissionTrigger(vector position, float radius)
    {
        MissionTrigger = GY_CustomTrigger.Cast(GetGame().CreateObject("GY_CustomTrigger", position));
        if (MissionTrigger)
        {
            MissionTrigger.InitializeTrigger(radius, this);
            Print("[GraveyardStashMission] Trigger created at " + position + " with radius " + radius + ".");
        }
        else
        {
            Print("[GraveyardStashMission] Error: Failed to create mission trigger.");
        }
    }

    override void OnEnterMissionTrigger(PlayerBase player)
    {
        if (playersInside.Find(player) == -1)
        {
            playersInside.Insert(player);
            Print("[GraveyardStashMission] Player " + player.GetIdentity().GetName() + " entered the mission trigger.");
            NotifyPlayer(player, "Investigate the area! Look everywhere!");
        }
    }

    override void OnExitMissionTrigger(PlayerBase player)
    {
        int index = playersInside.Find(player);
        if (index != -1)
        {
            playersInside.Remove(index);
            Print("[GraveyardStashMission] Player " + player.GetIdentity().GetName() + " exited the mission trigger.");
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

class GY_CustomTrigger : Trigger
{
    private ref array<PlayerBase> m_CurrentPlayers;
    private ref array<PlayerBase> m_PreviousPlayers;
    private float m_Radius;
    private BaseMission m_Mission;

    void GY_CustomTrigger()
    {
        m_CurrentPlayers = new array<PlayerBase>();
        m_PreviousPlayers = new array<PlayerBase>();
    }

    void InitializeTrigger(float radius, BaseMission mission)
    {
        m_Radius = radius;
        m_Mission = mission;
        SetExtents(Vector(-radius, 0, -radius), Vector(radius, 1, radius));
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CheckForPlayerInside, 1000, true);
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