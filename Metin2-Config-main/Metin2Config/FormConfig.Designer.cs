using System.Windows.Forms;

namespace Metin2Config
{
    partial class FormConfig
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormConfig));
            this.gboxDisplay = new System.Windows.Forms.GroupBox();
            this.cboxGamma = new System.Windows.Forms.ComboBox();
            this.lblGamma = new System.Windows.Forms.Label();
            this.cboxFrequency = new System.Windows.Forms.ComboBox();
            this.lblFrequency = new System.Windows.Forms.Label();
            this.cboxResolution = new System.Windows.Forms.ComboBox();
            this.lblResolution = new System.Windows.Forms.Label();
            this.gboxSound = new System.Windows.Forms.GroupBox();
            this.tboxSFX = new System.Windows.Forms.TextBox();
            this.trbarSFX = new System.Windows.Forms.TrackBar();
            this.lblSFX = new System.Windows.Forms.Label();
            this.tboxBGM = new System.Windows.Forms.TextBox();
            this.trbarBGM = new System.Windows.Forms.TrackBar();
            this.lblBGM = new System.Windows.Forms.Label();
            this.chboxUseSoftCursor = new System.Windows.Forms.CheckBox();
            this.cboxLocale = new System.Windows.Forms.ComboBox();
            this.btnOK = new System.Windows.Forms.Button();
            this.btnCANCEL = new System.Windows.Forms.Button();
            this.gboxWindowMode = new System.Windows.Forms.GroupBox();
            this.rbtnWindowMode = new System.Windows.Forms.RadioButton();
            this.rbtnFullscreen = new System.Windows.Forms.RadioButton();
            this.gboxIMEMode = new System.Windows.Forms.GroupBox();
            this.rbtnExternalIME = new System.Windows.Forms.RadioButton();
            this.rbtnGameIME = new System.Windows.Forms.RadioButton();
            this.gboxGFX = new System.Windows.Forms.GroupBox();
            this.cboxAntiAlias = new System.Windows.Forms.ComboBox();
            this.lblAntiAlias = new System.Windows.Forms.Label();
            this.cboxShadow = new System.Windows.Forms.ComboBox();
            this.lblShadow = new System.Windows.Forms.Label();
            this.cboxTNL = new System.Windows.Forms.ComboBox();
            this.lblTNL = new System.Windows.Forms.Label();
            this.cboxFog = new System.Windows.Forms.ComboBox();
            this.lblFog = new System.Windows.Forms.Label();
            this.gboxDisplay.SuspendLayout();
            this.gboxSound.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trbarSFX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trbarBGM)).BeginInit();
            this.gboxWindowMode.SuspendLayout();
            this.gboxIMEMode.SuspendLayout();
            this.gboxGFX.SuspendLayout();
            this.SuspendLayout();
            // 
            // gboxDisplay
            // 
            this.gboxDisplay.Controls.Add(this.cboxGamma);
            this.gboxDisplay.Controls.Add(this.lblGamma);
            this.gboxDisplay.Controls.Add(this.cboxFrequency);
            this.gboxDisplay.Controls.Add(this.lblFrequency);
            this.gboxDisplay.Controls.Add(this.cboxResolution);
            this.gboxDisplay.Controls.Add(this.lblResolution);
            this.gboxDisplay.Location = new System.Drawing.Point(12, 12);
            this.gboxDisplay.Name = "gboxDisplay";
            this.gboxDisplay.Size = new System.Drawing.Size(225, 105);
            this.gboxDisplay.TabIndex = 0;
            this.gboxDisplay.TabStop = false;
            this.gboxDisplay.Text = "Display";
            // 
            // cboxGamma
            // 
            this.cboxGamma.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboxGamma.FormattingEnabled = true;
            this.cboxGamma.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4",
            "5"});
            this.cboxGamma.Location = new System.Drawing.Point(90, 73);
            this.cboxGamma.Name = "cboxGamma";
            this.cboxGamma.Size = new System.Drawing.Size(120, 22);
            this.cboxGamma.TabIndex = 5;
            // 
            // lblGamma
            // 
            this.lblGamma.AutoSize = true;
            this.lblGamma.Location = new System.Drawing.Point(10, 76);
            this.lblGamma.Name = "lblGamma";
            this.lblGamma.Size = new System.Drawing.Size(46, 14);
            this.lblGamma.TabIndex = 4;
            this.lblGamma.Text = "Gamma";
            // 
            // cboxFrequency
            // 
            this.cboxFrequency.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboxFrequency.FormattingEnabled = true;
            this.cboxFrequency.Location = new System.Drawing.Point(90, 46);
            this.cboxFrequency.Name = "cboxFrequency";
            this.cboxFrequency.Size = new System.Drawing.Size(120, 22);
            this.cboxFrequency.TabIndex = 3;
            // 
            // lblFrequency
            // 
            this.lblFrequency.AutoSize = true;
            this.lblFrequency.Location = new System.Drawing.Point(10, 49);
            this.lblFrequency.Name = "lblFrequency";
            this.lblFrequency.Size = new System.Drawing.Size(62, 14);
            this.lblFrequency.TabIndex = 2;
            this.lblFrequency.Text = "Frequency";
            // 
            // cboxResolution
            // 
            this.cboxResolution.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboxResolution.FormattingEnabled = true;
            this.cboxResolution.Location = new System.Drawing.Point(90, 19);
            this.cboxResolution.Name = "cboxResolution";
            this.cboxResolution.Size = new System.Drawing.Size(120, 22);
            this.cboxResolution.TabIndex = 1;
            this.cboxResolution.SelectedIndexChanged += new System.EventHandler(this.cboxResolution_SelectedIndexChanged);
            // 
            // lblResolution
            // 
            this.lblResolution.AutoSize = true;
            this.lblResolution.Location = new System.Drawing.Point(10, 22);
            this.lblResolution.Name = "lblResolution";
            this.lblResolution.Size = new System.Drawing.Size(63, 14);
            this.lblResolution.TabIndex = 0;
            this.lblResolution.Text = "Resolution";
            // 
            // gboxSound
            // 
            this.gboxSound.Controls.Add(this.tboxSFX);
            this.gboxSound.Controls.Add(this.trbarSFX);
            this.gboxSound.Controls.Add(this.lblSFX);
            this.gboxSound.Controls.Add(this.tboxBGM);
            this.gboxSound.Controls.Add(this.trbarBGM);
            this.gboxSound.Controls.Add(this.lblBGM);
            this.gboxSound.Location = new System.Drawing.Point(12, 125);
            this.gboxSound.Name = "gboxSound";
            this.gboxSound.Size = new System.Drawing.Size(225, 80);
            this.gboxSound.TabIndex = 1;
            this.gboxSound.TabStop = false;
            this.gboxSound.Text = "Sound";
            // 
            // tboxSFX
            // 
            this.tboxSFX.Enabled = false;
            this.tboxSFX.Location = new System.Drawing.Point(50, 42);
            this.tboxSFX.Name = "tboxSFX";
            this.tboxSFX.Size = new System.Drawing.Size(25, 22);
            this.tboxSFX.TabIndex = 5;
            this.tboxSFX.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // trbarSFX
            // 
            this.trbarSFX.AutoSize = false;
            this.trbarSFX.Location = new System.Drawing.Point(85, 42);
            this.trbarSFX.Maximum = 50;
            this.trbarSFX.Name = "trbarSFX";
            this.trbarSFX.Size = new System.Drawing.Size(130, 20);
            this.trbarSFX.TabIndex = 4;
            this.trbarSFX.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trbarSFX.Scroll += new System.EventHandler(this.trbarSFX_Scroll);
            // 
            // lblSFX
            // 
            this.lblSFX.AutoSize = true;
            this.lblSFX.Location = new System.Drawing.Point(10, 45);
            this.lblSFX.Name = "lblSFX";
            this.lblSFX.Size = new System.Drawing.Size(27, 14);
            this.lblSFX.TabIndex = 3;
            this.lblSFX.Text = "SFX";
            // 
            // tboxBGM
            // 
            this.tboxBGM.Enabled = false;
            this.tboxBGM.Location = new System.Drawing.Point(50, 17);
            this.tboxBGM.Name = "tboxBGM";
            this.tboxBGM.Size = new System.Drawing.Size(25, 22);
            this.tboxBGM.TabIndex = 2;
            this.tboxBGM.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // trbarBGM
            // 
            this.trbarBGM.AutoSize = false;
            this.trbarBGM.Location = new System.Drawing.Point(85, 17);
            this.trbarBGM.Maximum = 1000;
            this.trbarBGM.Name = "trbarBGM";
            this.trbarBGM.Size = new System.Drawing.Size(130, 20);
            this.trbarBGM.TabIndex = 1;
            this.trbarBGM.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trbarBGM.Scroll += new System.EventHandler(this.trbarBGM_Scroll);
            // 
            // lblBGM
            // 
            this.lblBGM.AutoSize = true;
            this.lblBGM.Location = new System.Drawing.Point(10, 20);
            this.lblBGM.Name = "lblBGM";
            this.lblBGM.Size = new System.Drawing.Size(31, 14);
            this.lblBGM.TabIndex = 0;
            this.lblBGM.Text = "BGM";
            // 
            // chboxUseSoftCursor
            // 
            this.chboxUseSoftCursor.AutoSize = true;
            this.chboxUseSoftCursor.Location = new System.Drawing.Point(22, 220);
            this.chboxUseSoftCursor.Name = "chboxUseSoftCursor";
            this.chboxUseSoftCursor.Size = new System.Drawing.Size(130, 18);
            this.chboxUseSoftCursor.TabIndex = 2;
            this.chboxUseSoftCursor.Text = "Use Software cursor";
            this.chboxUseSoftCursor.UseVisualStyleBackColor = true;
            // 
            // cboxLocale
            // 
            this.cboxLocale.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboxLocale.FormattingEnabled = true;
            this.cboxLocale.Location = new System.Drawing.Point(135, 245);
            this.cboxLocale.Name = "cboxLocale";
            this.cboxLocale.Size = new System.Drawing.Size(225, 22);
            this.cboxLocale.TabIndex = 3;
            // 
            // btnOK
            // 
            this.btnOK.Location = new System.Drawing.Point(150, 290);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(85, 25);
            this.btnOK.TabIndex = 4;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // btnCANCEL
            // 
            this.btnCANCEL.Location = new System.Drawing.Point(245, 290);
            this.btnCANCEL.Name = "btnCANCEL";
            this.btnCANCEL.Size = new System.Drawing.Size(85, 25);
            this.btnCANCEL.TabIndex = 5;
            this.btnCANCEL.Text = "CANCEL";
            this.btnCANCEL.UseVisualStyleBackColor = true;
            this.btnCANCEL.Click += new System.EventHandler(this.btnCANCEL_Click);
            // 
            // gboxWindowMode
            // 
            this.gboxWindowMode.Controls.Add(this.rbtnWindowMode);
            this.gboxWindowMode.Controls.Add(this.rbtnFullscreen);
            this.gboxWindowMode.Location = new System.Drawing.Point(245, 12);
            this.gboxWindowMode.Name = "gboxWindowMode";
            this.gboxWindowMode.Size = new System.Drawing.Size(235, 55);
            this.gboxWindowMode.TabIndex = 6;
            this.gboxWindowMode.TabStop = false;
            this.gboxWindowMode.Text = "Window Mode";
            // 
            // rbtnWindowMode
            // 
            this.rbtnWindowMode.AutoSize = true;
            this.rbtnWindowMode.Location = new System.Drawing.Point(10, 20);
            this.rbtnWindowMode.Name = "rbtnWindowMode";
            this.rbtnWindowMode.Size = new System.Drawing.Size(102, 17);
            this.rbtnWindowMode.TabIndex = 1;
            this.rbtnWindowMode.Text = "Window Mode";
            this.rbtnWindowMode.UseVisualStyleBackColor = true;
            // 
            // rbtnFullscreen
            // 
            this.rbtnFullscreen.AutoSize = true;
            this.rbtnFullscreen.Checked = true;
            this.rbtnFullscreen.Location = new System.Drawing.Point(115, 20);
            this.rbtnFullscreen.Name = "rbtnFullscreen";
            this.rbtnFullscreen.Size = new System.Drawing.Size(111, 17);
            this.rbtnFullscreen.TabIndex = 0;
            this.rbtnFullscreen.TabStop = true;
            this.rbtnFullscreen.Text = "Fullscreen Mode";
            this.rbtnFullscreen.UseVisualStyleBackColor = true;
            // 
            // gboxIMEMode
            // 
            this.gboxIMEMode.Controls.Add(this.rbtnExternalIME);
            this.gboxIMEMode.Controls.Add(this.rbtnGameIME);
            this.gboxIMEMode.Location = new System.Drawing.Point(245, 73);
            this.gboxIMEMode.Name = "gboxIMEMode";
            this.gboxIMEMode.Size = new System.Drawing.Size(235, 45);
            this.gboxIMEMode.TabIndex = 7;
            this.gboxIMEMode.TabStop = false;
            this.gboxIMEMode.Text = "IME Mode";
            // 
            // rbtnExternalIME
            // 
            this.rbtnExternalIME.AutoSize = true;
            this.rbtnExternalIME.Location = new System.Drawing.Point(115, 18);
            this.rbtnExternalIME.Name = "rbtnExternalIME";
            this.rbtnExternalIME.Size = new System.Drawing.Size(95, 17);
            this.rbtnExternalIME.TabIndex = 1;
            this.rbtnExternalIME.Text = "External IME";
            this.rbtnExternalIME.UseVisualStyleBackColor = true;
            // 
            // rbtnGameIME
            // 
            this.rbtnGameIME.AutoSize = true;
            this.rbtnGameIME.Checked = true;
            this.rbtnGameIME.Location = new System.Drawing.Point(10, 18);
            this.rbtnGameIME.Name = "rbtnGameIME";
            this.rbtnGameIME.Size = new System.Drawing.Size(79, 17);
            this.rbtnGameIME.TabIndex = 0;
            this.rbtnGameIME.TabStop = true;
            this.rbtnGameIME.Text = "GAME IME";
            this.rbtnGameIME.UseVisualStyleBackColor = true;
            // 
            // gboxGFX
            // 
            this.gboxGFX.Controls.Add(this.cboxAntiAlias);
            this.gboxGFX.Controls.Add(this.lblAntiAlias);
            this.gboxGFX.Controls.Add(this.cboxShadow);
            this.gboxGFX.Controls.Add(this.lblShadow);
            this.gboxGFX.Controls.Add(this.cboxTNL);
            this.gboxGFX.Controls.Add(this.lblTNL);
            this.gboxGFX.Controls.Add(this.cboxFog);
            this.gboxGFX.Controls.Add(this.lblFog);
            this.gboxGFX.Location = new System.Drawing.Point(245, 125);
            this.gboxGFX.Name = "gboxGFX";
            this.gboxGFX.Size = new System.Drawing.Size(235, 115);
            this.gboxGFX.TabIndex = 8;
            this.gboxGFX.TabStop = false;
            this.gboxGFX.Text = "GFX";
            // 
            // cboxAntiAlias
            // 
            this.cboxAntiAlias.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboxAntiAlias.FormattingEnabled = true;
            this.cboxAntiAlias.Location = new System.Drawing.Point(85, 87);
            this.cboxAntiAlias.Name = "cboxAntiAlias";
            this.cboxAntiAlias.Size = new System.Drawing.Size(140, 22);
            this.cboxAntiAlias.TabIndex = 7;
            // 
            // lblAntiAlias
            // 
            this.lblAntiAlias.AutoSize = true;
            this.lblAntiAlias.Location = new System.Drawing.Point(10, 90);
            this.lblAntiAlias.Name = "lblAntiAlias";
            this.lblAntiAlias.Size = new System.Drawing.Size(65, 14);
            this.lblAntiAlias.TabIndex = 6;
            this.lblAntiAlias.Text = "Antialiasing";
            // 
            // cboxShadow
            // 
            this.cboxShadow.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboxShadow.FormattingEnabled = true;
            this.cboxShadow.Location = new System.Drawing.Point(85, 63);
            this.cboxShadow.Name = "cboxShadow";
            this.cboxShadow.Size = new System.Drawing.Size(140, 22);
            this.cboxShadow.TabIndex = 5;
            // 
            // lblShadow
            // 
            this.lblShadow.AutoSize = true;
            this.lblShadow.Location = new System.Drawing.Point(10, 66);
            this.lblShadow.Name = "lblShadow";
            this.lblShadow.Size = new System.Drawing.Size(48, 14);
            this.lblShadow.TabIndex = 4;
            this.lblShadow.Text = "Shadow";
            // 
            // cboxTNL
            // 
            this.cboxTNL.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboxTNL.FormattingEnabled = true;
            this.cboxTNL.Location = new System.Drawing.Point(85, 39);
            this.cboxTNL.Name = "cboxTNL";
            this.cboxTNL.Size = new System.Drawing.Size(140, 22);
            this.cboxTNL.TabIndex = 3;
            // 
            // lblTNL
            // 
            this.lblTNL.AutoSize = true;
            this.lblTNL.Location = new System.Drawing.Point(10, 42);
            this.lblTNL.Name = "lblTNL";
            this.lblTNL.Size = new System.Drawing.Size(27, 14);
            this.lblTNL.TabIndex = 2;
            this.lblTNL.Text = "TNL";
            // 
            // cboxFog
            // 
            this.cboxFog.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboxFog.FormattingEnabled = true;
            this.cboxFog.Location = new System.Drawing.Point(85, 15);
            this.cboxFog.Name = "cboxFog";
            this.cboxFog.Size = new System.Drawing.Size(140, 22);
            this.cboxFog.TabIndex = 1;
            // 
            // lblFog
            // 
            this.lblFog.AutoSize = true;
            this.lblFog.Location = new System.Drawing.Point(10, 18);
            this.lblFog.Name = "lblFog";
            this.lblFog.Size = new System.Drawing.Size(29, 14);
            this.lblFog.TabIndex = 0;
            this.lblFog.Text = "FOG";
            // 
            // FormConfig
            // 
            this.AcceptButton = this.btnOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 14F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(490, 330);
            this.Controls.Add(this.gboxGFX);
            this.Controls.Add(this.gboxIMEMode);
            this.Controls.Add(this.gboxWindowMode);
            this.Controls.Add(this.btnCANCEL);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.cboxLocale);
            this.Controls.Add(this.chboxUseSoftCursor);
            this.Controls.Add(this.gboxSound);
            this.Controls.Add(this.gboxDisplay);
            this.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "FormConfig";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "CONFIG";
            this.Load += new System.EventHandler(this.FormConfig_Load);
            this.gboxDisplay.ResumeLayout(false);
            this.gboxDisplay.PerformLayout();
            this.gboxSound.ResumeLayout(false);
            this.gboxSound.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trbarSFX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trbarBGM)).EndInit();
            this.gboxWindowMode.ResumeLayout(false);
            this.gboxWindowMode.PerformLayout();
            this.gboxIMEMode.ResumeLayout(false);
            this.gboxIMEMode.PerformLayout();
            this.gboxGFX.ResumeLayout(false);
            this.gboxGFX.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox gboxDisplay;
        private System.Windows.Forms.ComboBox cboxGamma;
        private System.Windows.Forms.Label lblGamma;
        private System.Windows.Forms.ComboBox cboxFrequency;
        private System.Windows.Forms.Label lblFrequency;
        private System.Windows.Forms.ComboBox cboxResolution;
        private System.Windows.Forms.Label lblResolution;
        private System.Windows.Forms.GroupBox gboxSound;
        private System.Windows.Forms.TextBox tboxSFX;
        private System.Windows.Forms.TrackBar trbarSFX;
        private System.Windows.Forms.Label lblSFX;
        private System.Windows.Forms.TextBox tboxBGM;
        private System.Windows.Forms.TrackBar trbarBGM;
        private System.Windows.Forms.Label lblBGM;
        private System.Windows.Forms.CheckBox chboxUseSoftCursor;
        private System.Windows.Forms.ComboBox cboxLocale;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Button btnCANCEL;
        private System.Windows.Forms.GroupBox gboxWindowMode;
        private System.Windows.Forms.RadioButton rbtnWindowMode;
        private System.Windows.Forms.RadioButton rbtnFullscreen;
        private System.Windows.Forms.GroupBox gboxIMEMode;
        private System.Windows.Forms.RadioButton rbtnExternalIME;
        private System.Windows.Forms.RadioButton rbtnGameIME;
        private System.Windows.Forms.GroupBox gboxGFX;
        private System.Windows.Forms.ComboBox cboxAntiAlias;
        private System.Windows.Forms.Label lblAntiAlias;
        private System.Windows.Forms.ComboBox cboxShadow;
        private System.Windows.Forms.Label lblShadow;
        private System.Windows.Forms.ComboBox cboxTNL;
        private System.Windows.Forms.Label lblTNL;
        private System.Windows.Forms.ComboBox cboxFog;
        private System.Windows.Forms.Label lblFog;
    }
}