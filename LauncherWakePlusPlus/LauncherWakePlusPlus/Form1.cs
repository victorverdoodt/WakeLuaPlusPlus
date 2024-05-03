using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Pipes;
using System.Diagnostics;
using System.IO;
using static LauncherWakePlusPlus.Interop;
using System.Runtime.InteropServices;
using GijSoft.DllInjection;
using MaterialSkin.Controls;
using MaterialSkin;
using System.Threading;

namespace LauncherWakePlusPlus
{

    public partial class Form1 : MaterialForm
    {
        bool received = false;

        [System.Security.Permissions.PermissionSet(System.Security.Permissions.SecurityAction.Demand, Name = "FullTrust")]
        protected override void WndProc(ref Message message)
        {
            if (message.Msg == 74U)
            {

                COPYDATASTRUCT cds = new COPYDATASTRUCT();

                Type mytype = cds.GetType();

                cds = (COPYDATASTRUCT)message.GetLParam(mytype);


                if (cds.lpData.Contains("SETPATH"))
                {
                    materialFlatButton1.Enabled = true;
                    received = true;
                }
            }
            base.WndProc(ref message);
        }

        bool IsProcessInjected(Process leagueProcess)
        {
            if (leagueProcess != null)
            {
                try
                {
                    return
                        leagueProcess.Modules.Cast<ProcessModule>()
                            .Any(
                                processModule => processModule.ModuleName == "LuaPlusPlus.dll");
                }
                catch (Exception e)
                {
                    //Log.Warn(e);
                }
            }
            return false;
        }

        public static void setPath(IntPtr wnd, string path)
        {
            var str = string.Format("SETPATH|{0}", path);
            Interop.SendWindowMessage(wnd, Interop.WindowMessageTarget.AppDomainManager, str);
        }

        public static void LoadAssembly(IntPtr wnd, string path)
        {
                var str = string.Format("LOAD|{0}", path);
                Interop.SendWindowMessage(wnd, Interop.WindowMessageTarget.AppDomainManager, str);
        }

        public static void UnloadAssembly(IntPtr wnd, string path)
        {
                var str = string.Format("UNLOAD|{0}", path);
                Interop.SendWindowMessage(wnd, Interop.WindowMessageTarget.AppDomainManager, str);
        }

        public static void SendLoginCredentials(IntPtr wnd, string user, string passwordHash)
        {
            var str = string.Format("LOGIN|{0}|{1}", user, passwordHash);
            Interop.SendWindowMessage(wnd, Interop.WindowMessageTarget.Core, str);
        }

        public Form1()
        {
            InitializeComponent();

            var materialSkinManager = MaterialSkinManager.Instance;
            materialSkinManager.AddFormToManage(this);
            materialSkinManager.Theme = MaterialSkinManager.Themes.LIGHT;
            materialSkinManager.ColorScheme = new ColorScheme(Primary.BlueGrey800, Primary.BlueGrey900, Primary.BlueGrey500, Accent.LightBlue200, TextShade.WHITE);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            /*IntPtr hWnd = IntPtr.Zero;
            foreach (Process pList in Process.GetProcesses())
            {
                if (pList.MainWindowTitle.Contains("PXG Client"))
                {
                    hWnd = pList.MainWindowHandle;
                    SendLoginCredentials(hWnd, "teste", "senha");
                }
            }*/

        }

        private void button2_Click(object sender, EventArgs e)
        {



        }

        private void button3_Click(object sender, EventArgs e)
        {

        }


        public class script
        {

            public bool Enabled;
            public string name;
            public string author;
            public string path;
        };
        private void Form1_Load(object sender, EventArgs e)
        {
            timer1.Start();
            DataTable FilesTable = new DataTable();
            FilesTable.Columns.Add(new DataColumn("Selected", typeof(bool)));
            FilesTable.Columns.Add("FileName");
            FilesTable.Columns.Add("Status");
            DataRow dRow;
            string yourPath = Directory.GetCurrentDirectory() + "\\LuaPlusPlus\\Scripts";
            string searchPattern = "*.lua*";
            var resultData = Directory.GetFiles(yourPath, searchPattern)
                            .Select(x => new { FileName = Path.GetFileName(x), FilePath = x });

            foreach (var item in resultData)
            {
                dRow = FilesTable.NewRow();
                dRow["Selected"] = false;
                dRow["FileName"] = item.FileName;
                dRow["Status"] = "Unloaded";
                FilesTable.Rows.Add(dRow);
            }

            dataGridView1.DataSource = FilesTable;

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            Process[] pxg = Process.GetProcessesByName("pxgme");
            var curDir = Directory.GetCurrentDirectory();
            if (pxg.Length > 0)
            {
                if (!IsProcessInjected(pxg[0]))
                {
                    materialFlatButton2.Enabled = true;
                }
                else
                {
                    materialFlatButton2.Enabled = false;
                }
            }
            else
            {
                materialFlatButton2.Enabled = false;
            }
        }

        private void materialFlatButton1_Click(object sender, EventArgs e)
        {
            foreach (DataGridViewRow row in dataGridView1.Rows)
            {
                
                bool completed = Convert.ToBoolean(row.Cells[0].Value);
                if (completed && row.Cells[2].Value.ToString() == "Unloaded")
                {
                    IntPtr hWnd = IntPtr.Zero;
                    foreach (Process pList in Process.GetProcesses())
                    {
                        if (pList.MainWindowTitle.Contains("PokeXGames"))
                        {
                            hWnd = pList.MainWindowHandle;
                            LoadAssembly(hWnd, row.Cells[1].Value.ToString());
                         
                            row.Cells[2].Value = "Loaded";
                            Thread.Sleep(100);
                        }
                    }
                }

            }
        }

        private void materialFlatButton2_Click(object sender, EventArgs e)
        {
            DataTable FilesTable = new DataTable();
            FilesTable.Columns.Add(new DataColumn("Selected", typeof(bool)));
            FilesTable.Columns.Add("FileName");
            FilesTable.Columns.Add("Status");
            DataRow dRow;
            string yourPath = Directory.GetCurrentDirectory() + "\\LuaPlusPlus\\Scripts";
            string searchPattern = "*.lua*";
            var resultData = Directory.GetFiles(yourPath, searchPattern)
                            .Select(x => new { FileName = Path.GetFileName(x), FilePath = x });

            foreach (var item in resultData)
            {
                dRow = FilesTable.NewRow();
                dRow["Selected"] = false;
                dRow["FileName"] = item.FileName;
                dRow["Status"] = "Unloaded";
                FilesTable.Rows.Add(dRow);
            }

            dataGridView1.DataSource = FilesTable;

            Process[] pxg = Process.GetProcessesByName("pxgme");
            var curDir = Directory.GetCurrentDirectory();
            if (pxg.Length > 0)
            {
                if (!IsProcessInjected(pxg[0]))
                {
                    DllInjector teste = DllInjector.GetInstance;
                    var retu = teste.Inject("pxgme", curDir + "/LuaPlusPlus/Core/LuaPlusPlus.dll");

                    if (retu == DllInjectionResult.Success)
                    {
                        IntPtr hWnd = IntPtr.Zero;
                        foreach (Process pList in Process.GetProcesses())
                        {
                            if (pList.MainWindowTitle.Contains("PokeXGames"))
                            {
                                hWnd = pList.MainWindowHandle;
                                setPath(hWnd, curDir);
                            }
                        }
                    }
                }
                else if (IsProcessInjected(pxg[0]) && !received)
                {
                    IntPtr hWnd = IntPtr.Zero;
                    foreach (Process pList in Process.GetProcesses())
                    {
                        if (pList.MainWindowTitle.Contains("PokeXGames"))
                        {
                            hWnd = pList.MainWindowHandle;
                            setPath(hWnd, curDir);
                        }
                    }
                }
            }
        }
    }
}
