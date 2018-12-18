// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class FGFPS : ModuleRules
{
	public FGFPS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "AIModule", "NavigationSystem" });

        string ThirdPartyPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/"));
        string LuaLibPath = Path.Combine(ThirdPartyPath, "Lua");
        PublicAdditionalLibraries.Add(Path.Combine(LuaLibPath, "lua_Win64.lib"));
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "Lua", "include"));
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "LuaBridge"));
    }
}
