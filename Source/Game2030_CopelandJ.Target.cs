// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Game2030_CopelandJTarget : TargetRules
{
	public Game2030_CopelandJTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("Game2030_CopelandJ");
	}
}
