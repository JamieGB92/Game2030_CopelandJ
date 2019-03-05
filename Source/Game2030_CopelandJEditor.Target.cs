// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Game2030_CopelandJEditorTarget : TargetRules
{
	public Game2030_CopelandJEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("Game2030_CopelandJ");
	}
}
