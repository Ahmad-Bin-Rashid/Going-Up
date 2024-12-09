using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static going_down.Utils;

namespace going_down
{
    public class Floor
    {
        public int FloorNumber { get; private set; }
        public List<Room> Rooms { get; private set; }

        public Floor(int floorNumber, int minRooms, int maxRooms)
        {
            FloorNumber = floorNumber;
            Rooms = new List<Room>();
            GenerateMap(minRooms, maxRooms);
        }

        private void GenerateMap(int minRooms, int maxRooms)
        {
            var random = new Random();
            int roomCount = random.Next(minRooms, maxRooms + 1);
            
            // Step 1: Create spawn and boss rooms
            Rooms.Add(new Room(RoomType.Spawn, (0, 0))); // Spawn room at (0,0)
            Rooms.Add(new Room(RoomType.Boss, (10, 10))); // Boss room at (10,10)

            // Step 2: Ensure at least one of each required room type
            Rooms.Add(new Room(RoomType.Large, (random.Next(1, 10), random.Next(1, 10))));
            Rooms.Add(new Room(RoomType.Large, (random.Next(1, 10), random.Next(1, 10))));

            // Step 3: Generate additional random rooms
            for (int i = 0; i < roomCount - 4; i++) // Remaining rooms after the fixed ones
            {
                int x = random.Next(1, 10);
                int y = random.Next(1, 10);
                RoomType randomType = (RoomType)random.Next(2, 5); // Random between Large, Treasure, Trap
                Rooms.Add(new Room(randomType, (x, y)));
            }

            // Step 4: Connect rooms based on shortest distance
            ConnectRooms();
        }

        private void ConnectRooms()
        {
            foreach (var room in Rooms)
            {
                // Determine connection range based on room type
                int minConnections = room.Type == RoomType.Spawn || room.Type == RoomType.Boss ? 1 : 2;
                int maxConnections = room.Type == RoomType.Spawn || room.Type == RoomType.Boss ? 1 : 3;

                // Sort other rooms by distance
                var potentialConnections = Rooms
                    .Where(r => r != room && !room.Connections.Contains(r))
                    .OrderBy(r => room.DistanceTo(r))
                    .ToList();

                // Select nearest rooms and add connections
                int connectionsToAdd = new Random().Next(minConnections, maxConnections + 1);
                for (int i = 0; i < connectionsToAdd && i < potentialConnections.Count; i++)
                {
                    Room target = potentialConnections[i];
                    AddConnection(room, target);
                }
            }

            // Step 3: Ensure graph connectivity
            EnsureGraphConnectivity();
        }

        private void AddConnection(Room room1, Room room2)
        {
            if (!room1.Connections.Contains(room2))
                room1.Connections.Add(room2);

            if (!room2.Connections.Contains(room1))
                room2.Connections.Add(room1);
        }

        private void EnsureGraphConnectivity()
        {
            // Perform BFS/DFS to check connectivity
            var visited = new HashSet<Room>();
            var queue = new Queue<Room>();

            queue.Enqueue(Rooms[0]); // Start with the first room (Spawn)
            while (queue.Count > 0)
            {
                var current = queue.Dequeue();
                if (visited.Contains(current)) continue;

                visited.Add(current);
                foreach (var neighbor in current.Connections)
                {
                    if (!visited.Contains(neighbor))
                        queue.Enqueue(neighbor);
                }
            }

            // Check if all rooms are visited
            if (visited.Count != Rooms.Count)
            {
                // Connect any unvisited rooms to the nearest visited room
                var unvisitedRooms = Rooms.Where(r => !visited.Contains(r)).ToList();
                foreach (var unvisited in unvisitedRooms)
                {
                    var nearestVisited = visited.OrderBy(r => r.DistanceTo(unvisited)).First();
                    AddConnection(unvisited, nearestVisited);
                }
            }
        }

        public void PrintMap()
        {
            foreach (var room in Rooms)
            {
                Console.WriteLine(room.ToString());
            }
            Console.WriteLine("\n");
            foreach (var room in Rooms)
            {
                Console.WriteLine($"{room}");
                Console.WriteLine($"Connected to: \n{string.Join(",\n ", room.Connections)}");
            }
        }
    }
}
