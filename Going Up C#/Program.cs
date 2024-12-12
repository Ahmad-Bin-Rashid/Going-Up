// using System;
// using System.Runtime.InteropServices;

// namespace GoingUp
// {
//     class Program
//     {
//         // Importing user32.dll to access Windows API
//         [DllImport("user32.dll")]
//         private static extern IntPtr GetConsoleWindow();

//         [DllImport("user32.dll")]
//         private static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

//         private const int SW_MAXIMIZE = 3;

//         public static void MaximizeConsole()
//         {
//             IntPtr consoleWindow = GetConsoleWindow();
//             if (consoleWindow != IntPtr.Zero)
//             {
//                 ShowWindow(consoleWindow, SW_MAXIMIZE);
//             }
//         }
//         static void Main(string[] args)
//         {
//             MaximizeConsole();

//             // Set console buffer size and window size for better UX
//             Console.SetWindowSize(Console.LargestWindowWidth, Console.LargestWindowHeight);
//             Console.SetBufferSize(Console.LargestWindowWidth, Console.LargestWindowHeight);

//             MapManager mapManager = new MapManager(3);
//             Console.Clear();
//             mapManager.Displaymap();
//         }
//     }
// }


using System;
using System.Runtime.InteropServices;

class Program  
   {  
      [DllImport("kernel32.dll", ExactSpelling = true)]  
      private static extern IntPtr GetConsoleWindow();  
      private static IntPtr ThisConsole = GetConsoleWindow();  
      [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]  
      private static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);  
      private const int HIDE = 0;  
      private const int MAXIMIZE = 3;  
      private const int MINIMIZE = 6;  
      private const int RESTORE = 9;  
      static void Main(string[] args)  
      {  
         Console.SetWindowSize(Console.LargestWindowWidth, Console.LargestWindowHeight);  
         ShowWindow(ThisConsole, MAXIMIZE);  
         Console.ReadLine();  
      } 
   }
