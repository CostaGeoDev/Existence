class BoatWreckMissionConfig
{
    // Config file location in the server profile directory
    private const static string existenceModFolder = "$profile:\\Existence\\";
    private const static string boatWreckMissionConfigName = "BoatWreckMissionConfig.json";

    // Static constant config version (used for versioning, not saved to JSON)
    static const string CONFIG_VERSION = "0.0.1";

    // Data fields stored in the configuration
    string ConfigVersion = ""; // Stores the current config version
    string BOAT_WRECK_DATA = "-----------------------------------------------------------------------------------------------------------------------"; // Divider for organization

    // Duration for the boat wreck mission (in seconds)
    int missionTime = 3600; // Default: 1 hour

    // Number of infected to spawn for the boat wreck mission
    int infectedAmount = 10;

    // Radius for detecting mission activation (outer boundary which lets players know they are close)
    float missionOuterRadius = 1000.0; // Minimum: 500 meters

    // Radius for detecting mission completion (inner boundary)
    float missionInnerRadius = 50.0; // Allowed range: 50-100 meters

    // Boat wreck asset name
    string boatwreckAsset = "Land_Boat_Small9"; // Default: "Land_Boat_Small9"

    // Coordinates of the boat wreck
    vector boatwreckCoordinates = "474.952  168.765  7430.6";
    vector boatwreckOrientation = "-174.385  -5.97536  18.9405";

    // Array defining the container item which is the mission reward
    string containerItem_1 = "WoodenCrate"; // Default: "WoodenCrate"
    string containerItem_2 = "WoodenCrate"; // Default: "WoodenCrate"
    string containerItem_3 = "WoodenCrate"; // Default: "WoodenCrate"

    // Coordinates of the mission reward container
    vector containerCoordinates_1 = "481.595  171.409  7424.91";
    vector containerCoordinates_2 = "482.726  171.784  7429.07";
    vector containerCoordinates_3 = "479.264  170.695  7424.9";

    // Container Loot Table
    autoptr array<string> containerItemLoot = {
        "WaterBottle",
        "MilitaryBelt",
        "M18SmokeGrenade_Yellow",
        "TacticalBaconCan",
        "SteakKnife",
        "SKS",
        "Ammo_762x39",
        "FNX45",
        "Mag_FNX45_15Rnd",
        "Ammo_45ACP",
        "Winchester70",
        "Ammo_308Win",
        "CZ75",
        "Mag_CZ75_15Rnd",
        "NVGHeadstrap",
        "NVGoggles",
        "Battery9V",
        "PsilocybeMushroom",
        "SpaghettiCan",
        "StoneKnife",
        "M67Grenade",
        "UMP45",
        "Mag_UMP_25Rnd",
        "TunaCan",
        "RDG5Grenade",
        "M4A1_Green",
        "Mag_STANAG_30Rnd"
    };

    // Array defining possible infected types to spawn during the mission
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
        if (FileExist(existenceModFolder + boatWreckMissionConfigName))
        { 
            // Load the existing config file
            JsonFileLoader<BoatWreckMissionConfig>.JsonLoadFile(existenceModFolder + boatWreckMissionConfigName, this);

            // If the version doesn't match, backup the old version
            if (ConfigVersion != CONFIG_VERSION)
            {
                JsonFileLoader<BoatWreckMissionConfig>.JsonSaveFile(existenceModFolder + boatWreckMissionConfigName + "_old", this);
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
        JsonFileLoader<BoatWreckMissionConfig>.JsonSaveFile(existenceModFolder + boatWreckMissionConfigName, this);
    }
}

// Reference to the global configuration object
ref BoatWreckMissionConfig m_BoatWreckMissionConfig;

// Function to access the configuration object
static BoatWreckMissionConfig GetBoatWreckMissionConfig()
{
    // Initialize the config only if it doesn't already exist and is running on a dedicated server
    if (!m_BoatWreckMissionConfig && GetGame().IsDedicatedServer())
    {
        Print("[BoatWreckMissionConfig] Initializing configuration...");
        m_BoatWreckMissionConfig = new BoatWreckMissionConfig;
        m_BoatWreckMissionConfig.Load();
    }

    return m_BoatWreckMissionConfig;
}