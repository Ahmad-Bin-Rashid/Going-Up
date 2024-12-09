using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static going_down.Utils;

namespace going_down
{
    public class Room
    {
        public RoomType Type { get; private set; }
        public int X { get; private set; }
        public int Y { get; private set; }
        public List<Room> Connections { get; set; }

        public Room(RoomType type, int x, int y)
        {
            Type = type;
            X = x;
            Y = y;
            Connections = new List<Room>();
        }

        public double DistanceTo(Room other)
        {
            return Math.Sqrt(Math.Pow(X - other.X, 2) + Math.Pow(Y - other.Y, 2));
        }

        public override string ToString()
        {
            return $"{Type} Room at ({X}, {Y})";
        }
    }
}
