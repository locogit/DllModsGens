/// <summary>
/// The main entry point for the injection.
/// </summary>
static const char* costumeTable[] = { "Rad", "Zagged", "Cosmic", "TieDye", "Dotted", "Grid", "Tiger", "Zebra","Cheetah", "Gold", "Silver","Burst"};
int shoeIndex = -1;
int gloveIndex = -1;
extern "C" _declspec(dllexport) void Init()
{
	INIReader reader("mod.ini");
	shoeIndex = reader.GetInteger("Shoes", "CurrentShoes", -1);
	gloveIndex = reader.GetInteger("Gloves", "CurrentGloves", -1);
	if (shoeIndex != -1) { ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency(std::string(costumeTable[shoeIndex]) + "Shoes", {"Sonic"})); }
	if (gloveIndex != -1) { ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency(std::string(costumeTable[gloveIndex]) + "Gloves", {"Sonic"})); }
	ArchiveTreePatcher::applyPatches();
}