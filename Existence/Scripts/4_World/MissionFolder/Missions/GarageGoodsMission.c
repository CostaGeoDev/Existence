class GarageGoodsMission : BaseMission
{
    Car MissionCar;
    ItemBase MissionBackpack;
    private G_CustomTrigger MissionTrigger;
    private ref array<PlayerBase> playersInside;
    private float missionStartTime;
    private ref GarageGoodsMissionConfig GGconfig;
    private ref MainSettingsConfig MSconfig;

    void GarageGoodsMission(string name)
    {
        BaseMission(name);
    }

    override vector GetMissionTriggerCoordinates()
    {
        // Return the specific trigger coordinates for this mission
        return GetGarageGoodsMissionConfig().vehicleLocationCoordinates;
    }

    // If the activatedMission flag is set to true, the mission will be started, if false, do nothing
    override void StartMission()
    {
        MSconfig = GetMainSettingsConfig();

        if (!MSconfig)
        {
            Print("[GarageGoodsMission] MSconfig is NULL! Check GetMainSettingsConfig().");
            Print("[GarageGoodsMission] MSconfig Object: " + MSconfig);
            Print("[GarageGoodsMission] RadioFrequency: " + MSconfig.RadioFrequency);
            return;
        }

        Print("[GarageGoodsMission] Initializing mission setup.");

        playersInside = new array<PlayerBase>();
        GGconfig = GetGarageGoodsMissionConfig();
        missionStartTime = GetGame().GetTime();

        // Spawn mission-related entities
        if (!SpawnMissionVehicle()) return;
        if (!SpawnContainer_1()) return;
        if (!SpawnContainer_2()) return;
        if (!SpawnMissionBackpack()) return;
        if (!SpawnInfected()) return;
        if (!SpawnGaragePick()) return;
        if (!LockGarageDoors()) return;

        // Setup mission trigger
        SetupMissionTrigger(GGconfig.vehicleLocationCoordinates, GGconfig.missionOuterRadius);

        // Schedule mission timer
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CheckMissionTime, GGconfig.missionTime * 1000, true);
        Print("[GarageGoodsMission] Mission successfully started.");
    }

    private void CheckMissionTime()
    {
        float elapsedTime = (GetGame().GetTime() - missionStartTime) / 1000.0; // Convert to seconds
        Print("[GarageGoodsMission] Mission elapsed time: " + elapsedTime + " seconds.");

        if (elapsedTime >= GGconfig.missionTime)
        {
            EndMission();
        }
    }

    protected void EndMission()
    {
        Print("[GarageGoodsMission] Mission time has expired. Ending mission.");
        if (MissionTrigger)
        {
            MissionTrigger.Delete(); // Clean up trigger
        }

        // Additional cleanup or rewards logic can be added here
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(CheckMissionTime);
    }

    private bool LockGarageDoors()
    {
        // Declare local variables for object lists
        ref array<Object> m_ObjectList = new array<Object>();
        ref array<CargoBase> m_ObjectCargoList = new array<CargoBase>();

        Print("[GarageGoodsMission] Debug: Checking objects at coordinates: " + GGconfig.vehicleLocationCoordinates);

        GetGame().GetObjectsAtPosition(GGconfig.vehicleLocationCoordinates, 10.0, m_ObjectList, m_ObjectCargoList);

        for (int i = 0; i < m_ObjectList.Count(); i++)
        {
            Object FoundObject = m_ObjectList.Get(i);
            Print("[GarageGoodsMission] Debug: Found object type: " + FoundObject.GetType());

            if (FoundObject.GetType() == GGconfig.garageTypeName)
            {
                Print("[GarageGoodsMission] Debug: Garage building found!");

                Building Garage = Building.Cast(FoundObject);
                if (!Garage)
                {
                    Print("[GarageGoodsMission] Error: Failed to cast object to Building.");
                    return false;
                }

                int doorCount = Garage.GetDoorCount(); // Safely get the total number of doors
                Print("[GarageGoodsMission] Debug: Garage has " + doorCount + " doors.");

                for (int j = 0; j < doorCount; j++)
                {
                    if (Garage.IsDoorOpen(j))
                    {
                        Garage.CloseDoor(j);
                        Print("[GarageGoodsMission] Debug: Closed door " + j);
                    }
                    if (!Garage.IsDoorLocked(j))
                    {
                        Garage.LockDoor(j);
                        Print("[GarageGoodsMission] Debug: Locked door " + j);
                    }
                }

                // Safely update the path graph if all operations succeeded
                GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 1000, false, Garage);
                Print("[GarageGoodsMission] Debug: Pathgraph updated for garage.");
                return true;
            }
        }

        Print("[GarageGoodsMission] Error: Garage building not found.");
        return false;
    }

    
    private bool SpawnMissionVehicle()
    {
        if (!GGconfig.vehicleTypeName || GGconfig.vehicleLocationCoordinates == vector.Zero)
        {
            Print("[GarageGoodsMission] Error: Invalid vehicle configuration.");
            return false;
        }

        Print("[GarageGoodsMission] Spawning mission vehicle at " + GGconfig.vehicleLocationCoordinates + "...");

        MissionCar = Car.Cast(GetGame().CreateObject(GGconfig.vehicleTypeName, GGconfig.vehicleLocationCoordinates));
        MissionCar.SetOrientation(GGconfig.vehicleLocationOrientation);
        // Place the vehicle on the surface
        MissionCar.PlaceOnSurface();

        // Attach predefined vehicle attachments
        array<string> vehicleAttachments = {
            GGconfig.vehicleAttachment_1, GGconfig.vehicleAttachment_2, GGconfig.vehicleAttachment_3,
            GGconfig.vehicleAttachment_4, GGconfig.vehicleAttachment_5, GGconfig.vehicleAttachment_6,
            GGconfig.vehicleAttachment_7, GGconfig.vehicleAttachment_8, GGconfig.vehicleAttachment_9,
            GGconfig.vehicleAttachment_10, GGconfig.vehicleAttachment_11, GGconfig.vehicleAttachment_12,
            GGconfig.vehicleAttachment_13, GGconfig.vehicleAttachment_14
        };

        foreach (string attachment : vehicleAttachments)
        {
            if (attachment && attachment != "")  // Ensure valid attachment name
            {
                MissionCar.GetInventory().CreateAttachment(attachment);
            }
        }

        // Add 20 random loot items to the vehicle's inventory
        for (int i = 0; i < 20; i++)
        {
            string randomItem = GGconfig.vehicleInventoryItems.GetRandomElement();
            MissionCar.GetInventory().CreateInInventory(randomItem);
            Print("[GarageGoodsMission] Added item '" + randomItem + "' to vehicle inventory.");
        }

        // Set fluids and lifetime
        MissionCar.Fill(CarFluid.COOLANT, 20.0);
        MissionCar.Fill(CarFluid.FUEL, 20.0);
        MissionCar.SetLifetime(GGconfig.missionTime);

        return true;
    }

    private bool SpawnContainer_1()
    {
        Print("[GarageGoodsMission] Spawning mission containers...");

        // Spawn the first container at the specified coordinates
        ItemBase Container_1 = ItemBase.Cast(GetGame().CreateObject(GGconfig.containerItem_1, GGconfig.containerCoordinates_1, false, true));

        if (!Container_1)
        {
            Print("[GarageGoodsMission] Error: Failed to spawn container 1.");
            return false;
        }

        Container_1.SetPosition(GGconfig.containerCoordinates_1);
        
        // Add 15 random loot items to the container's inventory
        for (int i = 0; i < 15; i++)
        {
            string randomItem = GGconfig.containerItemLoot.GetRandomElement();
            Container_1.GetInventory().CreateInInventory(randomItem);
            Print("[GarageGoodsMission] Added item '" + randomItem + "' to container 1.");
        }

        Print("[GarageGoodsMission] Container 1 setup completed successfully.");
        return true;
    }

    private bool SpawnContainer_2()
    {
        // Spawn the second container at the specified coordinates
        ItemBase Container_2 = ItemBase.Cast(GetGame().CreateObject(GGconfig.containerItem_2, GGconfig.containerCoordinates_2, false, true));

        if (!Container_2)
        {
            Print("[GarageGoodsMission] Error: Failed to spawn container 2.");
            return false;
        }

        Container_2.SetPosition(GGconfig.containerCoordinates_2);

        // Add 15 random loot items to the container's inventory
        for (int i = 0; i < 15; i++)
        {
            string randomItem = GGconfig.containerItemLoot.GetRandomElement();
            Container_2.GetInventory().CreateInInventory(randomItem);
            Print("[GarageGoodsMission] Added item '" + randomItem + "' to container 2.");
        }

        Print("[GarageGoodsMission] Container 2 setup completed successfully.");
        return true;
    }

    // Spawn the mission backpack at the specified coordinates and orientation
    private bool SpawnMissionBackpack()
    {
        Print("[GarageGoodsMission] Spawning mission backpack...");

        MissionBackpack = ItemBase.Cast(GetGame().CreateObject(GGconfig.backpackTypeName, GGconfig.backpackCoordinates, false, true));
        if (!MissionBackpack)
        {
            Print("[GarageGoodsMission] Error: Failed to spawn mission backpack.");
            return false;
        }

        MissionBackpack.SetPosition(GGconfig.backpackCoordinates);
        MissionBackpack.SetOrientation(GGconfig.backpackOrientation);

        // Add 7 random loot from the backpackLootItems array
        for (int i = 0; i < 15; i++)
        {
            MissionBackpack.GetInventory().CreateInInventory(GGconfig.backpackLootItems.GetRandomElement());
        }

        Print("[GarageGoodsMission] Mission backpack spawned successfully.");

        return true;
    }

    // Spawn infected AI at the specified coordinates
    private bool SpawnInfected()
    {
        Print("[GarageGoodsMission] Spawning infected AI...");
        for (int i = 0; i < GGconfig.infectedAmount; i++)
        {
            vector InfectedPosition = GGconfig.vehicleLocationCoordinates + Vector(Math.RandomFloat(-15, 15), 0, Math.RandomFloat(-15, 15));
            DayZInfected Infected = DayZInfected.Cast(GetGame().CreateObject(GGconfig.infectedTypes.GetRandomElement(), InfectedPosition, false, true));

            if (Infected)
            {
                Infected.SetPosition(InfectedPosition);
                Infected.PlaceOnSurface();
                Print("[GarageGoodsMission] Spawned infected '" + Infected.GetType() + "' at " + InfectedPosition + ".");
            }
        }

        Print("[GarageGoodsMission] Infected AI spawned successfully.");
        return true;
    }

    private void SetupMissionTrigger(vector position, float radius)
    {
        MissionTrigger = G_CustomTrigger.Cast(GetGame().CreateObject("G_CustomTrigger", position));
        if (MissionTrigger)
        {
            MissionTrigger.InitializeTrigger(radius, this);
            Print("[GarageGoodsMission] Trigger created at " + position + " with radius " + radius + ".");
        }
        else
        {
            Print("[GarageGoodsMission] Error: Failed to create mission trigger.");
        }
    }

    private bool SpawnGaragePick()
    {
        Print("[GarageGoodsMission] Spawning GaragePick item...");

        vector GaragePickPosition = GGconfig.vehicleLocationCoordinates + Vector(Math.RandomFloat(-20, 20), 0, Math.RandomFloat(-20, 20));
        ItemBase garagePickItem = ItemBase.Cast(GetGame().CreateObject("GaragePick", GaragePickPosition, false, true));

        if (!garagePickItem)
        {
            Print("[GarageGoodsMission] Error: Failed to spawn GaragePick item.");
            return false;
        }

        garagePickItem.SetPosition(GaragePickPosition);
        garagePickItem.PlaceOnSurface();
        Print("[GarageGoodsMission] GaragePick item spawned successfully at " + GaragePickPosition + ".");
        return true;
    }

    override void OnEnterMissionTrigger(PlayerBase player)
    {
        if (playersInside.Find(player) == -1)
        {
            playersInside.Insert(player);
            Print("[GarageGoodsMission] Player " + player.GetIdentity().GetName() + " entered the mission trigger.");
            NotifyPlayer(player, "Investigate the area! Maybe there is a Lock Pick around to open the Garage?");
        }
    }

    override void OnExitMissionTrigger(PlayerBase player)
    {
        int index = playersInside.Find(player);
        if (index != -1)
        {
            playersInside.Remove(index);
            Print("[GarageGoodsMission] Player " + player.GetIdentity().GetName() + " exited the mission trigger.");
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

class G_CustomTrigger : Trigger
{
    private ref array<PlayerBase> m_CurrentPlayers;
    private ref array<PlayerBase> m_PreviousPlayers;
    private float m_Radius;
    private BaseMission m_Mission;

    void G_CustomTrigger()
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
