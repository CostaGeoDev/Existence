class HordeMission : BaseMission
{
    private H_CustomTrigger MissionTrigger;
    private ref array<PlayerBase> playersInside;
    private float missionStartTime;
    private ref HordeMissionConfig HMconfig;
    private ref MainSettingsConfig MSconfig;

    void HordeMission(string name)
    {
        BaseMission(name);
    }

    override vector GetMissionTriggerCoordinates()
    {
        return GetHordeMissionConfig().containerCoordinates;
    }

    override void StartMission()
    {
        MSconfig = GetMainSettingsConfig();

        if (!MSconfig)
        {
            Print("[HordeMission] MSconfig is NULL! Check GetMainSettingsConfig().");
            Print("[HordeMission] MSconfig Object: " + MSconfig);
            Print("[HordeMission] RadioFrequency: " + MSconfig.RadioFrequency);
            return;
        }

        Print("[HordeMission] Initializing mission setup.");
        
        playersInside = new array<PlayerBase>();
        HMconfig = GetHordeMissionConfig();
        missionStartTime = GetGame().GetTime();

        // Spawn mission-related entities
        if (!SpawnContainer()) return;
        if (!SpawnHordeLeader()) return;
        SpawnHordeInfected();

        // Setup mission trigger
        SetupMissionTrigger(HMconfig.containerCoordinates, HMconfig.missionOuterRadius);

        // Schedule mission timer
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CheckMissionTime, HMconfig.missionTime * 1000, false);
        Print("[HordeMission] Mission successfully started.");
    }

    private void CheckMissionTime()
    {
        float elapsedTime = (GetGame().GetTime() - missionStartTime) / 1000.0; // Convert to seconds
        Print("[HordeMission] Mission elapsed time: " + elapsedTime + " seconds.");

        if (elapsedTime >= HMconfig.missionTime)
        {
            EndMission();
        }
    }

    protected void EndMission()
    {
        Print("[HordeMission] Mission time has expired. Ending mission.");
        if (MissionTrigger)
        {
            MissionTrigger.Delete(); // Clean up trigger
        }

        // Additional cleanup or rewards logic can be added here
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(CheckMissionTime);
    }

    private bool SpawnContainer()
    {
        Print("[HordeMission] Spawning container...");
        
        // Extract the x and y coordinates from HMconfig.containerCoordinates
        // vector containerPos = Vector(HMconfig.containerCoordinates[0], 0, HMconfig.containerCoordinates[2]);

        // Create the container object at the specified position (only x and y, z will be calculated by PlaceOnSurface)
        ItemBase MissionObject = ItemBase.Cast(GetGame().CreateObject(HMconfig.containerItem, HMconfig.containerCoordinates, false, true));
        
        if (!MissionObject)
        {
            Print("[HordeMission] Error: Failed to spawn container.");
            return false;
        }

        // Set the position (z-coordinate will be adjusted by PlaceOnSurface)
        MissionObject.SetPosition(HMconfig.containerCoordinates);
        //MissionObject.PlaceOnSurface();

        // Add random items to the container
        for (int i = 0; i < 15; i++)
        {
            string randomItem = HMconfig.containerItemLoadouts.GetRandomElement();
            MissionObject.GetInventory().CreateInInventory(randomItem);
            Print("[HordeMission] Added item '" + randomItem + "' to container.");
        }

        Print("[HordeMission] Container setup completed.");
        return true;
    }

    private bool SpawnHordeLeader()
    {
        Print("[HordeMission] Spawning horde leader...");

        // Validate HMconfig
        if (!HMconfig)
        {
            Print("[HordeMission] Error: Config is NULL. Cannot spawn horde leader.");
            return false;
        }

        // Validate leader gear and loot
        if (HMconfig.hordeLeaderGear.Count() == 0 || HMconfig.hordeLeaderLoot.Count() == 0)
        {
            Print("[HordeMission] Error: Horde leader gear or loot is not defined in the config.");
            return false;
        }

        // Validate container coordinates
        if (HMconfig.containerCoordinates == vector.Zero)
        {
            Print("[HordeMission] Error: Container coordinates are not set.");
            return false;
        }

        // Determine position near container
        vector leaderPos = HMconfig.containerCoordinates + Vector(Math.RandomFloat(-10, 10), 0, Math.RandomFloat(-10, 10));

        // Spawn the leader object
        DayZInfected leader = DayZInfected.Cast(GetGame().CreateObject(HMconfig.hordeLeader, leaderPos, false, true));
        if (!leader)
        {
            Print("[HordeMission] Error: Failed to create horde leader at position: " + leaderPos.ToString());
            return false;
        }

        // Set position and align to surface
        leader.SetPosition(leaderPos);
        leader.PlaceOnSurface();

        // Attach gear to the leader
        foreach (string gear : HMconfig.hordeLeaderGear)
        {
            EntityAI attachment = leader.GetInventory().CreateAttachment(gear);
            if (!attachment)
            {
                Print("[HordeMission] Warning: Failed to attach gear '" + gear + "' to horde leader.");
            }
            else
            {
                // Check if the gear has cargo and add random items
                if (attachment.GetInventory() && HasAnyCargo(attachment))
                {
                    for (int i = 0; i < 5; i++)
                    {
                        string randomItem = HMconfig.containerItemLoadouts.GetRandomElement();
                        if (randomItem)
                        {
                            attachment.GetInventory().CreateInInventory(randomItem);
                            Print("[HordeMission] Added item '" + randomItem + "' to leader's gear.");
                        }
                    }
                }
            }
        }

        // Add loot to the leader's inventory
        for (int j = 0; j < Math.RandomInt(2, 3); j++)
        {
            string lootItem = HMconfig.hordeLeaderLoot.GetRandomElement();
            if (lootItem)
            {
                leader.GetInventory().CreateInInventory(lootItem);
                Print("[HordeMission] Added item '" + lootItem + "' to leader's inventory.");
            }
        }

        Print("[HordeMission] Horde leader setup completed successfully.");
        return true;
    }

    //! Checks if the given entity has any cargo
    private bool HasAnyCargo(EntityAI entity)
    {
        CargoBase cargo = entity.GetInventory().GetCargo();
        if (!cargo) return false; // This entity does not have cargo

        return cargo.GetItemCount() > 0;
    }

    private void SpawnHordeInfected()
    {
        Print("[HordeMission] Spawning infected horde...");
        for (int k = 0; k < HMconfig.infectedAmount; k++)
        {
            vector InfectedPosition = HMconfig.containerCoordinates + Vector(Math.RandomFloat(-20, 20), 0, Math.RandomFloat(-20, 20));
            DayZInfected Infected = DayZInfected.Cast(GetGame().CreateObject(HMconfig.infectedTypes.GetRandomElement(), InfectedPosition, false, true));
            
            if (Infected)
            {
                Infected.SetPosition(InfectedPosition);
                Infected.PlaceOnSurface();
                Print("[HordeMission] Spawned infected '" + Infected.GetType() + "' at " + InfectedPosition + ".");
            }
        }
        Print("[HordeMission] Horde infected setup completed.");
    }

    private void SetupMissionTrigger(vector position, float radius)
    {
        MissionTrigger = H_CustomTrigger.Cast(GetGame().CreateObject("H_CustomTrigger", position));
        if (MissionTrigger)
        {
            MissionTrigger.InitializeTrigger(radius, this);
            Print("[HordeMission] Trigger created at " + position + " with radius " + radius + ".");
        }
        else
        {
            Print("[HordeMission] Error: Failed to create mission trigger.");
        }
    }

    override void OnEnterMissionTrigger(PlayerBase player)
    {
        if (playersInside.Find(player) == -1)
        {
            playersInside.Insert(player);
            Print("[HordeMission] Player '" + player.GetIdentity().GetName() + "' entered mission area.");
            NotifyPlayer(player, "You're close! Be cautious of infected and other survivors!");
        }
    }

    override void OnExitMissionTrigger(PlayerBase player)
    {
        int index = playersInside.Find(player);
        if (index != -1)
        {
            playersInside.Remove(index);
            Print("[HordeMission] Player '" + player.GetIdentity().GetName() + "' exited mission area.");
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

class H_CustomTrigger : Trigger
{
    private ref array<PlayerBase> m_CurrentPlayers;
    private ref array<PlayerBase> m_PreviousPlayers;
    private float m_Radius;
    private BaseMission m_Mission;

    void H_CustomTrigger()
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