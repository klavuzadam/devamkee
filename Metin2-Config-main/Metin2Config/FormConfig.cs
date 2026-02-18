/**
 * Author: blackdragonx61 / Mali
 * 30.03.2022
**/

#define DARK_MODE

using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace Metin2Config
{
    public partial class FormConfig : Form
    {
        public FormConfig()
        {
            InitializeComponent();
        }

        [DllImport("user32.dll")]
        public static extern bool EnumDisplaySettings(string deviceName, int modeNum, ref DEVMODE devMode);

        [StructLayout(LayoutKind.Sequential)]
        public struct DEVMODE
        {
            private const int CCHDEVICENAME = 0x20;
            private const int CCHFORMNAME = 0x20;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 0x20)]
            public string dmDeviceName;
            public short dmSpecVersion;
            public short dmDriverVersion;
            public short dmSize;
            public short dmDriverExtra;
            public int dmFields;
            public int dmPositionX;
            public int dmPositionY;
            public ScreenOrientation dmDisplayOrientation;
            public int dmDisplayFixedOutput;
            public short dmColor;
            public short dmDuplex;
            public short dmYResolution;
            public short dmTTOption;
            public short dmCollate;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 0x20)]
            public string dmFormName;
            public short dmLogPixels;
            public int dmBitsPerPel;
            public int dmPelsWidth;
            public int dmPelsHeight;
            public int dmDisplayFlags;
            public int dmDisplayFrequency;
            public int dmICMMethod;
            public int dmICMIntent;
            public int dmMediaType;
            public int dmDitherType;
            public int dmReserved1;
            public int dmReserved2;
            public int dmPanningWidth;
            public int dmPanningHeight;
        }

        public struct SLocale
        {
            public string language;
            public string code;
            public string name;

            public override string ToString()
            {
                return $"{this.code} {this.name}";
            }
        }

        public struct SScreenSettings
        {
            public int Width;
            public int Height;
            public int BPP;
            public List<int> FrequencyList;

            public SScreenSettings(int Width, int Height, int BPP)
            {
                this.Width = Width;
                this.Height = Height;
                this.BPP = BPP;
                FrequencyList = new List<int>();
            }

            public void AddFrequency(int Frequency)
            {
                if (!FrequencyList.Contains(Frequency))
                    FrequencyList.Add(Frequency);
            }

            public override string ToString()
            {
                return $"{this.Width}x{this.Height} {this.BPP}bpp";
            }
        }

        public enum ELocaleType
        {
            LANGUAGE,
            CODE,
            NAME,

            TYPE_MAX
        }

        private List<SScreenSettings> m_ScreenSettingsList = new List<SScreenSettings>();
        private List<SLocale> m_LocaleList = new List<SLocale>();
        private Dictionary<string, string> m_StringDictionary = new Dictionary<string, string>();

        private const string m_ConfigFileName = "metin2.cfg";
        private const string m_LocaleListFileName = "locale_list.txt";
        private const string m_LocaleFileName = "loca.cfg";
        private const string m_StringINIFileName = "config.ini";

        private bool m_DarkModeEnabled = false;
        private Color DarkBackColor = Color.FromArgb(22, 22, 24); // Main Form's Dark Back Color

        private string GetString(string key)
        {
#pragma warning disable CS8600 // Converting null literal or possible null value to non-nullable type.
            return m_StringDictionary.TryGetValue(key, out var value) ? value : "";
#pragma warning restore CS8600 // Converting null literal or possible null value to non-nullable type.
        }

        private void LoadStrings()
        {
            if (File.Exists(m_StringINIFileName) == false)
                return;

            var lines = File.ReadAllLines(m_StringINIFileName);
            foreach (var line in lines)
            {
                var arrString = line.Split('=');
                if (arrString.Length < 2)
                    continue;

                var k = arrString[0];
                if (k.EndsWith(" "))
                    k = k.Substring(0, k.Length - 1);

                var v = arrString[1];
                v = v.Replace("\"", "");
                if (v.StartsWith(" "))
                    v = v.Substring(1);

                m_StringDictionary.Add(k, v);
            }
        }

        private void SetStrings()
        {
            this.Text = GetString("CONFIG_STRING_CAPTINO");
            if (this.Text == "")
                this.Text = " ";

            btnOK.Text = GetString("CONFIG_STRING_APPLY");
            btnCANCEL.Text = GetString("CONFIG_STRING_CANCLE");

            gboxDisplaySettings.Text = GetString("CONFIG_STRING_GRAPHIC");
            lblResolution.Text = GetString("CONFIG_STRING_RESOLUTIN");
            lblFrequency.Text = GetString("CONFIG_STRING_FREQUENCY");
            lblGamma.Text = GetString("CONFIG_STRING_GAMMA");

            gboxSoundSettings.Text = GetString("CONFIG_STRING_SOUND");
            lblBGM.Text = GetString("CONFIG_STRING_MUSIC");
            lblSFX.Text = GetString("CONFIG_STRING_SOUNDEFFECT");

            gboxNightMode.Text = GetString("CONFIG_STRING_NIGHT_MODE");
            rbtnNightOn.Text = GetString("CONFIG_STRING_NIGHT_MODE_ON");
            rbtnNightOff.Text = GetString("CONFIG_STRING_NIGHT_MODE_OFF");

            gboxFogMode.Text = GetString("CONFIG_STRING_FOG_MODE");
            rbtnFogOn.Text = GetString("CONFIG_STRING_FOG_MODE_ON");
            rbtnFogOff.Text = GetString("CONFIG_STRING_FOG_MODE_OFF");

            gboxSnowMode.Text = GetString("CONFIG_STRING_SNOW_MODE");
            rbtnSnowOn.Text = GetString("CONFIG_STRING_SNOW_MODE_ON");
            rbtnSnowOff.Text = GetString("CONFIG_STRING_SNOW_MODE_OFF");

            gboxSnowTextureMode.Text = GetString("CONFIG_STRING_SNOW_TEXTURE_MODE");
            rbtnSnowTextureOn.Text = GetString("CONFIG_STRING_SNOW_TEXTURE_MODE_ON");
            rbtnSnowTextureOff.Text = GetString("CONFIG_STRING_SNOW_TEXTURE_MODE_OFF");

            gboxPetMode.Text = GetString("CONFIG_STRING_PET");
            rbtnPetOn.Text = GetString("CONFIG_STRING_PET_ON");
            rbtnPetOff.Text = GetString("CONFIG_STRING_PET_OFF");

            chboxUseSoftCursor.Text = GetString("CONFIG_STRING_USESOFTCURSOR");
            gboxLanguage.Text = GetString("CONFIG_STRING_LANGUAGE");

            gboxWindowSettings.Text = GetString("CONFIG_STRING_WINDOWSETTING");
            rbtnWindowMode.Text = GetString("CONFIG_STRING_WINDOWMODE");
            rbtnFullscreen.Text = GetString("CONFIG_STRING_FULLSCREEN");

            gboxIMESettings.Text = GetString("CONFIG_STRING_IMESETTING");
            rbtnGameIME.Text = GetString("CONFIG_STRING_GAMEIME");
            rbtnOutIME.Text = GetString("CONFIG_STRING_OUTIME");

            gboxGraphicSettings.Text = GetString("CONFIG_STRING_GRAPHICSETTING");
            lblShadowTarget.Text = GetString("CONFIG_STRING_SHADOW_TARGET");
            lblShadowQuality.Text = GetString("CONFIG_STRING_SHADOW_QUALITY");
            lblEffects.Text = GetString("CONFIG_STRING_EFFECT");
            lblPrivateShop.Text = GetString("CONFIG_STRING_PRIVATE_SHOP");
            lblDropItem.Text = GetString("CONFIG_STRING_DROP_ITEM");

            gboxNPCName.Text = GetString("CONFIG_STRING_NPC_NAME");
            rbtnNPCNameOn.Text = GetString("CONFIG_STRING_NPC_NAME_ON");
            rbtnNPCNameOff.Text = GetString("CONFIG_STRING_NPC_NAME_OFF");

            cboxTargetShadow.Items[0] = GetString("CONFIG_STRING_NONE");
            cboxTargetShadow.Items[1] = GetString("CONFIG_STRING_BG");
            cboxTargetShadow.Items[2] = GetString("CONFIG_STRING_BGSELF");
            cboxTargetShadow.Items[3] = GetString("CONFIG_STRING_ALL");

            cboxShadowQuality.Items[0] = GetString("CONFIG_STRING_BAD");
            cboxShadowQuality.Items[1] = GetString("CONFIG_STRING_AVERAGE");
            cboxShadowQuality.Items[2] = GetString("CONFIG_STRING_GOOD");

            for (int i = 0; i < cboxEffect.Items.Count; i++)
                cboxEffect.Items[i] = GetString($"CONFIG_STRING_EFFECT_LEVEL{i + 1}");

            for (int i = 0; i < cboxPrivateShop.Items.Count; i++)
                cboxPrivateShop.Items[i] = GetString($"CONFIG_STRING_PRIVATE_SHOP_LEVEL{i + 1}");

            for (int i = 0; i < cboxDropItem.Items.Count; i++)
                cboxDropItem.Items[i] = GetString($"CONFIG_STRING_DROP_ITEM_LEVEL{i + 1}");
        }

        private void LoadLocaleList()
        {
            if (File.Exists(m_LocaleListFileName) == false)
                return;

            var lines = File.ReadAllLines(m_LocaleListFileName);
            foreach (var line in lines)
            {
                var arrLocale = line.Split(' ');
                if (arrLocale.Length < (int)ELocaleType.TYPE_MAX)
                    continue;

                var localeObj = new SLocale
                {
                    language = arrLocale[(int)ELocaleType.LANGUAGE],
                    code = arrLocale[(int)ELocaleType.CODE],
                    name = arrLocale[(int)ELocaleType.NAME]
                };

                m_LocaleList.Add(localeObj);
                cboxLocale.Items.Add(localeObj.language);
            }
        }

        private void LoadSettings()
        {
            cboxTargetShadow.SelectedIndex = (cboxTargetShadow.Items.Count - 1);
            cboxShadowQuality.SelectedIndex = (cboxShadowQuality.Items.Count - 1);
            cboxEffect.SelectedIndex = 0;
            cboxPrivateShop.SelectedIndex = 0;
            cboxDropItem.SelectedIndex = 0;
            cboxGamma.SelectedIndex = 2;

            if (File.Exists(m_ConfigFileName))
            {
                try
                {
                    var TempScreenSettings = new SScreenSettings(-1, -1, -1);

                    var lines = File.ReadAllLines(m_ConfigFileName);
                    foreach (var line in lines)
                    {
                        if (line == "")
                            continue;

                        var Key = line.Substring(0, line.IndexOf('\t'));
                        var Value = line.Substring(line.LastIndexOf('\t') + 1);

                        switch (Key)
                        {
                            case "WIDTH":
                                TempScreenSettings.Width = Convert.ToInt32(Value);
                                break;

                            case "HEIGHT":
                                TempScreenSettings.Height = Convert.ToInt32(Value);
                                break;

                            case "BPP":
                                TempScreenSettings.BPP = Convert.ToInt32(Value);
                                break;

                            case "SOFTWARE_CURSOR":
                                chboxUseSoftCursor.Checked = (Value == "1");
                                break;

                            case "MUSIC_VOLUME":
                                if (Value.Contains('.'))
                                    trbarBGM.Value = (int)(decimal.Parse(Value, System.Globalization.CultureInfo.InvariantCulture) * trbarBGM.Maximum);
                                else
                                    trbarBGM.Value = Convert.ToByte(Value) * 200;
                                trbarBGM_Scroll(trbarBGM, null);
                                break;

                            case "VOICE_VOLUME":
                                trbarSFX.Value = Convert.ToByte(Value) * 10;
                                trbarSFX_Scroll(trbarSFX, null);
                                break;

                            case "GAMMA":
                                cboxGamma.SelectedIndex = (Convert.ToByte(Value) - 1);
                                break;

                            case "NIGHT_MODE_ON":
                                rbtnNightOff.Checked = (Value == "0");
                                break;

                            case "FOG_MODE_ON":
                                rbtnFogOff.Checked = (Value == "0");
                                break;

                            case "SNOW_MODE_ON":
                                rbtnSnowOff.Checked = (Value == "0");
                                break;

                            case "SNOW_TEXTURE_MODE":
                                rbtnSnowTextureOff.Checked = (Value == "0");
                                break;

                            case "WINDOWED":
                                rbtnWindowMode.Checked = (Value == "1");
                                break;

                            case "USE_DEFAULT_IME":
                                rbtnOutIME.Checked = (Value == "0");
                                break;

                            case "SHADOW_QUALITY_LEVEL":
                                cboxShadowQuality.SelectedIndex = Convert.ToByte(Value);
                                break;

                            case "SHADOW_TARGET_LEVEL":
                                cboxTargetShadow.SelectedIndex = Convert.ToByte(Value);
                                break;

                            case "EFFECT_LEVEL":
                                cboxEffect.SelectedIndex = Convert.ToByte(Value);
                                break;

                            case "PRIVATE_SHOP_LEVEL":
                                cboxPrivateShop.SelectedIndex = Convert.ToByte(Value);
                                break;

                            case "DROP_ITEM_LEVEL":
                                cboxDropItem.SelectedIndex = Convert.ToByte(Value);
                                break;

                            case "PET_STATUS":
                                rbtnPetOff.Checked = (Value == "0");
                                break;

                            case "NPC_NAME_STATUS":
                                rbtnNPCNameOff.Checked = (Value == "0");
                                break;

#if DARK_MODE
                            case "CONFIG_DARK_MODE":
                                if (Value == "1")
                                    pboxDarkMode_Click(pboxDarkMode, null);
                                break;
#endif
                        }
                    }

                    int idx = m_ScreenSettingsList.FindIndex(x =>
                        x.Width == TempScreenSettings.Width &&
                        x.Height == TempScreenSettings.Height &&
                        x.BPP == TempScreenSettings.BPP);

                    if (idx != -1)
                        cboxResolution.SelectedIndex = idx;
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Error");
                }
            }

            if (File.Exists(m_LocaleFileName))
            {
                var lines = File.ReadAllLines(m_LocaleFileName);
                foreach (var line in lines)
                {
                    var arrLocale = line.Split(' ');
                    if (arrLocale.Length < ((int)ELocaleType.TYPE_MAX - 1))
                        continue;

                    var localeName = arrLocale[(int)ELocaleType.NAME - 1];
                    int idx = m_LocaleList.FindIndex(x => x.name == localeName);
                    if (idx != -1)
                    {
                        cboxLocale.SelectedIndex = idx;
                        break;
                    }
                }
            }
        }

        private void GetScreenSettings()
        {
            DEVMODE vDevMode = new DEVMODE();
            int i = 0;
#pragma warning disable CS8625 // Cannot convert null literal to non-nullable reference type.
            while (EnumDisplaySettings(null, i++, ref vDevMode))
            {
                if (vDevMode.dmPelsWidth < 800 && vDevMode.dmPelsHeight < 600)
                    continue;

                int index = m_ScreenSettingsList.FindIndex(x =>
                    x.Width == vDevMode.dmPelsWidth &&
                    x.Height == vDevMode.dmPelsHeight &&
                    x.BPP == vDevMode.dmBitsPerPel);

                SScreenSettings s = (index != -1) ? m_ScreenSettingsList[index]
                    : new SScreenSettings(vDevMode.dmPelsWidth, vDevMode.dmPelsHeight, vDevMode.dmBitsPerPel);

                s.AddFrequency(vDevMode.dmDisplayFrequency);

                if (index == -1)
                    m_ScreenSettingsList.Add(s);
            }
#pragma warning restore CS8625 // Cannot convert null literal to non-nullable reference type.

            foreach (SScreenSettings s in m_ScreenSettingsList)
                cboxResolution.Items.Add(s.ToString());

            if (cboxResolution.Items.Count > 0)
                cboxResolution.SelectedIndex = 0;
        }

        private void FormConfig_Load(object sender, EventArgs e)
        {
#if DARK_MODE
            pboxDarkMode.Show();
#endif
            LoadStrings();
            SetStrings();
            LoadLocaleList();
            GetScreenSettings();
            LoadSettings();
        }

        private void btnCANCEL_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            using (var configFile = new StreamWriter(m_ConfigFileName))
            {
                configFile.WriteLine($"WIDTH\t\t\t\t\t\t{m_ScreenSettingsList.ElementAt(cboxResolution.SelectedIndex).Width}");
                configFile.WriteLine($"HEIGHT\t\t\t\t\t\t{m_ScreenSettingsList.ElementAt(cboxResolution.SelectedIndex).Height}");
                configFile.WriteLine($"BPP\t\t\t\t\t\t\t{m_ScreenSettingsList.ElementAt(cboxResolution.SelectedIndex).BPP}");
                configFile.WriteLine($"FREQUENCY\t\t\t\t\t{cboxFrequency.Text}");
                configFile.WriteLine($"SOFTWARE_CURSOR\t\t\t\t{(chboxUseSoftCursor.Checked ? 1 : 0)}");
                configFile.WriteLine($"OBJECT_CULLING\t\t\t\t1"); // default
                configFile.WriteLine($"MUSIC_VOLUME\t\t\t\t{((double)trbarBGM.Value / trbarBGM.Maximum).ToString("F3", System.Globalization.CultureInfo.InvariantCulture)}");
                configFile.WriteLine($"VOICE_VOLUME\t\t\t\t{tboxSFX.Text}");
                configFile.WriteLine($"GAMMA\t\t\t\t\t\t{cboxGamma.Text}");
                configFile.WriteLine($"IS_SAVE_ID\t\t\t\t\t0"); // default
                configFile.WriteLine($"SAVE_ID\t\t\t\t\t\t0"); // default
                configFile.WriteLine($"PRE_LOADING_DELAY_TIME\t\t20"); // default
                configFile.WriteLine($"DECOMPRESSED_TEXTURE\t\t0"); // default
                configFile.WriteLine($"NIGHT_MODE_ON\t\t\t\t{(rbtnNightOn.Checked ? 1 : 0)}");
                configFile.WriteLine($"FOG_MODE_ON\t\t\t\t\t{(rbtnFogOn.Checked ? 1 : 0)}");
                configFile.WriteLine($"SNOW_MODE_ON\t\t\t\t{(rbtnSnowOn.Checked ? 1 : 0)}");
                configFile.WriteLine($"SNOW_TEXTURE_MODE\t\t\t{(rbtnSnowTextureOn.Checked ? 1 : 0)}");
                configFile.WriteLine($"WINDOWED\t\t\t\t\t{(rbtnWindowMode.Checked ? 1 : 0)}");
                configFile.WriteLine($"SHOW_MOBLEVEL\t\t\t\t1"); // default
                configFile.WriteLine($"SHOW_MOBAIFLAG\t\t\t\t1"); // default
                configFile.WriteLine($"CHAT_FILTER_DICE\t\t\t1"); // default
                configFile.WriteLine($"USE_DEFAULT_IME\t\t\t\t{(rbtnGameIME.Checked ? 1 : 0)}");
                configFile.WriteLine($"SHADOW_QUALITY_LEVEL\t\t{cboxShadowQuality.SelectedIndex}");
                configFile.WriteLine($"SHADOW_TARGET_LEVEL\t\t\t{cboxTargetShadow.SelectedIndex}");
                configFile.WriteLine($"EFFECT_LEVEL\t\t\t\t{cboxEffect.SelectedIndex}");
                configFile.WriteLine($"PRIVATE_SHOP_LEVEL\t\t\t{cboxPrivateShop.SelectedIndex}");
                configFile.WriteLine($"DROP_ITEM_LEVEL\t\t\t\t{cboxDropItem.SelectedIndex}");
                configFile.WriteLine($"PET_STATUS\t\t\t\t\t{(rbtnPetOn.Checked ? 1 : 0)}");
                configFile.WriteLine($"NPC_NAME_STATUS\t\t\t\t{(rbtnNPCNameOn.Checked ? 1 : 0)}");
                configFile.WriteLine($"EVENT_BANNER_FLAG\t\t\t0"); // default
#if DARK_MODE
                configFile.WriteLine($"CONFIG_DARK_MODE\t\t\t{(m_DarkModeEnabled ? 1 : 0)}"); // default
#endif
            }

            if (cboxLocale.SelectedIndex != -1 && cboxLocale.SelectedIndex < m_LocaleList.Count)
            {
                var locale = m_LocaleList.ElementAt(cboxLocale.SelectedIndex);
                using (var localeFile = new StreamWriter(m_LocaleFileName))
                    localeFile.Write(locale.ToString());
            }

            btnCANCEL.PerformClick();
        }

        private void ChangeColors(Control parent)
        {
            foreach (Control c in parent.Controls)
            {
                if (c is GroupBox || c is CheckBox)
                    c.ForeColor = m_DarkModeEnabled ? SystemColors.ControlText : Color.White;

                ChangeColors(c);
            }
        }

        private void pboxDarkMode_Click(object sender, EventArgs e)
        {
            if (m_DarkModeEnabled)
            {
                BackColor = SystemColors.Control;
                pboxDarkMode.Image = global::Metin2Config.Properties.Resources.night_off;
            }
            else
            {
                BackColor = DarkBackColor;
                pboxDarkMode.Image = global::Metin2Config.Properties.Resources.night_on;
            }

            ChangeColors(this);
            m_DarkModeEnabled = !m_DarkModeEnabled;
        }

        private void ShowSlowWarning(object sender, EventArgs e)
        {
            MessageBox.Show(GetString("CONFIG_STRING_SPEEDLOW"), GetString("CONFIG_STRING_NOTIC"));
        }

        private void trbarBGM_Scroll(object sender, EventArgs e)
        {
            tboxBGM.Text = ((double)trbarBGM.Value / trbarBGM.Maximum).ToString("F1", System.Globalization.CultureInfo.InvariantCulture);
        }

        private void trbarSFX_Scroll(object sender, EventArgs e)
        {
            tboxSFX.Text = (trbarSFX.Value / 10).ToString();
        }

        private void cboxResolution_SelectedIndexChanged(object sender, EventArgs e)
        {
            cboxFrequency.Items.Clear();

            if (cboxResolution.SelectedIndex == -1 || cboxResolution.SelectedIndex >= m_ScreenSettingsList.Count)
                return;

            var Screen = m_ScreenSettingsList.ElementAt(cboxResolution.SelectedIndex);
            foreach (var frequency in Screen.FrequencyList)
                cboxFrequency.Items.Add(frequency);

            if (cboxFrequency.Items.Count > 0)
                cboxFrequency.SelectedIndex = 0;
        }
    }
}