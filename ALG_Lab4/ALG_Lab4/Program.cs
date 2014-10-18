using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ALG_Lab4
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                Console.WriteLine("Введите символы.");
                string sInput = Console.ReadLine();
                string sBase = new string(sInput.Distinct().ToArray());
                sBase = sBase.Replace(" ", string.Empty);
                sBase = sBase.Replace("\t", string.Empty);                
                string tmp = "";
                for (int i = 0; i <= sBase.Length; ++i)
                {
                    fnWriteAllCombinations(sBase, tmp, i);
                }
            }
            catch( Exception ex )
            {
                Console.WriteLine(ex.Message);
            }
            Console.ReadLine();
        }//main
        static void fnWriteAllCombinations(string sBase, string tmp, int left)
        {
            if (left == 0)
            {                
                Console.WriteLine(tmp);                
                return;
            }
            foreach (char c in sBase)
            {
                fnWriteAllCombinations(sBase, tmp + c, left - 1);
            }
        }
    }//program
}//namespace
