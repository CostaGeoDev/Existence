class GraveyardStashMissionConfig
{
    // Config file location in the server profile directory
    private const static string existenceModFolder = "$profile:\\Existence\\";
    private const static string graveyardstashMissionConfigName = "GraveyardStashMissionConfig.json";

    // Static constant config version (used for versioning, not saved to JSON)
    static const string CONFIG_VERSION = "0.0.1";

    // Data fields stored in the configuration
    string ConfigVersion = ""; // Stores the current config version
    string HORDE_MISSION_DATA = "-----------------------------------------------------------------------------------------------------------------------"; // Divider for organization

    // Duration for the horde mission (in seconds)
    int missionTime = 3600; // Default: 1 hour

    // Number of infected to spawn for the horde
    int infectedAmount = 10;    

    // Radius for detecting mission activation (outer boundary which lets players know they are close)
    float missionOuterRadius = 500.0; // Minimum: 500 meters

    // Radius for detecting mission completion (inner boundary)
    float missionInnerRadius = 50.0; // Allowed range: 50-100 meters

    // Stach Container Item
    string stashContainerItem = "SeaChest";

    vector stashCoordinates = "11129.030273  346.018005  4336.113281";

    autoptr array<string> stashLoot = {
        "StoneKnife",
        "Battery9V",
        "SteakKnife",
        "WaterBottle",
        "TunaCan",
        "FNX45",
        "Mag_FNX45_15Rnd",
        "Ammo_45ACP",
        "Canteen",
        "PortableGasStove",
        "SmallGasCanister",
        "Matchbox"
    };

    autoptr array<string> infectedTypes = {
        "ZmbM_CitizenASkinny_Brown",
        "ZmbM_priestPopSkinny",
        "ZmbM_HermitSkinny_Beige",
        "ZmbF_JoggerSkinny_Red",
        "ZmbF_BlueCollarFat_Green",
        "ZmbM_PatrolNormal_Summer",
        "ZmbM_CitizenBFat_Blue",
        "ZmbF_HikerSkinny_Grey",
        "ZmbF_JournalistNormal_White",
        "ZmbF_SkaterYoung_Striped",
        "ZmbM_Jacket_black",
        "ZmbM_PolicemanSpecForce",
        "ZmbM_Jacket_stripes",
        "ZmbM_HikerSkinny_Blue",
        "ZmbM_HikerSkinny_Yellow"
    };

    // Loads the configuration file, or creates a new one if it doesn't exist
    void Load()
    {
        // Check if the config file exists
        if (FileExist(existenceModFolder + graveyardstashMissionConfigName))
        { 
            // Load the existing config file
            JsonFileLoader<GraveyardStashMissionConfig>.JsonLoadFile(existenceModFolder + graveyardstashMissionConfigName, this);

            // If the version doesn't match, backup the old version
            if (ConfigVersion != CONFIG_VERSION)
            {
                JsonFileLoader<GraveyardStashMissionConfig>.JsonSaveFile(existenceModFolder + graveyardstashMissionConfigName + "_old", this);
            }
            else
            {
                // If the config version matches, no further action is needed
                return;
            }
        }

        // If the config file doesn't exist, set default values
        ConfigVersion = CONFIG_VERSION;

        // Save the default config
        Save();
    }

    // Saves the configuration to a file
    void Save()
    {
        // If the folder doesn't exist, create it
        if (!FileExist(existenceModFolder))
        { 
            MakeDirectory(existenceModFolder);
        }

        // Save the configuration in JSON format
        JsonFileLoader<GraveyardStashMissionConfig>.JsonSaveFile(existenceModFolder + graveyardstashMissionConfigName, this);
    }
}

// Reference to the global configuration object
ref GraveyardStashMissionConfig m_GraveyardStashMissionConfig;

// Function to access the configuration object
static GraveyardStashMissionConfig GetGraveyardStashMissionConfig()
{
    // Initialize the config only if it doesn't already exist and is running on a dedicated server
    if (!m_GraveyardStashMissionConfig && GetGame().IsDedicatedServer())
    {
        Print("[GraveyardStashMissionConfig] Initializing configuration...");
        m_GraveyardStashMissionConfig = new GraveyardStashMissionConfig;
        m_GraveyardStashMissionConfig.Load();
    }

    return m_GraveyardStashMissionConfig;
}