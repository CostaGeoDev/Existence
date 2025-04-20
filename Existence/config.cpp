class CfgPatches
{
	class Existence
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Scripts",
			"DZ_Data"
		};
	};
};
class CfgMods
{
	class Existence
	{
		dir="Existence";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="Existence";
		credits="CostaGeo";
		author="";
		authorID="0";
		version="1.0";
		extra=0;
		type="mod";
		inputs="";
		dependencies[]=
		{
			"Game",
			"World",
			"Mission"
		};
		class defs
		{
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"Existence/Scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"Existence/Scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"Existence/Scripts/5_Mission"
				};
			};
		};
	};
};
