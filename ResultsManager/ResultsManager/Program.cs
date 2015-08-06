using System;
using System.IO;
using System.Windows.Forms;

using Excel = Microsoft.Office.Interop.Excel;

namespace ResultsManager
{
    public class Program
    {
        [STAThread]
        private static void Main(string[] args)
        {
            OpenFileDialog openFileDialog1 = new OpenFileDialog();

            openFileDialog1.Filter = "Text Files (.txt)|*.txt|All Files (*.*)|*.*";
            openFileDialog1.FilterIndex = 1;
            openFileDialog1.Multiselect = true;
            //openFileDialog1.InitialDirectory = @"C:\Users\Yvens\Documents\GitHub\Shaders_Estudos\Logs\";
            openFileDialog1.InitialDirectory = @"F:\Repositorios\Shaders_Estudos\Logs\";

            DialogResult result = openFileDialog1.ShowDialog();

            int row = 1;
            int column = 1;
            int baseColumn = 1;

            int condensate = 0;
            float[] condensatedTokens = null;

            int condensity = 1;
            if (result == DialogResult.OK)
            {
                Excel.Application excelApp = new Excel.Application();

                excelApp.Workbooks.Add();
                Excel._Worksheet workSheet = (Excel.Worksheet)excelApp.ActiveSheet;
                excelApp.Visible = true;

                string[] files = openFileDialog1.FileNames;

                foreach (string file in files)
                {
                    string[] lines = File.ReadAllLines(file);

                    string[] fileNameTokens = file.Split(Path.DirectorySeparatorChar);
                    string fileName = fileNameTokens[fileNameTokens.Length - 1];

                    baseColumn = column;

                    workSheet.Cells[row, column] = fileName;
                    row++;
                    foreach (string line in lines)
                    {
                        column = baseColumn;

                        string[] tokens = line.Split(';');
                        if (condensatedTokens == null)
                        {
                            condensatedTokens = new float[tokens.Length];
                        }

                        for (int i = 0; i < tokens.Length; i++)
                        {
                            condensatedTokens[i] += float.Parse(tokens[i]);
                        }
                        if (condensate != 0)
                        {
                            condensity++;
                            if (condensity == condensate)
                            {
                                foreach (float condensatedToken in condensatedTokens)
                                {
                                    workSheet.Cells[row, column++] = condensatedToken / condensate;
                                }
                                row++;
                                condensity = 1;
                                condensatedTokens = null;
                            }
                        }
                        else
                        {
                            foreach (float condensatedToken in condensatedTokens)
                            {
                                workSheet.Cells[row, column++] = condensatedToken;
                            }
                            row++;
                            condensity = 1;
                            condensatedTokens = null;
                        }
                    }

                    if (condensatedTokens != null)
                    {
                        foreach (float condensatedToken in condensatedTokens)
                        {
                            workSheet.Cells[row, column++] = condensatedToken / condensate;
                        }
                        condensity = 1;
                        condensatedTokens = null;
                    }

                    column += 2;
                    row = 1;
                }
            }
        }
    }
}