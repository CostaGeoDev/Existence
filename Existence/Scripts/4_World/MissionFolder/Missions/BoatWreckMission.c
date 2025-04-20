class BoatWreckMission : BaseMission
{
    private B_CustomTrigger MissionTrigger;
    private ref array<PlayerBase> playersInside;
    private float missionStartTime;
    private ref BoatWreckMissionConfig BWconfig;
    private ref MainSettingsConfig MSconfig;

    void BoatWreckMission(string name)
    {
        BaseMission(name);
    }

    override vector GetMissionTriggerCoordinates()
    {
        return GetBoatWreckMissionConfig().boatwreckCoordinates;
    }

    override void StartMission()
    {
        MSconfig = GetMainSettingsConfig();

        if (!MSconfig)
        {
            Print("[BoatWreckMission] MSconfig is NULL! Check GetMainSettingsConfig().");
            Print("[BoatWreckMission] MSconfig Object: " + MSconfig);
            Print("[BoatWreckMission] RadioFrequency: " + MSconfig.RadioFrequency);
            return;
        }

        Print("[BoatWreckMission] Initializing mission setup.");
        
        playersInside = new array<PlayerBase>();
        BWconfig = GetBoatWreckMissionConfig();
        missionStartTime = GetGame().GetTime();

        // Spawn mission-related entities
        if (!SpawnBoatWreck()) return;
        if (!SpawnContainer()) return;
        if (!SpawnInfected()) return;

        // Setup mission trigger
        SetupMissionTrigger(BWconfig.boatwreckCoordinates, BWconfig.missionOuterRadius);

        // Schedule mission timer
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CheckMissionTime, BWconfig.missionTime * 1000, false);
        Print("[BoatWreckMission] Mission successfully started.");
    }

    private void CheckMissionTime()
    {
        float elapsedTime = (GetGame().GetTime() - missionStartTime) / 1000.0; // Convert to seconds
        Print("[BoatWreckMission] Mission elapsed time: " + elapsedTime + " seconds.");

        if (elapsedTime >= BWconfig.missionTime)
        {
            EndMission();
        }
    }

    protected void EndMission()
    {
        Print("[BoatWreckMission] Mission time has expired. Ending mission.");
        if (MissionTrigger)
        {
            MissionTrigger.Delete(); // Clean up trigger
        }

        // Additional cleanup or rewards logic can be added here
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(CheckMissionTime);
    }

    private bool SpawnBoatWreck()
    {
        if (!BWconfig.boatwreckAsset || BWconfig.boatwreckCoordinates == vector.Zero)
        {
            Print("[BoatWreckMission] Error: Invalid boat wreck configuration.");
            return false;
        }

        Print("[BoatWreckMission] Spawning boat wreck at " + BWconfig.boatwreckCoordinates + "...");

        Object boatWreck = Object.Cast(GetGame().CreateObject(BWconfig.boatwreckAsset, BWconfig.boatwreckCoordinates, false, true));
        if (!boatWreck)
        {
            Print("[BoatWreckMission] Error: Failed to spawn boat wreck.");
            return false;
        }

        boatWreck.SetPosition(BWconfig.boatwreckCoordinates);
        boatWreck.SetOrientation(BWconfig.boatwreckOrientation);
        Print("[BoatWreckMission] Boat wreck spawned successfully.");
        return true;
    }

    private bool SpawnContainer()
    {
        Print("[BoatWreckMission] Spawning containers...");
        
        // Ensure the object creation calls are working correctly
        ItemBase container_1 = ItemBase.Cast(GetGame().CreateObject(BWconfig.containerItem_1, BWconfig.containerCoordinates_1, false, true));
        ItemBase container_2 = ItemBase.Cast(GetGame().CreateObject(BWconfig.containerItem_2, BWconfig.containerCoordinates_2, false, true));
        ItemBase container_3 = ItemBase.Cast(GetGame().CreateObject(BWconfig.containerItem_3, BWconfig.containerCoordinates_3, false, true));
        
        if (!container_1 || !container_2 || !container_3)
        {
            Print("[BoatWreckMission] Error: Failed to spawn containers.");
            return false;
        }

        // Add random loot from the containerItemLoot array
        for (int i = 0; i < 15; i++)
        {
            string lootItem = BWconfig.containerItemLoot.GetRandomElement();
            if (lootItem)
            {
                container_1.GetInventory().CreateInInventory(lootItem);
                container_2.GetInventory().CreateInInventory(lootItem);
                container_3.GetInventory().CreateInInventory(lootItem);
                Print("[BoatWreckMission] Added item '" + lootItem + "' to containers.");
            }
        }
        
        // Set the position of the containers
        container_1.SetPosition(BWconfig.containerCoordinates_1);
        container_2.SetPosition(BWconfig.containerCoordinates_2);
        container_3.SetPosition(BWconfig.containerCoordinates_3);
        container_1.PlaceOnSurface();
        container_2.PlaceOnSurface();
        container_3.PlaceOnSurface();

        Print("[BoatWreckMission] Containers spawned successfully.");
        return true;
    }

    // Spawn the amount of infected from the infectedAmount and infectedTypes array near the boat wreck
    private bool SpawnInfected()
    {
        Print("[BoatWreckMission] Spawning infected...");
        bool spawnSuccess = true; // Assume success initially

        for (int i = 0; i < BWconfig.infectedAmount; i++)
        {
            vector infectedPosition = BWconfig.boatwreckCoordinates + Vector(Math.RandomFloat(-20, 20), 0, Math.RandomFloat(-20, 20));
            DayZInfected infected = DayZInfected.Cast(GetGame().CreateObject(BWconfig.infectedTypes.GetRandomElement(), infectedPosition, false, true));
            
            if (infected)
            {
                infected.SetPosition(infectedPosition);
                infected.PlaceOnSurface();
                Print("[BoatWreckMission] Spawned infected '" + infected.GetType() + "' at " + infectedPosition + ".");
            }
            else
            {
                Print("[BoatWreckMission] Error: Failed to spawn infected.");
                spawnSuccess = false; // Mark as failed if any spawn fails
            }
        }

        if (spawnSuccess)
        {
            Print("[BoatWreckMission] Infected spawned successfully.");
        }
        else
        {
            Print("[BoatWreckMission] Warning: Some infected failed to spawn.");
        }

        return spawnSuccess;
    }

    private void SetupMissionTrigger(vector position, float radius)
    {
        MissionTrigger = B_CustomTrigger.Cast(GetGame().CreateObject("B_CustomTrigger", position));
        if (MissionTrigger)
        {
            MissionTrigger.InitializeTrigger(radius, this);
            Print("[BoatWreckMission] Trigger created at " + position + " with radius " + radius + ".");
        }
        else
        {
            Print("[BoatWreckMission] Error: Failed to create mission trigger.");
        }
    }

    override void OnEnterMissionTrigger(PlayerBase player)
    {
        if (playersInside.Find(player) == -1)
        {
            playersInside.Insert(player);
            Print("[BoatWreckMission] Player '" + player.GetIdentity().GetName() + "' entered mission area.");
            NotifyPlayer(player, "You're close! Be cautious of infected and other survivors!");
        }
    }

    override void OnExitMissionTrigger(PlayerBase player)
    {
        int index = playersInside.Find(player);
        if (index != -1)
        {
            playersInside.Remove(index);
            Print("[BoatWreckMission] Player '" + player.GetIdentity().GetName() + "' exited mission area.");
            NotifyPlayer(player, "You’ve left the mission area! Are you scared?");
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

class B_CustomTrigger : Trigger
{
    private ref array<PlayerBase> m_CurrentPlayers;
    private ref array<PlayerBase> m_PreviousPlayers;
    private float m_Radius;
    private BaseMission m_Mission;

    void B_CustomTrigger()
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
