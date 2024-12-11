using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoingUp
{
    public class Map
    {
        public List<Floor> Floors { get; private set; }

        public Map(int numberOfFloors)
        {
            Floors = new List<Floor>();

            for (int i = 1; i <= numberOfFloors; i++)
            {
                int minRooms = 8 + (i - 1) * 3;    // minimum limit for rooms
                int maxRooms = 10 + (i - 1) * 3;   // maximum limit for rooms
                Floors.Add(new Floor(i, minRooms, maxRooms));
            }
        }
        public List<Floor> GetAllFloors()
        {
            return Floors;
        }
        public void PrintMap()
        {
            Console.WriteLine("Dungeon Map Overview:");
            for (int i = 0; i < Floors.Count; i++)
            {
                Console.WriteLine($"\n--- Floor {i + 1} ---");
                Floors[i].PrintMap();
            }
        }
    }
}
