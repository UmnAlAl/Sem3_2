using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Security.Cryptography;

namespace ALG_DZ1
{
    class Program
    {
        enum AlgType { RIJNDAEL, DES };
        static void Main(string[] args)
        {
            try
            {
                if (args[0] == "-help")
                {
                    string help = File.ReadAllText(@"HELP.TXT", UTF8Encoding.GetEncoding(0));
                    Console.WriteLine(help);
                    return;
                }

                bool ENCRYPT;
                if (args[0] == "-e")
                    ENCRYPT = true;
                else if (args[0] == "-d")
                    ENCRYPT = false;
                else
                    throw new Exception("Неверный аргумент шифрования\\дешифрования (необходимо -e или -d).");

                if (args[1] != "-in")
                    throw new Exception("Неверный аргумент, указывающий на входной файл (необходим -in).");

                string strInputFileName = args[2];
                byte[] InputBuf;
                int BYTES_READED = 0;
                if (File.Exists(strInputFileName))
                {
                    InputBuf = File.ReadAllBytes(strInputFileName);
                    BYTES_READED = InputBuf.Length;
                    string s = Path.GetExtension(strInputFileName);
                    if (s != ".exe")
                    {
                        Console.WriteLine("Warning: Входной файл имеет неподходящее разрешение: " + s);
                    }
                }
                else
                    throw new Exception("Указанный входной файл не существует " + strInputFileName + ".");

                if (args[3] != "-out")
                    throw new Exception("Неверный аргумент, указывающий на вsходной файл (необходим -out).");

                string strOutputFileName = args[4];
                //FileStream OutputStream =  File.Create(strOutputFileName);

                if (args[5] != "-alg")
                    throw new Exception("Неверный аргумент, указывающий на используемый алгоритм (необходим -alg).");

                AlgType CurrAlg;
                if (args[6] == "rijndael")
                {
                    CurrAlg = AlgType.RIJNDAEL;
                }
                else if (args[6] == "des")
                {
                    CurrAlg = AlgType.DES;
                }
                else
                    throw new Exception("Неверное название алгоритма (необходимо rijndael или des).");

                byte[] OutputBuf = new byte[0];
                byte[] sha = new byte[0];
                string password;
                int BYTES_WRITTEN;
                Console.WriteLine("Введите пароль:");
                password = Console.ReadLine();                

                DateTime Start = DateTime.Now;
                if (CurrAlg == AlgType.RIJNDAEL)
                    fn_Rijndael_DES_Password_Crypt(InputBuf, ref OutputBuf, password, ENCRYPT, AlgType.RIJNDAEL);
                else
                    fn_Rijndael_DES_Password_Crypt(InputBuf, ref OutputBuf, password, ENCRYPT, AlgType.DES);
                TimeSpan Time = DateTime.Now - Start;
                File.WriteAllBytes(strOutputFileName, OutputBuf);
                BYTES_WRITTEN = OutputBuf.Length;
                if (ENCRYPT)
                   fn_SHA384_calculate(ref sha, InputBuf);
                else
                   fn_SHA384_calculate(ref sha, OutputBuf);
                Console.WriteLine("Затраченное время: {0} миллисекунд", Time.TotalMilliseconds);
                Console.WriteLine("Объём входного файла: {0} байт.", BYTES_READED);
                Console.WriteLine("Объём выходного файла: {0} байт.", BYTES_WRITTEN);
                Console.WriteLine("Хеш: {0} ", Convert.ToBase64String(sha));
                Console.ReadLine();                                                  
            }//try
            catch (CryptographicException ex)
            {
                Console.WriteLine("Неверный пароль.");
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);                
            }           
        }//main        
        static void fn_Rijndael_DES_Byte_key_IV_Crypt(
            byte[] InputBuf,
            ref byte[] OutputBuf,
            byte[] key, 
            byte[] IV,
            bool ENCRYPT,
            AlgType CurrAlg)
        {
            Rijndael myRijndael;
            TripleDESCryptoServiceProvider myDES;
            ICryptoTransform cryptor = null;
            if (CurrAlg == AlgType.RIJNDAEL)
            {
                myRijndael = new RijndaelManaged();
                myRijndael.Key = key;
                myRijndael.IV = IV;
                if (ENCRYPT)
                    cryptor = myRijndael.CreateEncryptor();
                else
                    cryptor = myRijndael.CreateDecryptor();
            }
            else if (CurrAlg == AlgType.DES)
            {
                myDES = new TripleDESCryptoServiceProvider();
                myDES.Key = key;
                myDES.IV = IV;
                if (ENCRYPT)
                    cryptor = myDES.CreateEncryptor();
                else
                    cryptor = myDES.CreateDecryptor();
            }
            MemoryStream msCrypt = new MemoryStream();
            CryptoStream csCrypt = new CryptoStream(msCrypt, cryptor, CryptoStreamMode.Write);
            csCrypt.Write(InputBuf, 0, InputBuf.Length);            
            csCrypt.Close();
            OutputBuf = msCrypt.ToArray();            
        }//RijDESByteEnc
        static void fn_Rijndael_DES_Password_Crypt(
            byte[] InputBuf,
            ref byte[] OutputBuf,
            string password,
            bool ENCRYPT,
            AlgType CurrAlg)
        {
            byte[] salt = new byte[] {0x49, 0x76, 0x61, 0x6e, 0x20, 0x4d, 
            0x65, 0x64, 0x76, 0x65, 0x64, 0x65, 0x76};
            PasswordDeriveBytes pdb = new PasswordDeriveBytes(System.Text.Encoding.Unicode.GetBytes( password ), salt);            
            fn_Rijndael_DES_Byte_key_IV_Crypt(InputBuf, ref OutputBuf,
                (CurrAlg == AlgType.RIJNDAEL) ? pdb.GetBytes(32) : pdb.GetBytes(24),
                (CurrAlg == AlgType.RIJNDAEL) ? pdb.GetBytes(16) : pdb.GetBytes(8), 
                ENCRYPT, CurrAlg);
        }//RijEnc        
        static void fnDES()
        {
        }//DESEnc
        static void fn_SHA384_calculate(ref byte[] sha, byte[] InputBuf)
        {
            SHA384 shaPerform = new SHA384Managed();
            sha = shaPerform.ComputeHash(InputBuf);
        }
    }//program
}//namespace
