using System;

namespace GoingUp
{
    public class MapManager
    {
        public Map Map { get; private set; }

        public MapManager(int numberOfFloors)
        {
            Map = new Map(numberOfFloors);
        }

        public void Displaymap()
        {
            for (int i = 0; i < Map.Floors.Count; i++)
            {
                (int, int) position = Map.Floors[i].Rooms[0].Position;
                Console.SetCursorPosition(position.Item1, position.Item2);

                Console.WriteLine($"Floor {i + 1}");
            }
        }
    }
}