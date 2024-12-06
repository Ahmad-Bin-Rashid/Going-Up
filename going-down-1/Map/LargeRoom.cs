using System;
using System.Collections.Generic;

namespace GoingDown
{
    public class LargeRoom : Room
    {
        // public List<Encounter> Encounters { get; private set; } = new();

        public LargeRoom((int, int) position)
            : base(RoomType.Large, position) { }

        public void GenerateEncounters()
        {
            // Logic to add encounters to this room.
        }
    }
}

