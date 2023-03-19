using ConfigurationTool.Model.Devices;
using ConfigurationTool.Model.Settings;
using ConfigurationTool.Settings.Model;
using System;
using System.Diagnostics;
using System.IO;

namespace ConfigurationTool.Model.Configurations
{
    class GraphicsConfiguration : IConfiguration
    {
        public string ConfigLocation => Path.Combine(
            Path.GetDirectoryName(Process.GetCurrentProcess().MainModule.FileName), // Executable path
            "GraphicsConfig.cfg"); // File Name

        public Configuration LoadConfiguration()
        {
            Configuration config = new Configuration();

            if (!File.Exists(ConfigLocation)) return config;
            try
            {
                using (StreamReader sr = new StreamReader(new BufferedStream(File.Open(ConfigLocation, FileMode.Open))))
                {
                    sr.ReadLine(); // Skip warning line
                    string adapterDesc = sr.ReadLine();
                    string adapterName = sr.ReadLine();
                    if (String.IsNullOrEmpty(adapterName) || String.IsNullOrEmpty(adapterName))
                    {
                        return config;
                    }

                    string[] resString = sr.ReadLine().Split('.');
                    config.Resolution = new Resolution()
                    {
                        Width = int.Parse(resString[0]),
                        Height = int.Parse(resString[1]),
                    };
                    config.RefreshRate = new RefreshRate(int.Parse(resString[2]));

                    config.Antialiasing = int.Parse(sr.ReadLine()) > 0 ? OnOff.On : OnOff.Off;
                    config.VSync = int.Parse(sr.ReadLine()) > 0 ? OnOff.On : OnOff.Off;

                    config.ShadowQuality = int.Parse(sr.ReadLine()) > 0 ? HighLow.High : HighLow.Low;
                    config.ReflectionQuality = int.Parse(sr.ReadLine()) > 0 ? HighLow.High : HighLow.Low;

                    config.DisplayMode = int.Parse(sr.ReadLine()) > 0 ? DisplayMode.Letterbox : DisplayMode.Widescreen;

                    config.GraphicsAdapter = new GraphicsAdapter()
                    {
                        GUID = sr.ReadLine(),
                        Name = adapterName,
                        Description = adapterDesc,
                        MonitorID = sr.ReadLine()
                    };

                    config.DepthFormat = DepthFormat.FromFourCC(int.Parse(sr.ReadLine()));
                    return config;
                }
            }
            catch
            {
                return new Configuration();
            }
        }

        public void SaveConfiguration(Configuration config)
        {

            using (StreamWriter writer = new StreamWriter(ConfigLocation))
            {
                writer.WriteLine("This file has been re-saved by Sonic Generations.");
                writer.WriteLine(config.GraphicsAdapter.Description);
                writer.WriteLine(config.GraphicsAdapter.Name);
                Resolution res = config.Resolution;
                writer.WriteLine($"{res.Width}.{res.Height}.{config.RefreshRate.Value}");
                writer.WriteLine((int)config.Antialiasing);
                writer.WriteLine((int)config.VSync);
                writer.WriteLine((int)config.ShadowQuality);
                writer.WriteLine((int)config.ReflectionQuality);
                writer.WriteLine((int)config.DisplayMode);
                writer.WriteLine(config.GraphicsAdapter.GUID);
                writer.WriteLine(config.GraphicsAdapter.MonitorID);
                writer.WriteLine(config.DepthFormat.GetFourCC());
            }
        }
    }
}
