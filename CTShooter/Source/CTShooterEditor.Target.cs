// Copyright 2023, Combat Training Shooter, All rights reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class CTShooterEditorTarget : TargetRules
{
	public CTShooterEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "CTShooter" } );
	}
}
