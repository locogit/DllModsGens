/// <summary>
/// The main entry point for the injection.
/// </summary>

extern "C" __declspec(dllexport) void PostInit()
{
	if (Common::reader.GetBoolean("Main", "LocoEye", true)) {
		ArchiveTreePatcher::m_archiveDependencies.push_back({ "LocoEye", {"Sonic", "SonicSp"} });
	}
	ArchiveTreePatcher::applyPatches();
	AnimationSetPatcher::applyPatches();
	Update::Install();
}