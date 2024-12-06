using System;
using System.Collections.Generic;

namespace GoingDown
{
    public class Weapon : Item
    {
        public enum WeaponType { Sword, Dagger, Mace, Bow }

        public WeaponType Type { get; private set; }
        public int BaseDamage { get; private set; } // Damage dice determined by rarity and type.

        public Weapon(string name, RarityType rarity, WeaponType type)
            : base(name, rarity)
        {
            Type = type;
            CalculateAttributes();
            Description = $"A {rarity} {type} that deals {BaseDamage} base damage.";
        }

        private void CalculateAttributes()
        {
            int diceCount = GetDiceCount(Rarity);
            int diceType = Type switch
            {
                WeaponType.Sword => 6,
                WeaponType.Dagger => 4,
                WeaponType.Mace => 8,
                WeaponType.Bow => 6,
                _ => 6
            };
            BaseDamage = RollDice(diceCount, diceType);
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
            player.EquipWeapon(this);
        }
    }
}