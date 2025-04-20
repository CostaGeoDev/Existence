class MainSettingsConfig
{
    // Config file location in the server profile directory
    private const static string existenceModFolder = "$profile:\\Existence\\";
	private const static string mainsettingsConfigName = "MainSettingsConfig.json";

    // Static constant config version (used for versioning, not saved to JSON)
	static const string CONFIG_VERSION = "0.0.1";

    // Data fields stored in the configuration
	string ConfigVersion = ""; // Stores the current config version
    string CONFIG_DATA = "-----------------------------------------------------------------------------------------------------------------------"; // Divider for organization

    // Determines if missions should start automatically or remain inactive
    bool missionsActive = true;

    // Time (in seconds) after the server starts before missions are activated
    int initiateTimer = 600; // Default: 10 minutes (600 seconds)

    // Time (in seconds) between mission starts
    int missionInterval = 3600; // Default: 1 hour

	int despawnTimer = 3600; // Default: 30 minutes

    // Missions Radio Frequency
    // Can only be one of the following (87.8, 89.5, 91.3, 91.9, 94.6, 96.6, 99.7, 102.5)
    autoptr array<string> RadioFrequency = {
		"87.8"
    };

    // Loads the configuration file, or creates a new one if it doesn't exist
	void Load()
    {
		// Check if the config file exists
		if (FileExist(existenceModFolder + mainsettingsConfigName))
		{ 
			// Load the existing config file
			JsonFileLoader<MainSettingsConfig>.JsonLoadFile(existenceModFolder + mainsettingsConfigName, this);

			// If the version doesn't match, backup the old version
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<MainSettingsConfig>.JsonSaveFile(existenceModFolder + mainsettingsConfigName + "_old", this);
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
		JsonFileLoader<MainSettingsConfig>.JsonSaveFile(existenceModFolder + mainsettingsConfigName, this);
	}
}

// Reference to the global configuration object
ref MainSettingsConfig m_MainSettingsConfig;

// Function to access the configuration object
static MainSettingsConfig GetMainSettingsConfig()
{
	// Initialize the config only if it doesn't already exist and is running on a dedicated server
	if (!m_MainSettingsConfig && GetGame().IsDedicatedServer())
	{
		Print("[MainSettingsConfig] Initializing configuration...");
		m_MainSettingsConfig = new MainSettingsConfig;
		m_MainSettingsConfig.Load();
	}

	return m_MainSettingsConfig;
};