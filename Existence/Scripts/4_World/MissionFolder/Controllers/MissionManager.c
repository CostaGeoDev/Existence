class MissionManager
{
    private ref array<ref BaseMission> missions;

    void MissionManager()
    {
        missions = new array<ref BaseMission>();

        if (GetMainSettingsConfig().missionsActive)
        {
            missions.Insert(new HordeMission("Horde Mission"));
            missions.Insert(new BoatWreckMission("Boat Wreck Mission"));
            missions.Insert(new GarageGoodsMission("Garage Goods Mission"));
            missions.Insert(new DeadCampMission("Dead Camp Mission"));
            missions.Insert(new GraveyardStashMission("Graveyard Stash Mission"));
            // Add other missions here as needed
        }
        else
        {
            Print("[MissionManager] Missions are disabled via config.");
        }
    }

    void StartRandomMission()
    {
        if (missions.Count() > 0)
        {
            int randomIndex = Math.RandomInt(0, missions.Count());
            BaseMission selectedMission = missions[randomIndex];

            if (selectedMission)
            {
                Print("[MissionManager] Starting mission: " + selectedMission.GetMissionName());
                selectedMission.StartMission();

                // Retrieve the mission's trigger coordinates
                vector position = selectedMission.GetMissionTriggerCoordinates();

                // Convert vector to readable format
                string positionString = "(" + position[0].ToString() + ", " + position[1].ToString() + ", " + position[2].ToString() + ")";
                GetGame().ChatPlayer(string.Format("A new mission has started! If you dare, head to these coordinates: %1", positionString));
            }
        }
        else
        {
            Print("[MissionManager] No missions available to start.");
        }
    }
}