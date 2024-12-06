using System;
using System.Collections.Generic;

namespace GoingDown
{
    public class TreasureRoom : Room
    {
        public TreasureRoom((int, int) position)
            : base(RoomType.Treasure, position) { }

        public Item GenerateLoot()
        {
            // Logic to generate a random item.
            return new Armor("Treasure", RarityType.Common, Armor.ArmorType.Chestplate);
        }
    }

}