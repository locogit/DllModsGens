using ConfigurationTool.Handlers;
using ConfigurationTool.Model;
using ConfigurationTool.Model.Configurations;
using ConfigurationTool.Model.Devices;
using ConfigurationTool.Model.Settings;
using ConfigurationTool.Settings.Model;
using System.Collections.Generic;

namespace GensMismatchFix
{
    public class Main
    {
        [DllExport]
        public static void Init()
        {
            // Load config file
            GraphicsConfiguration graphicsConfig = new GraphicsConfiguration();
            Configuration config = graphicsConfig.LoadConfiguration();

            // Declare variables to correct
            Resolution resolution = config.Resolution;
            RefreshRate refreshRate = config.RefreshRate;
            GraphicsAdapter adapter;

            // Load existing adapters
            List<GraphicsAdapter> adapters = DevicesHandler.GetGraphicsAdapters();

            // Find corresponding monitor ID
            adapter = FindAdapter(adapters, config);

            if (adapter == null)
            {
                // Unable to find adapter, use the default one
                adapter = adapters[0];
                resolution = adapter.Resolutions[0];
                refreshRate = resolution.RefreshRates[0];
            }

            // Change config
            config.GraphicsAdapter = adapter;
            config.Resolution = resolution;
            config.RefreshRate = refreshRate;

            // Re-save
            graphicsConfig.SaveConfiguration(config);
        }

        private static GraphicsAdapter FindAdapter(List<GraphicsAdapter> adapters, Configuration config)
        {
            if (config.GraphicsAdapter == null) return null; // No graphics adapter

            foreach (GraphicsAdapter adapter in adapters)
            {
                // Find by Monitor ID
                if (adapter.MonitorID.Equals(config.GraphicsAdapter.MonitorID))
                {
                    return adapter;
                }
            }

            return null;
        }
    }
}
