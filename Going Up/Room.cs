using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoingUp
{
    public class Room
    {
        public RoomType Type { get; private set; }
        public (int, int) Position { get; private set; }
        public bool visited;
        public List<Room> Connections { get; set; }

        public Room(RoomType type, (int, int) position)
        {
            Type = type;
            Position = position;
            visited = false;
            Connections = new List<Room>();
        }

        public Room(RoomType type, (int, int) position, bool visit)
        {
            Type = type;
            Position = position;
            visited = visit;
            Connections = new List<Room>();
        }

        public double DistanceTo(Room other)
        {
            return Math.Sqrt(Math.Pow(Position.Item1 - other.Position.Item1, 2) + Math.Pow(Position.Item2 - other.Position.Item2, 2));
        }

        public override string ToString()
        {
            return $"{Type} Room at ({Position.Item1}, {Position.Item2})";
        }

        public override bool Equals(object obj)
        {
            return obj is Room other && this.Position == other.Position;
        }
        public override int GetHashCode()
        {
            return Position.GetHashCode();
        }

        public List<Room> GetConnectedsRoom()
        {
            return Connections;
        }
        public List<string> GetConnectedRoomDoors()
        {
            List<string> rooms = new List<string>();

            foreach (var connectedRoom in Connections)
            {
                if (connectedRoom.visited == true)
                {
                    rooms.Add(connectedRoom.Type.ToString());
                }
                else
                {
                    // rooms.Add(connectedRoom.Type.ToString());
                    rooms.Add("????????");       //Disabled for testing
                }

            }

            return rooms;
        }
        public String GetRoomDescription()
        {
            switch (Type)
            {
                case RoomType.Boss:
                    return "You Feel a Chill as you enter.Pray well as a hard foe awaits ahead.";
                case RoomType.Spawn:
                    return "This is the start of a new floor.May u survive this.";
                case RoomType.Large:
                    return "No time to rest.Is there a foe ahead or a friend";
                case RoomType.Treasure:
                    return "You got lucky but will this luck remain";
                case RoomType.Trap:
                    return "If u walk near a river u are bound to get wet.Pray that u survive";
                default:
                    return "I dont know how u got here.";
            }
        }
    }
}
