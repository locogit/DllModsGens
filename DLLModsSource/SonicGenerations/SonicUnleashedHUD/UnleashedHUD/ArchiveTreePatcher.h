#pragma once

#define XBOX_SERIES_AR = "XboxSeriesButtons"
#define PLAYSTATION_AR = "PlaystationButtons"
#define SWITCH_AR = "SwitchProButtons"

struct ArchiveDependency
{
    std::string archive;
    std::vector<std::string> dependencies;

    ArchiveDependency() {};
    ArchiveDependency(std::string archive, std::vector<std::string> dependencies) : archive(archive), dependencies(dependencies) {};
};



class ArchiveTreePatcher
{
public:
    static std::vector<ArchiveDependency> archiveDependencies;

    static void Install();
};