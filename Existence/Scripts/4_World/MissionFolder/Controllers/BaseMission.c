class BaseMission
{
    private string missionName;

    void BaseMission(string name)
    {
        missionName = name;
    }

    string GetMissionName()
    {
        return missionName;
    }

    void StartMission()
    {
        // Placeholder
    }

    void EndMission()
    {
        // Placeholder
    }

    void OnEnterMissionTrigger(PlayerBase player)
    {
        // Placeholder for child classes
    }

    void OnExitMissionTrigger(PlayerBase player)
    {
        // Placeholder for child classes
    }

    // Virtual method to be overridden by derived classes
    vector GetMissionTriggerCoordinates()
    {
        return "0 0 0"; // Default position if not overridden
    }
}
