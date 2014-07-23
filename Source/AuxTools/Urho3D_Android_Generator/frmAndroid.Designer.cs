namespace Urho3D_Android_Generator
{
    partial class frmAndroid
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmAndroid));
            this.btnCheckNDK = new System.Windows.Forms.Button();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.radStaticLib = new System.Windows.Forms.RadioButton();
            this.radDllBuild = new System.Windows.Forms.RadioButton();
            this.radCommonBuild = new System.Windows.Forms.RadioButton();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.chkLuaJit = new System.Windows.Forms.CheckBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.chkAngleScript = new System.Windows.Forms.CheckBox();
            this.chkLua = new System.Windows.Forms.CheckBox();
            this.chkTools = new System.Windows.Forms.CheckBox();
            this.chkSamples = new System.Windows.Forms.CheckBox();
            this.btnGenerate = new System.Windows.Forms.Button();
            this.btnCompile = new System.Windows.Forms.Button();
            this.btnGenDebugApk = new System.Windows.Forms.Button();
            this.btnGenReleaseApk = new System.Windows.Forms.Button();
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.linkLabel1 = new System.Windows.Forms.LinkLabel();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox4.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnCheckNDK
            // 
            this.btnCheckNDK.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnCheckNDK.Location = new System.Drawing.Point(12, 13);
            this.btnCheckNDK.Name = "btnCheckNDK";
            this.btnCheckNDK.Size = new System.Drawing.Size(103, 23);
            this.btnCheckNDK.TabIndex = 1;
            this.btnCheckNDK.Text = "环境检测";
            this.btnCheckNDK.UseVisualStyleBackColor = true;
            this.btnCheckNDK.Click += new System.EventHandler(this.btnCheckNDK_Click);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.radStaticLib);
            this.groupBox4.Controls.Add(this.listBox1);
            this.groupBox4.Controls.Add(this.radDllBuild);
            this.groupBox4.Controls.Add(this.radCommonBuild);
            this.groupBox4.Location = new System.Drawing.Point(11, 198);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(426, 93);
            this.groupBox4.TabIndex = 7;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "编译选项";
            // 
            // radStaticLib
            // 
            this.radStaticLib.AutoSize = true;
            this.radStaticLib.Cursor = System.Windows.Forms.Cursors.Hand;
            this.radStaticLib.Location = new System.Drawing.Point(18, 67);
            this.radStaticLib.Name = "radStaticLib";
            this.radStaticLib.Size = new System.Drawing.Size(263, 16);
            this.radStaticLib.TabIndex = 2;
            this.radStaticLib.Text = "Urho3D 静态库 (-DURHO3D_LIB_TYPE=STATIC)";
            this.radStaticLib.UseVisualStyleBackColor = true;
            // 
            // radDllBuild
            // 
            this.radDllBuild.AutoSize = true;
            this.radDllBuild.Cursor = System.Windows.Forms.Cursors.Hand;
            this.radDllBuild.Location = new System.Drawing.Point(18, 45);
            this.radDllBuild.Name = "radDllBuild";
            this.radDllBuild.Size = new System.Drawing.Size(263, 16);
            this.radDllBuild.TabIndex = 1;
            this.radDllBuild.Text = "Urho3D 动态库 (-DURHO3D_LIB_TYPE=SHARED)";
            this.radDllBuild.UseVisualStyleBackColor = true;
            // 
            // radCommonBuild
            // 
            this.radCommonBuild.AutoSize = true;
            this.radCommonBuild.Checked = true;
            this.radCommonBuild.Cursor = System.Windows.Forms.Cursors.Hand;
            this.radCommonBuild.Location = new System.Drawing.Point(17, 22);
            this.radCommonBuild.Name = "radCommonBuild";
            this.radCommonBuild.Size = new System.Drawing.Size(113, 16);
            this.radCommonBuild.TabIndex = 0;
            this.radCommonBuild.TabStop = true;
            this.radCommonBuild.Text = "普通方式 (默认)";
            this.radCommonBuild.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.chkLuaJit);
            this.groupBox2.Controls.Add(this.checkBox1);
            this.groupBox2.Controls.Add(this.chkAngleScript);
            this.groupBox2.Controls.Add(this.chkLua);
            this.groupBox2.Controls.Add(this.chkTools);
            this.groupBox2.Controls.Add(this.chkSamples);
            this.groupBox2.Location = new System.Drawing.Point(12, 57);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(426, 135);
            this.groupBox2.TabIndex = 6;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "构建选项";
            // 
            // chkLuaJit
            // 
            this.chkLuaJit.AutoSize = true;
            this.chkLuaJit.Cursor = System.Windows.Forms.Cursors.Hand;
            this.chkLuaJit.Enabled = false;
            this.chkLuaJit.Location = new System.Drawing.Point(17, 64);
            this.chkLuaJit.Name = "chkLuaJit";
            this.chkLuaJit.Size = new System.Drawing.Size(204, 16);
            this.chkLuaJit.TabIndex = 5;
            this.chkLuaJit.Text = "LuaJIT支持 (-DURHO3D_LUAJIT=1)";
            this.chkLuaJit.UseVisualStyleBackColor = true;
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Checked = true;
            this.checkBox1.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox1.Cursor = System.Windows.Forms.Cursors.Hand;
            this.checkBox1.Enabled = false;
            this.checkBox1.Location = new System.Drawing.Point(17, 20);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(60, 16);
            this.checkBox1.TabIndex = 4;
            this.checkBox1.Text = "Engine";
            this.checkBox1.UseVisualStyleBackColor = true;
            // 
            // chkAngleScript
            // 
            this.chkAngleScript.AutoSize = true;
            this.chkAngleScript.Cursor = System.Windows.Forms.Cursors.Hand;
            this.chkAngleScript.Location = new System.Drawing.Point(16, 113);
            this.chkAngleScript.Name = "chkAngleScript";
            this.chkAngleScript.Size = new System.Drawing.Size(264, 16);
            this.chkAngleScript.TabIndex = 3;
            this.chkAngleScript.Text = "AngleScript支持 (-DURHO3D_ANGELSCRIPT=1)";
            this.chkAngleScript.UseVisualStyleBackColor = true;
            // 
            // chkLua
            // 
            this.chkLua.AutoSize = true;
            this.chkLua.Cursor = System.Windows.Forms.Cursors.Hand;
            this.chkLua.Location = new System.Drawing.Point(17, 42);
            this.chkLua.Name = "chkLua";
            this.chkLua.Size = new System.Drawing.Size(168, 16);
            this.chkLua.TabIndex = 2;
            this.chkLua.Text = "Lua支持 (-DURHO3D_LUA=1)";
            this.chkLua.UseVisualStyleBackColor = true;
            // 
            // chkTools
            // 
            this.chkTools.AutoSize = true;
            this.chkTools.Checked = true;
            this.chkTools.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkTools.Cursor = System.Windows.Forms.Cursors.Hand;
            this.chkTools.Enabled = false;
            this.chkTools.Location = new System.Drawing.Point(104, 20);
            this.chkTools.Name = "chkTools";
            this.chkTools.Size = new System.Drawing.Size(54, 16);
            this.chkTools.TabIndex = 1;
            this.chkTools.Text = "Tools";
            this.chkTools.UseVisualStyleBackColor = true;
            // 
            // chkSamples
            // 
            this.chkSamples.AutoSize = true;
            this.chkSamples.Cursor = System.Windows.Forms.Cursors.Hand;
            this.chkSamples.Location = new System.Drawing.Point(17, 91);
            this.chkSamples.Name = "chkSamples";
            this.chkSamples.Size = new System.Drawing.Size(192, 16);
            this.chkSamples.TabIndex = 0;
            this.chkSamples.Text = "Samples (-DURHO3D_SAMPLES=1)";
            this.chkSamples.UseVisualStyleBackColor = true;
            // 
            // btnGenerate
            // 
            this.btnGenerate.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnGenerate.Location = new System.Drawing.Point(177, 297);
            this.btnGenerate.Name = "btnGenerate";
            this.btnGenerate.Size = new System.Drawing.Size(115, 23);
            this.btnGenerate.TabIndex = 8;
            this.btnGenerate.Text = "1.生成Android工程";
            this.btnGenerate.UseVisualStyleBackColor = true;
            this.btnGenerate.Click += new System.EventHandler(this.btnGenerate_Click);
            // 
            // btnCompile
            // 
            this.btnCompile.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnCompile.Location = new System.Drawing.Point(298, 297);
            this.btnCompile.Name = "btnCompile";
            this.btnCompile.Size = new System.Drawing.Size(112, 23);
            this.btnCompile.TabIndex = 9;
            this.btnCompile.Text = "2.编译工程";
            this.btnCompile.UseVisualStyleBackColor = true;
            this.btnCompile.Click += new System.EventHandler(this.btnCompile_Click);
            // 
            // btnGenDebugApk
            // 
            this.btnGenDebugApk.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnGenDebugApk.Location = new System.Drawing.Point(178, 326);
            this.btnGenDebugApk.Name = "btnGenDebugApk";
            this.btnGenDebugApk.Size = new System.Drawing.Size(114, 23);
            this.btnGenDebugApk.TabIndex = 10;
            this.btnGenDebugApk.Text = "3.生成Debug apk";
            this.btnGenDebugApk.UseVisualStyleBackColor = true;
            this.btnGenDebugApk.Click += new System.EventHandler(this.btnGenDebugApk_Click);
            // 
            // btnGenReleaseApk
            // 
            this.btnGenReleaseApk.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnGenReleaseApk.Location = new System.Drawing.Point(298, 326);
            this.btnGenReleaseApk.Name = "btnGenReleaseApk";
            this.btnGenReleaseApk.Size = new System.Drawing.Size(112, 23);
            this.btnGenReleaseApk.TabIndex = 11;
            this.btnGenReleaseApk.Text = "3.生成Release apk";
            this.btnGenReleaseApk.UseVisualStyleBackColor = true;
            this.btnGenReleaseApk.Click += new System.EventHandler(this.btnGenReleaseApk_Click);
            // 
            // listBox1
            // 
            this.listBox1.FormattingEnabled = true;
            this.listBox1.ItemHeight = 12;
            this.listBox1.Location = new System.Drawing.Point(0, 25);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(179, 100);
            this.listBox1.TabIndex = 12;
            this.listBox1.Visible = false;
            // 
            // linkLabel1
            // 
            this.linkLabel1.AutoSize = true;
            this.linkLabel1.Location = new System.Drawing.Point(26, 326);
            this.linkLabel1.Name = "linkLabel1";
            this.linkLabel1.Size = new System.Drawing.Size(89, 12);
            this.linkLabel1.TabIndex = 13;
            this.linkLabel1.TabStop = true;
            this.linkLabel1.Text = "Urho3D中文论坛";
            this.linkLabel1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabel1_LinkClicked);
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(138, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(299, 41);
            this.label1.TabIndex = 14;
            this.label1.Text = "适用于Urho3D V1.3.1,使用前请配置好Android NDK,SDK,ant，构建工具会对这些必须的工具做检测";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(452, 356);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.linkLabel1);
            this.Controls.Add(this.btnGenReleaseApk);
            this.Controls.Add(this.btnGenDebugApk);
            this.Controls.Add(this.btnCompile);
            this.Controls.Add(this.btnGenerate);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.btnCheckNDK);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "Urho3D Android Generator";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnCheckNDK;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.RadioButton radStaticLib;
        private System.Windows.Forms.RadioButton radDllBuild;
        private System.Windows.Forms.RadioButton radCommonBuild;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.CheckBox chkLuaJit;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.CheckBox chkAngleScript;
        private System.Windows.Forms.CheckBox chkLua;
        private System.Windows.Forms.CheckBox chkTools;
        private System.Windows.Forms.CheckBox chkSamples;
        private System.Windows.Forms.Button btnGenerate;
        private System.Windows.Forms.Button btnCompile;
        private System.Windows.Forms.Button btnGenDebugApk;
        private System.Windows.Forms.Button btnGenReleaseApk;
        private System.Windows.Forms.ListBox listBox1;
        private System.Windows.Forms.LinkLabel linkLabel1;
        private System.Windows.Forms.Label label1;
    }
}

