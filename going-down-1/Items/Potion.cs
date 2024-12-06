using System;

namespace GoingDown
{
    public class Potion : Item
    {
        public enum PotionType { Healing, Mana, Strength, Constitution }

        public PotionType Type { get; private set; }
        public int EffectValue { get; private set; } // Effect value based on dice rolls.

        public Potion(string name, RarityType rarity, PotionType type)
            : base(name, rarity)
        {
            Type = type;
            CalculateAttributes();
            Description = $"A {rarity} potion of {type} with an effect value of {EffectValue}.";
        }

        private void CalculateAttributes()
        {
            int diceCount = GetDiceCount(Rarity);
            EffectValue = RollDice(diceCount, 6);
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
            switch (Type)
            {
                case PotionType.Healing:
                    player.Heal(EffectValue);
                    break;
                case PotionType.Mana:
                    player.RestoreMP(EffectValue);
                    break;
                case PotionType.Strength:
                    player.Strength += 1;
                    Console.WriteLine($"{player.Name}'s Strength permanently increased by 1!");
                    break;
                case PotionType.Constitution:
                    player.Constitution += 1;
                    Console.WriteLine($"{player.Name}'s Constitution permanently increased by 1!");
                    break;
            }
        }
    }

}