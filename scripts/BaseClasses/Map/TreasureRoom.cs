using System;
using System.Collections.Generic;
using Godot;

namespace GoingDown
{
    public class TreasureRoom : Room
    {
        public Item item { get; private set; }
        public Random random = new();

        public TreasureRoom((int, int) position)
            : base(Utils.RoomType.Treasure, position) 
        {
            random = new Random();
            Utils.TreasureType treasureType = (TreasureType)random.Next(1, Enum.GetValues(typeof(TreasureType)).Length);

            switch (treasureType)
            {
                case Utils.TreasureType.Armor:
                    item = new Armor();
                    break;
                case Utils.TreasureType.Weapon:
                    item = new Weapon();
                    break;
                case Utils.TreasureType.Potion:
                    item = new Potion();
                    break;
            }

            GD.Print($"Treasure Room created with {item.Name}");
        }

        public Item GetItem()
        {
            return item;
        }

        public Item RemoveItem()
        {
            var temp = item;
            item = null;
            return temp;
        }
    }

}