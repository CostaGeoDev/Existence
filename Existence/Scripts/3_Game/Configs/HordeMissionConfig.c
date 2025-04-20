class HordeMissionConfig
{
    // Config file location in the server profile directory
    private const static string existenceModFolder = "$profile:\\Existence\\";
    private const static string hordeMissionConfigName = "HordeMissionConfig.json";

    // Static constant config version (used for versioning, not saved to JSON)
    static const string CONFIG_VERSION = "0.0.1";

    // Data fields stored in the configuration
    string ConfigVersion = ""; // Stores the current config version
    string HORDE_MISSION_DATA = "-----------------------------------------------------------------------------------------------------------------------"; // Divider for organization

    // Duration for the horde mission (in seconds)
    int missionTime = 3600; // Default: 1 hour

    // Number of infected to spawn for the horde
    int infectedAmount = 30;    

    // Radius for detecting mission activation (outer boundary which lets players know they are close)
    float missionOuterRadius = 500.0; // Minimum: 500 meters

    // Radius for detecting mission completion (inner boundary)
    float missionInnerRadius = 50.0; // Allowed range: 50-100 meters

	// Coordinates of the mission reward container
	vector containerCoordinates = "2199.47  275.891  6759.91";

	// Array defining the container item which is the mission reward
	string containerItem = "Barrel_Yellow"; // Default: "Barrel_Yellow"

	// Container item spawn loadouts array (randomized)
	autoptr array<string> containerItemLoadouts = {
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
		"HuntingKnife",
		"BakedBeansCan",
		"Canteen",
		"SVD",
		"Mag_SVD_10Rnd",
		"Ammo_762x54",
		"CanOpener",
		"PeachesCan",
		"AKM",
		"Mag_AKM_30Rnd",
		"KitchenKnife",
		"SardinesCan",
		"FAL",
		"FieldShovel",
		"Canteen",
		"MackerelFilletMeat",
		"Battery9V"
	};

	// Array defining the horde leader type (unique infected)
	string hordeLeader = "ZmbM_CommercialPilotOld_Olive"; // Default: "ZmbM_CommercialPilotOld_Olive"

	// Array defining the horde leader backpacks
	autoptr array<string> hordeLeaderGear = {
		"AliceBag_Camo",
		"MountainBag_Blue",
		"ZSh3PilotHelmet_Green",
		"PressVest_Blue",
		"PressVest_LightBlue",
		"HighCapacityVest_Black",
		"HighCapacityVest_Olive",
		"UKAssVest_Olive"
	};

	// Horde leader loadout
	autoptr array<string> hordeLeaderLoot = {
		"M67Grenade",
		"CanOpener",
		"BandageDressing",
		"PeachesCan",
		"Canteen",
		"HuntingKnife",
		"CanOpener",
		"MackerelFilletMeat",
		"WaterBottle",
		"Battery9V"
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
		"ZmbM_HikerSkinny_Yellow",
		"ZmbM_PolicemanFat",
		"ZmbM_JoggerSkinny_Blue",
		"ZmbM_VillagerOld_White",
		"ZmbM_SkaterYoung_Brown",
		"ZmbM_MechanicSkinny_Green",
		"ZmbM_DoctorFat",
		"ZmbM_PatientSkinny",
		"ZmbM_ClerkFat_Brown",
		"ZmbM_ClerkFat_White",
		"ZmbM_Jacket_magenta"
	};

    // Loads the configuration file, or creates a new one if it doesn't exist
    void Load()
    {
        // Check if the config file exists
        if (FileExist(existenceModFolder + hordeMissionConfigName))
        { 
            // Load the existing config file
            JsonFileLoader<HordeMissionConfig>.JsonLoadFile(existenceModFolder + hordeMissionConfigName, this);

            // If the version doesn't match, backup the old version
            if (ConfigVersion != CONFIG_VERSION)
            {
                JsonFileLoader<HordeMissionConfig>.JsonSaveFile(existenceModFolder + hordeMissionConfigName + "_old", this);
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
        JsonFileLoader<HordeMissionConfig>.JsonSaveFile(existenceModFolder + hordeMissionConfigName, this);
    }
}

// Reference to the global configuration object
ref HordeMissionConfig m_HordeMissionConfig;

// Function to access the configuration object
static HordeMissionConfig GetHordeMissionConfig()
{
    // Initialize the config only if it doesn't already exist and is running on a dedicated server
    if (!m_HordeMissionConfig && GetGame().IsDedicatedServer())
    {
        Print("[HordeMissionConfig] Initializing configuration...");
        m_HordeMissionConfig = new HordeMissionConfig;
        m_HordeMissionConfig.Load();
    }

    return m_HordeMissionConfig;
}