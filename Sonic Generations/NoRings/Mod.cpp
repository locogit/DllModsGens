/// <summary>
/// The main entry point for the injection.
/// </summary>
extern "C" _declspec(dllexport) void Init()
{
	ArchiveTreePatcher::m_archiveDependencies.push_back({ "RingReimagined", {"DecorationText"} });
	ArchiveTreePatcher::applyPatches();

	CSDCommon::Initialize();
	LetterboxHelper::Initialize(1280, 720);

	Update::Install();
}