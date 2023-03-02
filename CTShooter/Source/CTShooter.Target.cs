// Copyright 2023, Combat Training Shooter, All rights reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class CTShooterTarget : TargetRules
{
	public CTShooterTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "CTShooter" } );
	}
}
