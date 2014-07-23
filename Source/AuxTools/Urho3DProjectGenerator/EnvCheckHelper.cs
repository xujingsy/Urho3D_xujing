using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Urho3DGenerator
{
    class EnvCheckHelper
    {
        //path环境变量指向的所有目录中是否存在特定的可执行文件
        public static bool IsPathExistExe(string exeName)
        {
            var strPath = Environment.GetEnvironmentVariable("path");
            string[] strArray = strPath.Split(';');
            for (int i = 0; i < strArray.Length; i++)
            {
                string strSingle = strArray[i] + "\\" + exeName;
                if (File.Exists(strSingle) == true)
                {
                    return true;
                }
            }

            return false;
        }

        //是否存在环境变量
        public static bool IsExistEnvVar(string strKey)
        {
            foreach (string enKey in Environment.GetEnvironmentVariables().Keys)
            {
                if (enKey == strKey)
                {
                    return true;
                }
            }

            return false;
        }

        //获得某个环境变量的值
        public static string GetKeyValue(string strKey)
        {
            if (IsExistEnvVar(strKey) == true)
            {
                return Environment.GetEnvironmentVariable(strKey);
            }
            else
            {
                return null;
            }
        }
    }
}
