using System;
using System.Collections.Generic;
using Godot;


namespace GoingDown
{
    public class Room
    {
        public Utils.RoomType Type { get; private set; }
        public (int, int) Position { get; private set; }
        public List<Room> Connections { get; private set; } = new();
        public bool visited;
        public bool cleared;

        public Room(Utils.RoomType type, (int, int) position)
        {
            Type = type;
            Position = position;
            visited = false;
            cleared = false;
        }
        public Room(Utils.RoomType type, (int, int) position,bool visit)
        {
            Type = type;
            Position = position;
            visited = visit;
            cleared = false;
        }

        public void Connect(Room roomToAdd)
        {
            if (!Connections.Contains(roomToAdd))
            {
                Connections.Add(roomToAdd);
            }
            if (!roomToAdd.Connections.Contains(this))
            {
                roomToAdd.Connections.Add(this);
            }
        }
        public override bool Equals(object? obj)
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
                    rooms.Add(connectedRoom.Type.ToString());
                    // rooms.Add("????????");       //Disabled for testing
                }
                
            }

            return rooms;
        }
        public String GetRoomDescription()
	{
		switch (Type)
		{
			case Utils.RoomType.Boss:
				return "You Feel a Chill as you enter.Pray well as a hard foe awaits ahead.";
			case Utils.RoomType.Spawn:
				return "This is the start of a new floor.May u survive this.";
			case Utils.RoomType.Large:
				return "No time to rest.Is there a foe ahead or a friend";
			case Utils.RoomType.Treasure:
				return "You got lucky but will this luck remain";
			case Utils.RoomType.Trap:
				return "If u walk near a river u are bound to get wet.Pray that u survive";
			default:
				return "I dont know how u got here.";
		}
	}
    }

}