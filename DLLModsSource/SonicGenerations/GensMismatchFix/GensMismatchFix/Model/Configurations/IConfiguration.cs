namespace ConfigurationTool.Model.Configurations
{
    // Should be able to save and load configurations from a set place
    interface IConfiguration
    {
        Configuration LoadConfiguration();
        void SaveConfiguration(Configuration config);
    }
}
