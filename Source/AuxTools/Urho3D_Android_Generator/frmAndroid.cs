using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;
using System.Management;

namespace Urho3D_Android_Generator
{
    public partial class frmAndroid : Form
    {
        public frmAndroid()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //PnPEntityInfo[] devs = USB.AllPnPEntities;
            //PnPEntityInfo[] infos = USB.AllUsbDevices;
        }

        private bool CheckEnv()
        {
            //make.exe是否注册到path
            if (EnvCheckHelper.IsPathExistExe("make.exe") == false)
            {
                MessageBox.Show("未检测到交叉编译工具NDK make位置！请正确安装NDK，并将make.exe位置放在path中！");
                return false;
            }

            //检测Android SDK
            if (EnvCheckHelper.IsExistEnvVar("ANDROID_NDK") == false)
            {
                MessageBox.Show("未检测到 ANDROID_NDK 环境变量，请设置好NDK的路径！");
                return false;
            }

            //检测ant
            if (EnvCheckHelper.IsPathExistExe("ant.bat") == false)
            {
                MessageBox.Show("未正确安装ant，请正确安装ant，并将ant.bat所在路径放在path中！");
                return false;
            }

            return true;
        }

        private void btnCheckNDK_Click(object sender, EventArgs e)
        {
            if (CheckEnv() == true)
            {
                MessageBox.Show("经检测环境变量已设置成功！");
            }
        }

        private void btnGenerate_Click(object sender, EventArgs e)
        {
            if (CheckEnv() == false)
                return;

            string batName = "cmake_android.bat";

            string strOption = " -DURHO3D_MKLINK=1";
            if (chkLua.Checked == true)
            {
                strOption += " -DURHO3D_LUA=1";
            }
            if (chkLuaJit.Checked == true)
            {
                strOption += " -DURHO3D_LUAJIT=1";
            }
            if (chkSamples.Checked == true)
            {
                strOption += " -DURHO3D_SAMPLES=1";
            }
            if (chkAngleScript.Checked == true)
            {
				strOption += " -DURHO3D_ANGELSCRIPT=1";
            }

            string strLibType = ""; //默认
            if (radDllBuild.Checked == true)
            {
                strLibType += " -DURHO3D_LIB_TYPE=SHARED";
            }
            if (radStaticLib.Checked == true)
            {
                strLibType += " -DURHO3D_LIB_TYPE=STATIC";
            }

            string strParams = strOption + strLibType;
            Process pro = Process.Start(batName, strParams);

            //2.调用android update
            Process.Start("android", "update project -p ./android-Build -t 1");   //todo:需要列举android设备？
        }

        private void btnCompile_Click(object sender, EventArgs e)
        {
            if (CheckEnv() == false)
                return;

            string strParam = "-j" + Environment.ProcessorCount.ToString(); //cpu核数
            Process.Start("make", strParam);
        }

        private void btnGenDebugApk_Click(object sender, EventArgs e)
        {
            if (CheckEnv() == false)
                return;

            Process.Start("ant", "debug");
        }

        private void btnGenReleaseApk_Click(object sender, EventArgs e)
        {
            if (CheckEnv() == false)
                return;

            Process.Start("ant", "release");
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("http://www.urho3d.com");
        }
    }
}
