/// <summary>
/// The main entry point for the injection.
/// </summary>
static const char* costumeTable[] = { "PaintSplatter", "ZigZag", "Cosmic", "TieDye", "PolkaDots", "Grid", "Tiger", "Zebra"};
int costumeIndex = 0;
extern "C" _declspec(dllexport) void Init()
{
	INIReader reader("mod.ini");
	costumeIndex = reader.GetInteger("Costume", "CurrentCostume", 0);
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency(std::string(costumeTable[costumeIndex]), { "Sonic" }));
	ArchiveTreePatcher::applyPatches();
}