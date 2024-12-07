using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace GoingDown
{
    public class Floor
    {
        public List<Room> Rooms { get; private set; } = new();
        public Room SpawnRoom { get; private set; }
        public Room BossRoom { get; private set; }

        private Random random = new();

        public void GenerateFloorGraph()
        {
            Console.WriteLine("Generating Floor...");
            SpawnRoom = new Room(RoomType.Spawn, (0, 0));
            BossRoom = new Room(RoomType.Boss, (5, 5));

            Rooms.Add(SpawnRoom);
            Rooms.Add(BossRoom);

            CreatePath(SpawnRoom, BossRoom);
            AddAdditionalRooms();
            EnsureConnectivity();
            Console.WriteLine("Floor Generated!");
        }

        private void CreatePath(Room start, Room end)
        {
            Console.WriteLine("Creating Main Path...");
            Room current = start;

            while (!current.Equals(end))
            {
                (int, int) currentPosition = current.Position;
                (int, int) nextPosition = (
                    currentPosition.Item1 + Math.Sign(end.Position.Item1 - currentPosition.Item1),
                    currentPosition.Item2 + Math.Sign(end.Position.Item2 - currentPosition.Item2)
                );

                Room? existingRoom = Rooms.Find(r => r.Position == nextPosition);
                if (existingRoom != null)
                {
                    current.Connect(existingRoom);
                    current = existingRoom;
                }
                else
                {
                    Room newRoom = new Room(RoomType.Large, nextPosition);
                    Rooms.Add(newRoom);
                    current.Connect(newRoom);
                    current = newRoom;
                }
            }
        }

        private void AddAdditionalRooms()
        {
            Console.WriteLine("Adding Additional Rooms...");
            int additionalRooms = random.Next(3, 7);

            for (int i = 0; i < additionalRooms; i++)
            {
                RoomType randomType = (RoomType)random.Next(2, Enum.GetValues(typeof(RoomType)).Length);
                (int, int) randomPosition = GenerateUniquePosition();
                Room newRoom = new Room(randomType, randomPosition);
                Rooms.Add(newRoom);

                int connections = random.Next(2, 5);
                for (int j = 0; j < connections; j++)
                {
                    Room existingRoom = Rooms[random.Next(Rooms.Count)];
                    newRoom.Connect(existingRoom);
                }
            }
        }

        private (int, int) GenerateUniquePosition()
        {
            (int, int) position;
            do
            {
                position = (random.Next(0, 5), random.Next(0, 5));
            } while (Rooms.Exists(r => r.Position == position));
            return position;
        }

        private void EnsureConnectivity()
        {
            Console.WriteLine("Ensuring Connectivity...");
            HashSet<Room> visited = new();
            Queue<Room> queue = new();
            queue.Enqueue(SpawnRoom);

            while (queue.Count > 0)
            {
                Room current = queue.Dequeue();
                if (visited.Contains(current)) continue;

                visited.Add(current);
                foreach (var connectedRoom in current.Connections)
                {
                    if (!visited.Contains(connectedRoom))
                    {
                        queue.Enqueue(connectedRoom);
                    }
                }
            }

            foreach (var room in Rooms)
            {
                if (!visited.Contains(room))
                {
                    Room randomConnectedRoom = visited.ElementAt(random.Next(visited.Count));
                    room.Connect(randomConnectedRoom);
                    visited.Add(room);
                }
            }
        }
    }
}
