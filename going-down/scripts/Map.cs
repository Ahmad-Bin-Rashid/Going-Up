// using System;
// using System.Collections.Generic;
// using System.Linq;

// public partial class Map 
// {
//     public enum RoomType
//     {
//         Spawn,
//         Boss,
//         Large,
//         Treasure,
//         Trap
//     }

//     public class Room
//     {
//         public RoomType Type { get; set; }
//         public (int, int) Position { get; set; }
//         public List<Room> Connections { get; set; }

//         public Room(RoomType type, (int, int) position)
//         {
//             Type = type;
//             Position = position;
//             Connections = new List<Room>();
//         }

//         public void Connect(Room RoomToAdd)
//         {
//             if (!Connections.Contains(RoomToAdd))
//             {
//                 Connections.Add(RoomToAdd);
//             }
//             if (!RoomToAdd.Connections.Contains(this))
//             {
//                 RoomToAdd.Connections.Add(this);
//             }
//         }

//         public override bool Equals(object obj)
//         {
//             if (obj is Room other)
//             {
//                 return this.Position == other.Position;
//             }
//             return false;
//         }

//         public override int GetHashCode()
//         {
//             return Position.GetHashCode();
//         }
//     }


//     private List<Room> rooms;
//     private Random random;

//     public Map()
//     {
//         rooms = new List<Room>();
//         random = new Random();
//     }

//     public void GenerateMap()
//     {
//         Console.WriteLine("Generating Map...");
//         Room spawnRoom = new(RoomType.Spawn, (0, 0));
//         Room bossRoom = new(RoomType.Boss, (10, 10));

//         rooms.Add(spawnRoom);
//         rooms.Add(bossRoom);

//         CreatePath(spawnRoom, bossRoom);

//         int additionalRooms = random.Next(5, 10); 
//         for (int i = 0; i < additionalRooms; i++)
//         {
//             (int, int) newRandomPosition = GenerateUniquePosition();
//             RoomType randomType = (RoomType)random.Next(2, Enum.GetValues(typeof(RoomType)).Length);
//             Room newRoom = new(randomType, newRandomPosition);
//             rooms.Add(newRoom);

//             int connections = random.Next(1, 4);
//             for (int j = 0; j < connections; j++)
//             {
//                 Room existingRoom = rooms[random.Next(rooms.Count)];
//                 newRoom.Connect(existingRoom);
//             }
//         }

//         CreateAdditionalPaths();
//         Console.WriteLine("Map Generated!");
//     }


//     private void AddRandomRoom()
//     {
//         RoomType randomType = (RoomType)random.Next(2, Enum.GetValues(typeof(RoomType)).Length); // Exclude Spawn and Boss

//         (int, int) randomPosition = GenerateUniquePosition();

//         Room newRoom = new(randomType, randomPosition);
//         rooms.Add(newRoom);

//         int connections = random.Next(2, 5);
//         HashSet<Room> connectedRooms = new();
//         for (int i = 0; i < connections; i++)
//         {
//             Room existingRoom = rooms[random.Next(rooms.Count)];
//             if (!connectedRooms.Contains(existingRoom) && existingRoom != newRoom)
//             {
//                 newRoom.Connect(existingRoom);
//                 connectedRooms.Add(existingRoom);
//             }
//         }
//     }

//     (int, int) GenerateUniquePosition()
//     {
//         (int, int) position;
//         do
//         {
//             position = (random.Next(-10, 10), random.Next(-10, 10));
//         } while (rooms.Exists(r => r.Position == position));
//         return position;
//     }


//     private void CreatePath(Room start, Room end)
//     {
//         Console.WriteLine("Creating Main Path...");
//         Room current = start;

//         while (!current.Equals(end))
//         {
//             (int, int) currentPosition = current.Position;

//             (int, int) nextPosition = (
//                 currentPosition.Item1 + Math.Sign(end.Position.Item1 - currentPosition.Item1),
//                 currentPosition.Item2 + Math.Sign(end.Position.Item2 - currentPosition.Item2)
//             );

//             Room? existingRoom = rooms.Find(r => r.Position == nextPosition);
//             if (existingRoom != null)
//             {
//                 current.Connect(existingRoom);
//                 current = existingRoom;
//             }
//             else
//             {
//                 Room newRoom = new(RoomType.Large, nextPosition);
//                 rooms.Add(newRoom);
//                 current.Connect(newRoom);
//                 current = newRoom;
//             }
//         }
//         Console.WriteLine("Main Path Created!");
//     }



//     private void CreateAdditionalPaths()
//     {
//         Console.WriteLine("Creating Additional Paths...");
//         foreach (var room in rooms)
//         {
//             int connections = random.Next(1, 4);
//             for (int i = 0; i < connections; i++)
//             {
//                 Room targetRoom = rooms[random.Next(rooms.Count)];
//                 if (!room.Connections.Contains(targetRoom) && room != targetRoom)
//                 {
//                     room.Connect(targetRoom);
//                 }
//             }
//         }
//         Console.WriteLine("Additional Paths Created!");
//     }


//     private void EnsureConnectivity()
//     {
//         Console.WriteLine("Ensuring Connectivity...");
//         HashSet<Room> visited = new();
//         Queue<Room> queue = new();
//         queue.Enqueue(rooms[0]);

//         while (queue.Count > 0)
//         {
//             Room current = queue.Dequeue();
//             if (visited.Contains(current)) continue;

//             visited.Add(current);
//             foreach (var connectedRoom in current.Connections)
//             {
//                 if (!visited.Contains(connectedRoom))
//                 {
//                     queue.Enqueue(connectedRoom);
//                 }
//             }
//         }

//         foreach (var room in rooms)
//         {
//             if (!visited.Contains(room))
//             {
//                 Room randomConnectedRoom = visited.ElementAt(random.Next(visited.Count));
//                 room.Connect(randomConnectedRoom);
//                 visited.Add(room);
//             }
//         }
//         Console.WriteLine("All Rooms Connected!");
//     }


//     public List<Room> GetRooms()
//     {
//         return rooms;
//     }
// }



// /* ---------------------Main To Check Map Connections-------------- */

// // public static void Main(string[] args)
// //     {
// //         Map gameMap = new();

// //         gameMap.GenerateMap();

// //         var rooms = gameMap.GetRooms();
// //         Console.WriteLine("\nGenerated Rooms:");
// //         foreach (var room in rooms)
// //         {
// //             Console.WriteLine($"Room Type: {room.Type}, Position: {room.Position}, Connections: {room.Connections.Count}");
// //         }

// //         Console.WriteLine("\nRoom Connections:");
// //         foreach (var room in rooms)
// //         {
// //             Console.WriteLine($"Room {room.Position} ({room.Type}) is connected to:");
// //             foreach (var connection in room.Connections)
// //             {
// //                 Console.WriteLine($"  -> Room {connection.Position} ({connection.Type})");
// //             }
// //         }

// //         Console.WriteLine("\nMap generation complete. Enjoy your game!");
// //     }