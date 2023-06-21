#pragma once
class SaveDataStructure {
public:
	static inline int expLevel;
	static inline float expAmount;
	static inline int rings;
	SaveDataStructure() {};
	SaveDataStructure(int _expLevel = 0, float _expAmount = 0.0f, float _rings = 0) {
		expLevel = _expLevel;
		expAmount = _expAmount;
		rings = _rings;
	}
};
class Save
{
public:

	static SaveDataStructure saveData;

	static inline INIReader saveReader;

	static inline void Initialize() {
		saveReader = INIReader("sur_save.ini");
		LoadData();
	}

	static inline void LoadData() {
		saveData.expLevel = saveReader.GetInteger("EXP", "EXPLevel", 0);
		saveData.expAmount = saveReader.GetFloat("EXP", "EXPAmount", 0.0f);
		saveData.rings = saveReader.GetInteger("Hub", "Rings", 0);
	}

	static inline void SaveDataINI() {
		INIReader::INIFileClass saveFile = INIReader::INIFileClass();
		saveFile.SetIsPretty(false);

		saveFile.AddSection("EXP");
		saveFile.AddValue("EXP", "EXPLevel", saveData.expLevel);
		saveFile.AddValue("EXP", "EXPAmount", saveData.expAmount);

		saveFile.AddSection("Hub");
		saveFile.AddValue("Hub", "Rings", saveData.rings);

		INIReader::WriteINI("sur_save.ini", saveFile);
	}

    static void Install();
};
