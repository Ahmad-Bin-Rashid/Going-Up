using System;

namespace GoingUp
{
    class Program
    {
        static void Main(string[] args)
        {
            MapManager mapManager = new MapManager(3);
            Console.Clear();
            mapManager.Displaymap();
        }
    }
}
