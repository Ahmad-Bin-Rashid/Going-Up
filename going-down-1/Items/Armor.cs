using System;
using System.Collections.Generic;

namespace GoingDown
{
    public class Armor : Item
    {
        public enum ArmorType { Helmet, Boots, Chestplate }

        public ArmorType Type { get; private set; }
        public int Bonus { get; private set; } // Constitution or HP bonus based on type.

        public Armor(string name, RarityType rarity, ArmorType type)
            : base(name, rarity)
        {
            Type = type;
            CalculateAttributes();
            Description = $"A {rarity} {type} that provides a bonus of {Bonus}.";
        }

        private void CalculateAttributes()
        {
            int diceCount = GetDiceCount(Rarity);
            Bonus = RollDice(diceCount, 6);
        }

        private int GetDiceCount(RarityType rarity)
        {
            return rarity switch
            {
                RarityType.Common => 1,
                RarityType.Uncommon => 2,
                RarityType.Rare => 4,
                RarityType.Legendary => 6,
                _ => 1
            };
        }

        private int RollDice(int diceCount, int diceType)
        {
            Random random = new();
            int total = 0;
            for (int i = 0; i < diceCount; i++)
            {
                total += random.Next(1, diceType + 1);
            }
            return total;
        }

        public override void Use(Player player)
        {
            player.EquipArmor(this);
        }
    }

}