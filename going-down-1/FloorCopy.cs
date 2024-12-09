// using System;
// using System.Collections.Generic;

// namespace GoingDown
// {
//     public class Floorr
//     {
//         public Room SpawnRoom { get; private set; }
//         public Room BossRoom { get; private set; }
//         public List<Room> Rooms;
//         public Random random = new();

//         public Floor()
//         {
//             SpawnRoom = new Room(Utils.RoomType.Spawn, (0, 0), true);
//             BossRoom = new Room(Utils.RoomType.Boss, (10, 10), true);
//             Rooms = new List<Room>();
//             Rooms.Add(SpawnRoom);
//             Rooms.Add(BossRoom);
//         }

//         public void GenerateFloorGraph()
//         {
//             Console.WriteLine("Generating Floor...");

//             int additionalRooms = random.Next(5, 8);
//             for (int i = 0; i < additionalRooms; i++)
//             {
//                 var roomType = (RoomType)random.Next(1, Enum.GetValues(typeof(RoomType)).Length);
//                 var position = (random.Next(1, 9), random.Next(1, 9));
//                 var room = new Room(roomType, position);
//                 Rooms.Add(room);
//             }

//             Console.WriteLine("Floor Generated!");

//             foreach (var room in Rooms)
//             {
//                 Console.WriteLine($"Room: {room.Type} at {room.Position}");
//             }

//             Console.WriteLine("Connecting rooms...");

//             // Connect rooms based on the respective distance between rooms
//             foreach (var room in Rooms)
//             {
//                 var closestRooms = GetClosestRoom(room);
//                 foreach (var closestRoom in closestRooms)
//                 {
//                     room.Connect(closestRoom);
//                 }
//             }

//             Console.WriteLine("Rooms connected!");

//             // Print the connections

//             foreach (var room in Rooms)
//             {
//                 Console.WriteLine($"Room: {room.Type} at {room.Position}");
//                 foreach (var connectedRoom in room.ConnectedRooms)
//                 {
//                     Console.WriteLine($"Connected to: {connectedRoom.Type} at {connectedRoom.Position}");
//                 }
//             }
//         }

//         private List<Room> GetClosestRoom(Room room)
//         {
//             var closestRooms = new List<Room>();
//             var closestDistance = 1000f;

//             foreach (var otherRoom in Rooms)
//             {
//                 if (room == otherRoom) continue;

//                 var distance = room.Position.DistanceTo(otherRoom.Position);
//                 if (distance < closestDistance)
//                 {
//                     closestDistance = distance;
//                     closestRooms.Clear();
//                     closestRooms.Add(otherRoom);
//                 }
//                 else if (distance == closestDistance)
//                 {
//                     closestRooms.Add(otherRoom);
//                 }
//             }

//             return closestRooms;
//         }
//     }
// }