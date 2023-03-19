using ConfigurationTool.Model.Devices;
using ConfigurationTool.Model.Settings;
using ConfigurationTool.Settings.Model;

namespace ConfigurationTool.Model
{
    class Configuration
    {
        public Resolution Resolution;
        public RefreshRate RefreshRate;

        public GraphicsAdapter GraphicsAdapter;
        public DepthFormat DepthFormat = DepthFormat.INTZ;

        public OnOff Antialiasing = OnOff.On;
        public OnOff VSync = OnOff.On;
        public OnOff Analytics = OnOff.On;

        public HighLow ShadowQuality = HighLow.High;
        public HighLow ReflectionQuality = HighLow.High;

        public DisplayMode DisplayMode = DisplayMode.Letterbox;
    }
}
