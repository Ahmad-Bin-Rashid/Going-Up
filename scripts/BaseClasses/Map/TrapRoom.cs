using System;
using System.Collections.Generic;
using Godot;

namespace GoingDown
{
    public class TrapRoom : Room
    {
        public Potion Trap { get; private set; }

        public TrapRoom((int, int) position)
            : base(Utils.RoomType.Trap, position)
        {
            Random random = new Random();
            Utils.TrapType trapType = (TrapType)random.Next(1, Enum.GetValues(typeof(TrapType)).Length);

            switch (trapType)
            {
                case Utils.TrapType.Fire:
                    Trap = new FirePotion();
                    break;
                case Utils.TrapType.Ice:
                    Trap = new IcePotion();
                    break;
                case Utils.TrapType.Poison:
                    Trap = new PoisonPotion();
                    break;
            }

            GD.Print($"Trap Room created with {Trap.Name}");
        }

        public Potion GetTrap()
        {
            return Trap;
        }

        public Potion RemoveTrap()
        {
            var temp = Trap;
            Trap = null;
            return temp;
        }
    }

}