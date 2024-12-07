using System;
using System.Collections.Generic;
using Godot;

namespace GoingDown
{
    public class TrapRoom : Room
    {
        public Potion Trap { get; private set; }

        public TrapRoom((int, int) position)
            : base(Utils.RoomType.Trap, position) { }

        public bool AttemptEvasion(Player player)
        {
            // Logic for trap evasion.
            return true;
        }
    }

}