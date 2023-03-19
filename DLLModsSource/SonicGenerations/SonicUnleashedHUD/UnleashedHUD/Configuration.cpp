#include "Configuration.h"
bool Configuration::windowTitle = true;
Configuration::WindowTitleIcon Configuration::windowTitleIcon = WindowTitleIcon::X360;
Configuration::WindowTitleRegion Configuration::windowTitleRegion = WindowTitleRegion::English;
Configuration::ButtonType Configuration::buttonType = ButtonType::X360;

bool Configuration::unleashedResultMusic = true;

void Configuration::Read()
{
	INIReader reader(INI_FILE);

	// Appearance
	windowTitle		  = reader.GetBoolean("Appearance", "windowTitle", windowTitle);
	windowTitleIcon   = (WindowTitleIcon)reader.GetInteger("Appearance", "windowTitleIcon", (int)windowTitleIcon);
	windowTitleRegion = (WindowTitleRegion)reader.GetInteger("Appearance", "windowTitleRegion", windowTitleRegion);
	buttonType		  = (ButtonType)reader.GetInteger("Appearance", "buttonType", (int)buttonType);

	// Music
	unleashedResultMusic = reader.GetBoolean("Music", "resultMusic", unleashedResultMusic);
}
