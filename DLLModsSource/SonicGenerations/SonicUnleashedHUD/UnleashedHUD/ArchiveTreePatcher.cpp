std::vector<ArchiveDependency> ArchiveTreePatcher::archiveDependencies = {};

HOOK(bool, __stdcall, ParseArchiveTree, 0xD4C8E0, void* a1, char* pData, const size_t size, void* pDatabase)
{
	std::string str;
	{
		std::stringstream stream;

		for (auto& node : ArchiveTreePatcher::archiveDependencies)
		{
			stream << "  <Node>\n";
			stream << "    <Name>" << node.archive << "</Name>\n";
			stream << "    <Archive>" << node.archive << "</Archive>\n";
			stream << "    <Order>" << 0 << "</Order>\n";
			stream << "    <DefAppend>" << node.archive << "</DefAppend>\n";

			for (auto& archive : node.dependencies)
			{
				stream << "    <Node>\n";
				stream << "      <Name>" << archive << "</Name>\n";
				stream << "      <Archive>" << archive << "</Archive>\n";
				stream << "      <Order>" << 0 << "</Order>\n";
				stream << "    </Node>\n";
			}

			stream << "  </Node>\n";
		}

		str = stream.str();
	}

	const size_t newSize = size + str.size();
	const std::unique_ptr<char[]> pBuffer = std::make_unique<char[]>(newSize);
	memcpy(pBuffer.get(), pData, size);

	char* pInsertionPos = strstr(pBuffer.get(), "<Include>");

	memmove(pInsertionPos + str.size(), pInsertionPos, size - (size_t)(pInsertionPos - pBuffer.get()));
	memcpy(pInsertionPos, str.c_str(), str.size());

	bool result;
	{
		result = originalParseArchiveTree(a1, pBuffer.get(), newSize, pDatabase);
	}

	return result;
}

void ArchiveTreePatcher::Install()
{
	//std::vector<std::string> dependencies = { "SonicActionCommon", "SonicActionCommonHud", "SystemCommon", "cmn200", "Guide", "pam_cmn", "StageGate", "Title", "NoticeBoard", "pam000" };
    switch (Configuration::buttonType) 
    {
		case Configuration::ButtonType::XSX:
			archiveDependencies.push_back(ArchiveDependency("XboxSeriesButtons", { "SystemCommon" }));
			WRITE_STRING(0x1688344, "ui_howxb");
			WRITE_STRING(0x16886A8, "ui_howxb");
			WRITE_STRING(0x1692BC4, "ui_howxb");
			break;

		case Configuration::ButtonType::PS3:
			archiveDependencies.push_back(ArchiveDependency("PlaystationButtons", { "SystemCommon" }));
			WRITE_STRING(0x1688344, "ui_howps");
			WRITE_STRING(0x16886A8, "ui_howps");
			WRITE_STRING(0x1692BC4, "ui_howps");
			break;

		case Configuration::ButtonType::Switch:
			archiveDependencies.push_back(ArchiveDependency("SwitchProButtons", { "SystemCommon" }));
			WRITE_STRING(0x1688344, "ui_howns");
			WRITE_STRING(0x16886A8, "ui_howns");
			WRITE_STRING(0x1692BC4, "ui_howns");
			break;

		case Configuration::ButtonType::X360:
			archiveDependencies.push_back(ArchiveDependency("Xbox360Buttons", { "SystemCommon" }));
			break;

		default:
			break;
    }

	archiveDependencies.push_back(ArchiveDependency("SonicRankE", {}));
	archiveDependencies.push_back(ArchiveDependency("SonicActionCommonSWA", { "SonicActionCommon" }));
    INSTALL_HOOK(ParseArchiveTree);
}