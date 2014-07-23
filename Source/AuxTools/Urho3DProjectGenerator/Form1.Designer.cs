namespace Urho3DGenerator
{
	partial class Form1
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.radOpenGL2 = new System.Windows.Forms.RadioButton();
            this.radDirectX9 = new System.Windows.Forms.RadioButton();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.chkLuaJit = new System.Windows.Forms.CheckBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.chkAngleScript = new System.Windows.Forms.CheckBox();
            this.chkLua = new System.Windows.Forms.CheckBox();
            this.chkTools = new System.Windows.Forms.CheckBox();
            this.chkSamples = new System.Windows.Forms.CheckBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.radVS2013 = new System.Windows.Forms.RadioButton();
            this.radVS2012 = new System.Windows.Forms.RadioButton();
            this.radVS2010 = new System.Windows.Forms.RadioButton();
            this.radVS2008 = new System.Windows.Forms.RadioButton();
            this.btnGen = new System.Windows.Forms.Button();
            this.btnClean = new System.Windows.Forms.Button();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.radStaticLib = new System.Windows.Forms.RadioButton();
            this.radDllBuild = new System.Windows.Forms.RadioButton();
            this.radCommonBuild = new System.Windows.Forms.RadioButton();
            this.linkLabel1 = new System.Windows.Forms.LinkLabel();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.radOpenGL2);
            this.groupBox1.Controls.Add(this.radDirectX9);
            this.groupBox1.Location = new System.Drawing.Point(13, 47);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(426, 69);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "渲染底层";
            // 
            // radOpenGL2
            // 
            this.radOpenGL2.AutoSize = true;
            this.radOpenGL2.Checked = true;
            this.radOpenGL2.Cursor = System.Windows.Forms.Cursors.Hand;
            this.radOpenGL2.Location = new System.Drawing.Point(17, 43);
            this.radOpenGL2.Name = "radOpenGL2";
            this.radOpenGL2.Size = new System.Drawing.Size(203, 16);
            this.radOpenGL2.TabIndex = 1;
            this.radOpenGL2.TabStop = true;
            this.radOpenGL2.Text = "OpenGL 2.0 (-DURHO3D_OPENGL=1)";
            this.radOpenGL2.UseVisualStyleBackColor = true;
            // 
            // radDirectX9
            // 
            this.radDirectX9.AutoSize = true;
            this.radDirectX9.Cursor = System.Windows.Forms.Cursors.Hand;
            this.radDirectX9.Location = new System.Drawing.Point(17, 21);
            this.radDirectX9.Name = "radDirectX9";
            this.radDirectX9.Size = new System.Drawing.Size(131, 16);
            this.radDirectX9.TabIndex = 0;
            this.radDirectX9.Text = "DirectX 9.0 (默认)";
            this.radDirectX9.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.chkLuaJit);
            this.groupBox2.Controls.Add(this.checkBox1);
            this.groupBox2.Controls.Add(this.chkAngleScript);
            this.groupBox2.Controls.Add(this.chkLua);
            this.groupBox2.Controls.Add(this.chkTools);
            this.groupBox2.Controls.Add(this.chkSamples);
            this.groupBox2.Location = new System.Drawing.Point(13, 124);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(426, 135);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "构建选项";
            // 
            // chkLuaJit
            // 
            this.chkLuaJit.AutoSize = true;
            this.chkLuaJit.Checked = true;
            this.chkLuaJit.CheckState = System.Windows.Forms.CheckState.Checked;
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
            this.chkAngleScript.Checked = true;
            this.chkAngleScript.CheckState = System.Windows.Forms.CheckState.Checked;
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
            this.chkLua.Checked = true;
            this.chkLua.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkLua.Cursor = System.Windows.Forms.Cursors.Hand;
            this.chkLua.Location = new System.Drawing.Point(17, 42);
            this.chkLua.Name = "chkLua";
            this.chkLua.Size = new System.Drawing.Size(168, 16);
            this.chkLua.TabIndex = 2;
            this.chkLua.Text = "Lua支持 (-DURHO3D_LUA=1)";
            this.chkLua.UseVisualStyleBackColor = true;
            this.chkLua.CheckedChanged += new System.EventHandler(this.chkLua_CheckedChanged);
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
            this.chkSamples.Checked = true;
            this.chkSamples.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkSamples.Cursor = System.Windows.Forms.Cursors.Hand;
            this.chkSamples.Location = new System.Drawing.Point(17, 91);
            this.chkSamples.Name = "chkSamples";
            this.chkSamples.Size = new System.Drawing.Size(192, 16);
            this.chkSamples.TabIndex = 0;
            this.chkSamples.Text = "Samples (-DURHO3D_SAMPLES=1)";
            this.chkSamples.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.radVS2013);
            this.groupBox3.Controls.Add(this.radVS2012);
            this.groupBox3.Controls.Add(this.radVS2010);
            this.groupBox3.Controls.Add(this.radVS2008);
            this.groupBox3.Location = new System.Drawing.Point(12, 364);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(426, 99);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "目标平台";
            // 
            // radVS2013
            // 
            this.radVS2013.AutoSize = true;
            this.radVS2013.Cursor = System.Windows.Forms.Cursors.Hand;
            this.radVS2013.Location = new System.Drawing.Point(188, 20);
            this.radVS2013.Name = "radVS2013";
            this.radVS2013.Size = new System.Drawing.Size(131, 16);
            this.radVS2013.TabIndex = 3;
            this.radVS2013.TabStop = true;
            this.radVS2013.Text = "Visual Studio 2013";
            this.radVS2013.UseVisualStyleBackColor = true;
            // 
            // radVS2012
            // 
            this.radVS2012.AutoSize = true;
            this.radVS2012.Cursor = System.Windows.Forms.Cursors.Hand;
            this.radVS2012.Location = new System.Drawing.Point(17, 67);
            this.radVS2012.Name = "radVS2012";
            this.radVS2012.Size = new System.Drawing.Size(131, 16);
            this.radVS2012.TabIndex = 2;
            this.radVS2012.Text = "Visual Studio 2012";
            this.radVS2012.UseVisualStyleBackColor = true;
            // 
            // radVS2010
            // 
            this.radVS2010.AutoSize = true;
            this.radVS2010.Cursor = System.Windows.Forms.Cursors.Hand;
            this.radVS2010.Location = new System.Drawing.Point(17, 44);
            this.radVS2010.Name = "radVS2010";
            this.radVS2010.Size = new System.Drawing.Size(131, 16);
            this.radVS2010.TabIndex = 1;
            this.radVS2010.Text = "Visual Studio 2010";
            this.radVS2010.UseVisualStyleBackColor = true;
            // 
            // radVS2008
            // 
            this.radVS2008.AutoSize = true;
            this.radVS2008.Checked = true;
            this.radVS2008.Cursor = System.Windows.Forms.Cursors.Hand;
            this.radVS2008.Location = new System.Drawing.Point(17, 21);
            this.radVS2008.Name = "radVS2008";
            this.radVS2008.Size = new System.Drawing.Size(131, 16);
            this.radVS2008.TabIndex = 0;
            this.radVS2008.TabStop = true;
            this.radVS2008.Text = "Visual Studio 2008";
            this.radVS2008.UseVisualStyleBackColor = true;
            // 
            // btnGen
            // 
            this.btnGen.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnGen.Location = new System.Drawing.Point(315, 476);
            this.btnGen.Name = "btnGen";
            this.btnGen.Size = new System.Drawing.Size(108, 23);
            this.btnGen.TabIndex = 3;
            this.btnGen.Text = "生成项目工程";
            this.btnGen.UseVisualStyleBackColor = true;
            this.btnGen.Click += new System.EventHandler(this.btnGen_Click);
            // 
            // btnClean
            // 
            this.btnClean.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnClean.Location = new System.Drawing.Point(191, 476);
            this.btnClean.Name = "btnClean";
            this.btnClean.Size = new System.Drawing.Size(118, 23);
            this.btnClean.TabIndex = 4;
            this.btnClean.Text = "清理已生成项目";
            this.btnClean.UseVisualStyleBackColor = true;
            this.btnClean.Click += new System.EventHandler(this.btnClean_Click);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.radStaticLib);
            this.groupBox4.Controls.Add(this.radDllBuild);
            this.groupBox4.Controls.Add(this.radCommonBuild);
            this.groupBox4.Location = new System.Drawing.Point(12, 265);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(426, 93);
            this.groupBox4.TabIndex = 5;
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
            this.radDllBuild.Checked = true;
            this.radDllBuild.Cursor = System.Windows.Forms.Cursors.Hand;
            this.radDllBuild.Location = new System.Drawing.Point(18, 45);
            this.radDllBuild.Name = "radDllBuild";
            this.radDllBuild.Size = new System.Drawing.Size(257, 16);
            this.radDllBuild.TabIndex = 1;
            this.radDllBuild.TabStop = true;
            this.radDllBuild.Text = "Urho3D dll库 (-DURHO3D_LIB_TYPE=SHARED)";
            this.radDllBuild.UseVisualStyleBackColor = true;
            // 
            // radCommonBuild
            // 
            this.radCommonBuild.AutoSize = true;
            this.radCommonBuild.Cursor = System.Windows.Forms.Cursors.Hand;
            this.radCommonBuild.Location = new System.Drawing.Point(17, 22);
            this.radCommonBuild.Name = "radCommonBuild";
            this.radCommonBuild.Size = new System.Drawing.Size(113, 16);
            this.radCommonBuild.TabIndex = 0;
            this.radCommonBuild.Text = "普通方式 (默认)";
            this.radCommonBuild.UseVisualStyleBackColor = true;
            // 
            // linkLabel1
            // 
            this.linkLabel1.AutoSize = true;
            this.linkLabel1.Location = new System.Drawing.Point(18, 481);
            this.linkLabel1.Name = "linkLabel1";
            this.linkLabel1.Size = new System.Drawing.Size(89, 12);
            this.linkLabel1.TabIndex = 6;
            this.linkLabel1.TabStop = true;
            this.linkLabel1.Text = "Urho3D中文论坛";
            this.linkLabel1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabel1_LinkClicked);
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(334, 35);
            this.label1.TabIndex = 7;
            this.label1.Text = "说明：适合Urho3D 1.3.1及以上版本，使用该工具前需要安装好CMake并设置好相应环境变量";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(465, 504);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.linkLabel1);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.btnClean);
            this.Controls.Add(this.btnGen);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "Urho3D项目生成工具 for Windows";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.RadioButton radOpenGL2;
		private System.Windows.Forms.RadioButton radDirectX9;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.CheckBox chkTools;
		private System.Windows.Forms.CheckBox chkSamples;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.Windows.Forms.RadioButton radVS2010;
		private System.Windows.Forms.RadioButton radVS2008;
		private System.Windows.Forms.Button btnGen;
		private System.Windows.Forms.Button btnClean;
		private System.Windows.Forms.RadioButton radVS2012;
		private System.Windows.Forms.RadioButton radVS2013;
		private System.Windows.Forms.GroupBox groupBox4;
		private System.Windows.Forms.RadioButton radCommonBuild;
		private System.Windows.Forms.RadioButton radDllBuild;
		private System.Windows.Forms.RadioButton radStaticLib;
		private System.Windows.Forms.CheckBox chkLua;
		private System.Windows.Forms.CheckBox chkAngleScript;
        private System.Windows.Forms.LinkLabel linkLabel1;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.CheckBox chkLuaJit;
        private System.Windows.Forms.Label label1;
	}
}

