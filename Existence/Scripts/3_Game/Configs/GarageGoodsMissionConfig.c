class GarageGoodsMissionConfig
{
    // Config file location in the server profile directory
    private const static string existenceModFolder = "$profile:\\Existence\\";
    private const static string garageGoodsConfigName = "GarageGoodsMissionConfig.json";

    // Static constant config version (used for versioning, not saved to JSON)
    static const string CONFIG_VERSION = "0.0.1";

    // Data fields stored in the configuration
    string ConfigVersion = ""; // Stores the current config version
    string GARAGE_GOODS_DATA = "-----------------------------------------------------------------------------------------------------------------------"; // Divider for organization

    // Duration for the garage goods mission (in seconds)
    int missionTime = 3600; // Default: 1 hour

    // Number of infected to spawn for the garage goods mission
    int infectedAmount = 10;

    // Radius for detecting mission activation (outer boundary which lets players know they are close)
    float missionOuterRadius = 1000.0; // Minimum: 500 meters

    // Radius for detecting mission completion (inner boundary)
    float missionInnerRadius = 50.0; // Allowed range: 50-100 meters

    string garageTypeName = "Land_Shed_Closed"; // Default: "Land_Shed_Closed"

    // Vehicle Type name
    string vehicleTypeName = "OffroadHatchback"; // Default: "OffroadHatchback"

    // Vehicle attachment items
    string vehicleAttachment_1 = "HatchbackWheel";
    string vehicleAttachment_2 = "HatchbackWheel";
    string vehicleAttachment_3 = "HatchbackWheel";
    string vehicleAttachment_4 = "HatchbackWheel";
    string vehicleAttachment_5 = "HatchbackWheel";
    string vehicleAttachment_6 = "HatchbackTrunk";
    string vehicleAttachment_7 = "HatchbackHood";
    string vehicleAttachment_8 = "HatchbackDoors_Driver";
    string vehicleAttachment_9 = "HatchbackDoors_CoDriver_GreenRust";
    string vehicleAttachment_10 = "CarRadiator";
    string vehicleAttachment_11 = "CarBattery";
    string vehicleAttachment_12 = "HeadlightH7";
    string vehicleAttachment_13 = "HeadlightH7";
    string vehicleAttachment_14 = "SparkPlug";

    // Vehicle inventory items
    autoptr array<string> vehicleInventoryItems = {
        "CarRadiator",
        "CarBattery",
        "HeadlightH7",
        "HeadlightH7",
        "StoneKnife",
        "M67Grenade",
        "UMP45",
        "Mag_UMP_25Rnd",
        "TunaCan",
        "RDG5Grenade",
        "M4A1_Green",
        "Mag_STANAG_30Rnd",
        "Ammo_556x45",
        "Mag_STANAG_30Rnd"
    };

    // Coordinates of the mission vehicle location
    vector vehicleLocationCoordinates = "4917.63  352.663  5401.73"; // Must be inside of a garage (placeholders for now)
    
    // Orientation of the mission vehicle
    vector vehicleLocationOrientation = "-133.675  0.419761  0.308304"; // Must be inside of a garage (placeholders for now)

    // Coordinates of the mission containers
    string containerItem_1 = "WoodenCrate"; // Default: "WoodenCrate"
    vector containerCoordinates_1 = "4928.02  356.024  5414.21"; // Must be inside of a garage (placeholders for now)

    string containerItem_2 = "Barrel_Yellow"; // Default: "Barrel_Yellow"
    vector containerCoordinates_2 = "4927.46  356.018  5413.54"; // Must be inside of a garage (placeholders for now)

    // Array of possible loot items to spawn in the containers
    autoptr array<string> containerItemLoot = {
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

    // Backpack Type name
    string backpackTypeName = "MountainBag_Green";

    // BackPack location
    vector backpackCoordinates = "4911.33  352.724  5400.25";
    vector backpackOrientation = "-159.732  -87.4906  -64.0278";

    // Container Loot Table
    autoptr array<string> backpackLootItems = {
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
        if (FileExist(existenceModFolder + garageGoodsConfigName))
        {
            // Load the config file
            JsonFileLoader<GarageGoodsMissionConfig>.JsonLoadFile(existenceModFolder + garageGoodsConfigName, this);

            // If the version doesn't match, backup the old version
            if (ConfigVersion != CONFIG_VERSION)
            {
                JsonFileLoader<GarageGoodsMissionConfig>.JsonSaveFile(existenceModFolder + garageGoodsConfigName + ".old", this);
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
        JsonFileLoader<GarageGoodsMissionConfig>.JsonSaveFile(existenceModFolder + garageGoodsConfigName, this);
    }
};

// Reference to the global configuration object
ref GarageGoodsMissionConfig m_GarageGoodsMissionConfig;

// Function to load the configuration file
static GarageGoodsMissionConfig GetGarageGoodsMissionConfig()
{
    // Initialize the config only if it doesn't already exist and is running on a dedicated server
    if (!m_GarageGoodsMissionConfig && GetGame().IsServer())
    {
        Print("[GarageGoodsMissionConfig] Initializing configuration...");
        m_GarageGoodsMissionConfig = new ref GarageGoodsMissionConfig();
        m_GarageGoodsMissionConfig.Load();
    }

    return m_GarageGoodsMissionConfig;
}