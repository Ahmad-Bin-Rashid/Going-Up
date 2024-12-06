using System;
using System.Collections.Generic;

namespace GoingDown
{
    public class Room
    {
        public RoomType Type { get; private set; }
        public (int, int) Position { get; private set; }
        public List<Room> Connections { get; private set; } = new();

        public Room(RoomType type, (int, int) position)
        {
            Type = type;
            Position = position;
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
    }

    public enum RoomType
    {
        Spawn,
        Boss,
        Large,
        Treasure,
        Trap
    }

}