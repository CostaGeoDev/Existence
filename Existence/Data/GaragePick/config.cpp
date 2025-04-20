class CfgPatches
{
	class GaragePick
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Weapons_Melee"
		};
	};
};
class CfgVehicles
{
	class Lockpick;
	class GaragePick: Lockpick
	{
		scope=2;
		displayName="Garage Pick";
		descriptionShort="This tool is used to pick locks on garages.";
		itemSize[]={1,3};
		hiddenSelections[]=
		{
			"zbytek"
		};
		hiddenSelectionsTextures[]=
		{
			"Existence\Data\GaragePick\garagepick_co.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=125;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\gear\tools\data\Lockpick.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\gear\tools\data\Lockpick.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\gear\tools\data\Lockpick_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\gear\tools\data\Lockpick_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\gear\tools\data\Lockpick_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
};
