/**
 * Author: blackdragonx61 / Mali
 * 30.03.2022
**/

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
                // 0 veya 1 genelde 60Hz (Varsayılan) demektir.
                int freq = (Frequency <= 1) ? 60 : Frequency;

                // Sadece 60 ve 75 frekanslarını kabul et
                if (freq == 60 || freq == 75)
                {
                    if (!FrequencyList.Contains(freq))
                        FrequencyList.Add(freq);
                }
            }

            public override string ToString()
            {
                return $"{this.Width}x{this.Height} {this.BPP}bpp";
            }
        }

        private List<SScreenSettings> m_ScreenSettingsList = new List<SScreenSettings>();
        private List<SLocale> m_LocaleList = new List<SLocale>();

        private const string m_ConfigFileName = "game1.cfg";
        private const string m_LocaleListFileName = "locale_list.txt";
        private const string m_LocaleFileName = "loca.cfg";

        private void LoadLocaleList()
        {
            cboxLocale.Items.Clear();
            m_LocaleList.Clear();

            // Önce varsayılan Türkçe'yi ekleyelim kimseden bağımsız olsun
            SLocale trLocale = new SLocale { language = "TURKISH", code = "tr", name = "TURKISH" };
            m_LocaleList.Add(trLocale);
            cboxLocale.Items.Add("Türkçe");

             if (File.Exists(m_LocaleListFileName))
             {
                try
                {
                    var lines = File.ReadAllLines(m_LocaleListFileName);
                    foreach (var line in lines)
                    {
                        if (string.IsNullOrWhiteSpace(line)) continue;

                        var tokens = line.Split('\t');
                        if (tokens.Length < 3) continue;

                        if (tokens[1] == "tr") continue; // Zaten ekledik

                        SLocale localeObj = new SLocale();
                        localeObj.language = tokens[0];
                        localeObj.code = tokens[1];
                        localeObj.name = tokens[2];

                        m_LocaleList.Add(localeObj);
                        cboxLocale.Items.Add(localeObj.language); // Dil ismini gösterelim
                    }
                }
                catch { }
             }

            if (cboxLocale.Items.Count > 0)
                cboxLocale.SelectedIndex = 0;
        }

        private void SetStrings()
        {
            this.Text = "Metin2 Ayarları";

            btnOK.Text = "Kaydet";
            btnCANCEL.Text = "İptal";

            gboxDisplay.Text = "Ekran";
            lblResolution.Text = "Çözünürlük";
            lblFrequency.Text = "Frekans";
            lblGamma.Text = "Gama";

            gboxSound.Text = "Ses";
            lblBGM.Text = "Müzik";
            lblSFX.Text = "Efekt";

            gboxWindowMode.Text = "Pencere";
            rbtnWindowMode.Text = "Pencere Modu";
            rbtnFullscreen.Text = "Tam Ekran";
            
            gboxIMEMode.Text = "IME Modu";
            rbtnGameIME.Text = "Oyun IME";
            rbtnExternalIME.Text = "Harici IME";

            gboxGFX.Text = "Grafik";
            lblFog.Text = "Sis";
            lblTNL.Text = "TNL";
            lblShadow.Text = "Gölge";
            lblAntiAlias.Text = "Yumuşatma";

            chboxUseSoftCursor.Text = "Yazılım İmleci";
            
            // Populate ComboBoxes
            cboxGamma.Items.Clear();
            cboxGamma.Items.AddRange(new object[] { "1", "2", "3", "4", "5" });

            cboxFog.Items.Clear();
            cboxFog.Items.AddRange(new object[] { "Yakın", "Orta", "Uzak" }); // Near, Mid, Far

            cboxTNL.Items.Clear();
            cboxTNL.Items.AddRange(new object[] { "Otomatik", "CPU", "GPU" }); // Auto, CPU, GPU

            cboxShadow.Items.Clear();
            // Brak, Teren, Teren i gracz, Teren gracz i NPC, Niskie, Srednie, Wysokie, Ultra
            cboxShadow.Items.AddRange(new object[] { 
                "Yok",                  // Brak
                "Zemin",                // Teren
                "Zemin ve Karakter",    // Teren i gracz
                "Zemin, Karakter ve NPC", // Teren, gracz i NPC
                "Düşük",                // Niskie
                "Orta",                 // Srednie
                "Yüksek",               // Wysokie
                "Ultra"                 // Ultra
            });

            cboxAntiAlias.Items.Clear();
            cboxAntiAlias.Items.AddRange(new object[] { "Yok", "2x", "4x", "8x" }); // Brak -> Yok
        }

        private List<string> m_FileLines = new List<string>();
        private Dictionary<string, int> m_KeyLineMap = new Dictionary<string, int>();

        private void LoadSettings()
        {
            // Defaults
            cboxFog.SelectedIndex = 2; // Uzak
            cboxTNL.SelectedIndex = 0; // Otomatik
            cboxShadow.SelectedIndex = 6; // Yüksek (Wysokie)
            cboxGamma.SelectedIndex = 2; // 3
            cboxAntiAlias.SelectedIndex = 0; // Yok

            m_FileLines.Clear();
            m_KeyLineMap.Clear();

            if (File.Exists(m_ConfigFileName))
            {
                try
                {
                    m_FileLines = File.ReadAllLines(m_ConfigFileName).ToList();
                    var TempScreenSettings = new SScreenSettings(-1, -1, -1);

                    for (int i = 0; i < m_FileLines.Count; i++)
                    {
                        string line = m_FileLines[i];
                        if (string.IsNullOrWhiteSpace(line))
                            continue;

                        int tabIndex = line.IndexOf('\t');
                        int spaceIndex = line.IndexOf(' ');
                        int separatorIndex = (tabIndex != -1 && (spaceIndex == -1 || tabIndex < spaceIndex)) ? tabIndex : spaceIndex;

                        if (separatorIndex == -1) 
                            continue;

                        var Key = line.Substring(0, separatorIndex).Trim();
                        var Value = line.Substring(separatorIndex).Trim(); // Trim başındaki boşlukları/tabları atar

                        if (!m_KeyLineMap.ContainsKey(Key))
                        {
                            m_KeyLineMap.Add(Key, i);
                        }

                        if (string.IsNullOrEmpty(Value))
                            continue;

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
                                
                            case "FREQUENCY":
                                int freq;
                                if (int.TryParse(Value, out freq))
                                    TempScreenSettings.FrequencyList.Add(freq);
                                break;

                            case "MUSIC_VOLUME":
                                if (Value.Contains('.'))
                                {
                                    float vol;
                                    if (float.TryParse(Value, System.Globalization.NumberStyles.Any, System.Globalization.CultureInfo.InvariantCulture, out vol))
                                        trbarBGM.Value = (int)(vol * trbarBGM.Maximum);
                                }
                                else
                                {
                                    byte vol;
                                    if (byte.TryParse(Value, out vol))
                                        trbarBGM.Value = vol * 200;
                                }
                                trbarBGM_Scroll(trbarBGM, null);
                                break;

                            case "VOICE_VOLUME":
                                byte voiceVol;
                                if (byte.TryParse(Value, out voiceVol))
                                {
                                    trbarSFX.Value = voiceVol * 10;
                                    trbarSFX_Scroll(trbarSFX, null);
                                }
                                break;

                            case "GAMMA":
                                byte gammaVal;
                                if (byte.TryParse(Value, out gammaVal) && gammaVal >= 1 && gammaVal <= cboxGamma.Items.Count)
                                    cboxGamma.SelectedIndex = (gammaVal - 1);
                                break;

                            case "WINDOWED":
                                // 1 = Pencere, 0 veya 2 = Tam Ekran (2 genelde daha stabildir)
                                rbtnWindowMode.Checked = (Value == "1");
                                rbtnFullscreen.Checked = (Value == "0" || Value == "2");
                                break;
                                
                            case "USE_DEFAULT_IME":
                                rbtnGameIME.Checked = (Value == "0");
                                rbtnExternalIME.Checked = (Value == "1");
                                break;
                                
                            case "SOFTWARE_CURSOR":
                                chboxUseSoftCursor.Checked = (Value == "1");
                                break;
                                
                            case "VISIBILITY": // FOG
                            case "FOG":
                                int fogVal;
                                if (int.TryParse(Value, out fogVal) && fogVal >= 1 && fogVal <= 3)
                                    cboxFog.SelectedIndex = fogVal - 1;
                                break;
                                
                            case "SHADOW_LEVEL":
                            case "SHADOW":
                                int shadowVal;
                                if (int.TryParse(Value, out shadowVal) && shadowVal >= 0 && shadowVal < cboxShadow.Items.Count)
                                    cboxShadow.SelectedIndex = shadowVal;
                                break;

                            case "TNL_MODE": 
                            case "TNL":
                            case "SOFTWARE_TILING":
                                int tnlVal;
                                if (int.TryParse(Value, out tnlVal))
                                {
                                    // Kullanıcı bildirimine göre:
                                    // 0 = Auto
                                    // 1 = CPU
                                    // 2 = GPU
                                    if (tnlVal >= 0 && tnlVal <= 2)
                                        cboxTNL.SelectedIndex = tnlVal;
                                }
                                break;

                            case "ANTIALIASING_LEVEL":
                            case "MULTI_SAMPLE":
                                int aaVal;
                                if (int.TryParse(Value, out aaVal))
                                {
                                    // Kullanıcı bildirimine göre Index = Value (0, 1, 2, 3)
                                    if (aaVal >= 0 && aaVal <= 3)
                                        cboxAntiAlias.SelectedIndex = aaVal;
                                }
                                break;

                            case "LANGUAGE":
                                // game1.cfg içindeki LANGUAGE tr kısmını okuyalım
                                int langIdx = m_LocaleList.FindIndex(x => x.code == Value.Trim());
                                if (langIdx != -1)
                                    cboxLocale.SelectedIndex = langIdx;
                                break;
                        }
                    }


                    int idx = m_ScreenSettingsList.FindIndex(x =>
                        x.Width == TempScreenSettings.Width &&
                        x.Height == TempScreenSettings.Height &&
                        x.BPP == TempScreenSettings.BPP);

                    if (idx != -1)
                    {
                        cboxResolution.SelectedIndex = idx;
                        
                        // Resolution değişti, Frequency listesi doldu (SelectedIndexChanged).
                        // Şimdi varsa doğru frekansı seç.
                        if (TempScreenSettings.FrequencyList.Count > 0)
                        {
                            int targetFreq = TempScreenSettings.FrequencyList[0];
                            for (int f = 0; f < cboxFrequency.Items.Count; f++)
                            {
                                if (cboxFrequency.Items[f].ToString() == targetFreq.ToString())
                                {
                                    cboxFrequency.SelectedIndex = f;
                                    break;
                                }
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Error");
                }
            }

            // loca.cfg dosyasını oku ve dili seç
            if (File.Exists(m_LocaleFileName))
            {
                try
                {
                    string currentLocale = File.ReadAllText(m_LocaleFileName).Trim();
                    for (int i = 0; i < m_LocaleList.Count; i++)
                    {
                        // SLocale.ToString() "ID Name" formatında döner, loca.cfg'deki değerle karşılaştırıyoruz.
                        if (m_LocaleList[i].ToString() == currentLocale)
                        {
                            cboxLocale.SelectedIndex = i;
                            break;
                        }
                    }
                }
                catch { }
            }
        }

        private void GetScreenSettings()
        {
            DEVMODE vDevMode = new DEVMODE();
            int i = 0;
#pragma warning disable CS8625
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
#pragma warning restore CS8625

            foreach (SScreenSettings s in m_ScreenSettingsList)
                cboxResolution.Items.Add(s.ToString());

            if (cboxResolution.Items.Count > 0)
                cboxResolution.SelectedIndex = 0;
        }

        private void UpdateConfigLine(string key, string value, bool forceAdd = false)
        {
            // Orijinal dosya yapısı: KEY [BOŞLUK] VALUE
            string newLine = $"{key} {value}";

            if (m_KeyLineMap.ContainsKey(key))
            {
                int index = m_KeyLineMap[key];
                // Var olan satırın formatını korumak yerine yenisini yazıyoruz ama
                // gereksiz boşlukları azaltıyoruz.
                m_FileLines[index] = newLine;
            }
            else
            {
                // Sadece zorunluysa ekle
                if (forceAdd)
                {
                    m_FileLines.Add(newLine);
                    m_KeyLineMap.Add(key, m_FileLines.Count - 1);
                }
            }
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (cboxResolution.SelectedIndex >= 0 && cboxResolution.SelectedIndex < m_ScreenSettingsList.Count)
            {
                var screen = m_ScreenSettingsList.ElementAt(cboxResolution.SelectedIndex);
                UpdateConfigLine("WIDTH", screen.Width.ToString(), true);
                UpdateConfigLine("HEIGHT", screen.Height.ToString(), true);
                UpdateConfigLine("BPP", screen.BPP.ToString(), true);
                UpdateConfigLine("FREQUENCY", cboxFrequency.Text, true); // Frekans bazen olmayabilir ama ekleyelim.
            }

            // Aşağıdaki ayarlar eğer dosyada varsa güncellenir, yoksa EKLENMEZ.
            // Böylece orijinal dosyada olmayan ayarlar (kullanıcının şikayet ettiği 33. satır sonrası)
            // zorla dosyaya sokulmaz.
            UpdateConfigLine("OBJECT_CULLING", "1");
            UpdateConfigLine("MUSIC_VOLUME", ((double)trbarBGM.Value / trbarBGM.Maximum).ToString("F3", System.Globalization.CultureInfo.InvariantCulture));
            UpdateConfigLine("VOICE_VOLUME", tboxSFX.Text);
            UpdateConfigLine("GAMMA", cboxGamma.Text);
            UpdateConfigLine("IS_SAVE_ID", "0");
            UpdateConfigLine("SAVE_ID", "0");
            UpdateConfigLine("PRE_LOADING_DELAY_TIME", "20");
            UpdateConfigLine("DECOMPRESSED_TEXTURE", "0");

            // Bu ayarlar Config Tool'un temel işlevi, bunlar yoksa eklenebilir ama
            // yine de kullanıcının "bizimki eklemiş" şikayetine binaen
            // sadece varsa güncelle diyelim. Eğer çok gerekirse true yaparız.
            // Ancak Windowed, IME ve SoftCursor genellikle vardır. Yoksa da varsayılan çalışır.
            // Tam Ekran seçildiğinde bozmaması için kullanıcının çalışan ayarı olan '2'yi yazıyoruz.
            UpdateConfigLine("WINDOWED", (rbtnWindowMode.Checked ? "1" : "2"), true); 
            UpdateConfigLine("USE_DEFAULT_IME", (rbtnExternalIME.Checked ? "1" : "0"), true);
            UpdateConfigLine("SOFTWARE_CURSOR", (chboxUseSoftCursor.Checked ? "1" : "0"), true); 

            // Grafik ayarlarını zorunlu kaydet (forceAdd = true), yoksa ayar değişmez.
            UpdateConfigLine("VISIBILITY", (cboxFog.SelectedIndex + 1).ToString(), true);
            UpdateConfigLine("SHADOW_LEVEL", cboxShadow.SelectedIndex.ToString(), true);
            
            // TNL: 0:Auto, 1:CPU, 2:GPU (Sırayla index ile aynı)
            UpdateConfigLine("SOFTWARE_TILING", cboxTNL.SelectedIndex.ToString(), true);

            // Antialiasing: Yok(0), 2x(1), 4x(2), 8x(3) (Index ile aynı)
            UpdateConfigLine("ANTIALIASING_LEVEL", cboxAntiAlias.SelectedIndex.ToString(), true);
            
            // Eğer dosyada TNL_MODE gibi alternatif isimler varsa onları da güncel tutalım
            if (m_KeyLineMap.ContainsKey("TNL_MODE")) UpdateConfigLine("TNL_MODE", cboxTNL.SelectedIndex.ToString());
            if (m_KeyLineMap.ContainsKey("TNL")) UpdateConfigLine("TNL", cboxTNL.SelectedIndex.ToString());
            if (m_KeyLineMap.ContainsKey("FOG")) UpdateConfigLine("FOG", (cboxFog.SelectedIndex + 1).ToString());

            // Dil ayarını LANGUAGE tr olarak game1.cfg'ye ekleyelim (Zorunlu)
            if (cboxLocale.SelectedIndex != -1 && cboxLocale.SelectedIndex < m_LocaleList.Count)
            {
                var locale = m_LocaleList.ElementAt(cboxLocale.SelectedIndex);
                UpdateConfigLine("LANGUAGE", locale.code, true);
            }

            try
            {
                File.WriteAllLines(m_ConfigFileName, m_FileLines);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Ayarlar kaydedilemedi: " + ex.Message);
            }            

            // Save Locale
            if (cboxLocale.SelectedIndex != -1 && cboxLocale.SelectedIndex < m_LocaleList.Count)
            {
                var locale = m_LocaleList.ElementAt(cboxLocale.SelectedIndex);
                using (var localeFile = new StreamWriter(m_LocaleFileName))
                    localeFile.Write(locale.ToString());
            }

            Application.Exit();
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
            
            // Orijinal Metin2 sadece 60 ve 75 değerlerini gösterir.
            // Donanımdan gelse de gelmese de bu ikisini ekliyoruz (Uyumluluk için).
            if (!Screen.FrequencyList.Contains(60)) Screen.FrequencyList.Add(60);
            if (!Screen.FrequencyList.Contains(75)) Screen.FrequencyList.Add(75);

            Screen.FrequencyList.Sort();
            foreach (var frequency in Screen.FrequencyList)
                cboxFrequency.Items.Add(frequency);

            if (cboxFrequency.Items.Count > 0)
                cboxFrequency.SelectedIndex = 0;
        }

        private void btnCANCEL_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void FormConfig_Load(object sender, EventArgs e)
        {
            SetStrings();
            LoadLocaleList();
            GetScreenSettings();
            LoadSettings();
        }
    }
}