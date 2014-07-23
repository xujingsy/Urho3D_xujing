using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace ExportTool
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        List<RadioButton> lstModelFormat = new List<RadioButton>();

        private void btnTranslate_Click(object sender, EventArgs e)
        {
            if (lvModelList.SelectedItems.Count > 0)
            {
                string fileName = lvModelList.SelectedItems[0].Text;
                string outName = fileName.Split('.')[1] + ".mdl";
                string args = "model " + fileName + " export\\Models\\" + outName + get_select_model_args();
                Process process = Process.Start("AssetImporter.exe", args);
            }
        }

        private void LoadModelList()
        {
            string strPath = System.Environment.CurrentDirectory;
            DirectoryInfo dirInfo = new DirectoryInfo(strPath);
            FileInfo[] files = dirInfo.GetFiles();
            lvModelList.Columns.Add("");
            lvModelList.Columns[0].Width = 260;

            for (int i = 0; i < files.Length; i++)
            {
                FileInfo fileInfo = files[i];
                string fileName = fileInfo.Name;

                if (fileInfo.Extension.ToUpper() == ".DAE")
                {
                    ListViewItem item = new ListViewItem();
                    item.Text = fileName;

                    lvModelList.Items.Add(item);
                }
            }
        }

        private string get_select_model_args()
        {
            for (int i = 0; i < lstModelFormat.Count; i++)
            {
                RadioButton rad = lstModelFormat[i];
                if (rad.Checked == true)
                {
                    return (string)rad.Tag;
                }
            }

            return "";
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            LoadModelList();

            lstModelFormat.Add(radioButton1);
            lstModelFormat.Add(radioButton2);
            lstModelFormat.Add(radioButton3);

            string strCommon = " -b -l ";
            string strFix = " -p export";
            radioButton1.Tag = strCommon + "" + strFix;
            radioButton2.Tag = strCommon + "" + strFix;
            radioButton3.Tag = strCommon + " -t" + strFix;


        }
    }
}
