class DeadCampMissionConfig
{
    // Config file location in the server profile directory
    private const static string existenceModFolder = "$profile:\\Existence\\";
    private const static string deadCampConfigName = "DeadCampMissionConfig.json";

    // Static constant config version (used for versioning, not saved to JSON)
    static const string CONFIG_VERSION = "0.0.1";

    // Data fields stored in the configuration
    string ConfigVersion = ""; // Stores the current config version
    string DEAD_CAMP_DATA = "-----------------------------------------------------------------------------------------------------------------------"; // Divider for organization

    // Duration for the dead camp mission (in seconds)
    int missionTime = 3600; // Default: 1 hour

    // Number of infected to spawn for the garage goods mission
    int infectedAmount = 5;

    // Radius for detecting mission activation (outer boundary which lets players know they are close)
    float missionOuterRadius = 1000.0; // Minimum: 500 meters

    // Radius for detecting mission completion (inner boundary)
    float missionInnerRadius = 50.0; // Allowed range: 50-100 meters

    // Tent Type name
    string tentTypeName = "MediumTent_Green"; // Default: "MediumTent_Green"

    // Coordinates for the tent location 
    vector tentLocationCoordinates = "3216.85 409.383 1430.5"; // Default: "3216.85 409.383 1430.5"

    // Orientation for the tent location
    vector tentOrientation = "-72.9901 0 0"; // Default: "0 0 0"

    // Asset name one
    string assetTypeName_1 = "Land_Wreck_Caravan_WGreen"; // Default: "Land_Wreck_Caravan_WGreen"

    // Coordinates for asset one location
    vector assetLocationCoordinates_1 = "3219.38 410.811 1421.11"; // Default: "3219.38 410.811 1421.11"

    // Orientation for asset one location
    vector assetOrientation_1 = "172.958 0.0 -0.0"; // Default: "172.958 0.0 -0.0"

    // Asset name two
    string assetTypeName_2 = "Land_Misc_FireBarrel_Green"; // Default: "Land_Misc_FireBarrel_Green"

    // Coordinates for asset two location
    vector assetLocationCoordinates_2 = "3220.56 410.531 1428.86"; // Default: "3220.56 410.531 1428.86"

    // Orientation for asset two location
    vector assetOrientation_2 = "0.0 3.65048 2.65128"; // Default: "0.0 3.65048 2.65128"

    // Asset name three
    string assetTypeName_3 = "Land_Wreck_hb01_aban2_green_DE"; // Default: "Land_Wreck_hb01_aban2_green_DE"

    // Coordinates for asset three location
    vector assetLocationCoordinates_3 = "3220.21 409.673 1414.01"; // Default: "3220.21 409.673 1414.01"

    // Orientation for asset three location
    vector assetOrientation_3 = "0.0 1.37438 0.183192"; // Default: "0.0 1.37438 0.183192"

    // Array of possible loot items to spawn in the tent
    autoptr array<string> tentLootItems = {
        "WaterBottle",
        "MilitaryBelt",
        "M18SmokeGrenade_Yellow",
        "TacticalBaconCan",
        "SteakKnife",
        "SKS",
        "Ammo_762x39",
        "FNX45",
        "SpaghettiCan",
        "StoneKnife",
        "M67Grenade",
        "UMP45",
        "Mag_UMP_25Rnd",
        "TunaCan",
        "RDG5Grenade",
        "M4A1_Green",
        "Mag_STANAG_30Rnd",
        "Ammo_556x45",
        "Mag_STANAG_30Rnd",
        "Glock19",
        "PistolSuppressor",
        "Mag_Glock_15Rnd",
        "CombatKnife",
        "NailBox"
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
        if (FileExist(existenceModFolder + deadCampConfigName))
        {
            // Load the config file
            JsonFileLoader<DeadCampMissionConfig>.JsonLoadFile(existenceModFolder + deadCampConfigName, this);

            // If the version doesn't match, backup the old version
            if (ConfigVersion != CONFIG_VERSION)
            {
                JsonFileLoader<DeadCampMissionConfig>.JsonSaveFile(existenceModFolder + deadCampConfigName + ".old", this);
            }
            else
            {
                // If the config version matches, no further action is needed
                return;
            }       
        }

        // If the config file doesn't exist, set default values
        ConfigVersion = CONFIG_VERSION;

        // Save the default values to a new config file
        Save();
    }

    // Saves the configuration file
    void Save()
    {
        // If the folder doesn't exist, create it
        if (!FileExist(existenceModFolder))
        { 
            MakeDirectory(existenceModFolder);
        }

        // Save the configuration to a file
        JsonFileLoader<DeadCampMissionConfig>.JsonSaveFile(existenceModFolder + deadCampConfigName, this);
    }
};

// Reference to the global configuration object
ref DeadCampMissionConfig m_DeadCampMissionConfig;

// Function to load the configuration file
static DeadCampMissionConfig GetDeadCampMissionConfig()
{
    // Initialize the config only if it doesn't already exist and is running on a dedicated server
    if (!m_DeadCampMissionConfig && GetGame().IsServer())
    {
        Print("[DeadCampMissionConfig] Initializing configuration...");
        m_DeadCampMissionConfig = new ref DeadCampMissionConfig();
        m_DeadCampMissionConfig.Load();
    }

    return m_DeadCampMissionConfig;
}