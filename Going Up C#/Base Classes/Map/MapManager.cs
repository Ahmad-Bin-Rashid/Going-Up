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
            for (int j = 0; j < Map.Floors[0].Rooms.Count; j++)
            {
                var position = Map.Floors[0].Rooms[j].Position;
                // Console.SetCursorPosition(position.Item1, position.Item2);
                Console.WriteLine($"{position.Item1}, {position.Item2}");
            }   
        }
    }
}