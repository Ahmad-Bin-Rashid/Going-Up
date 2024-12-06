// using System;

// namespace GoingDown
// {
//     public class Potion : Item
//     {
//         public enum PotionType { Healing, Mana, Strength, Constitution }
//         public PotionType Type { get; set; }
//         public int Effect { get; private set; }

//         public Potion(string name, RarityType rarity, PotionType type)
//             : base(name, rarity)
//         {
//             Type = type;
//             CalculateAttributes();
//         }

//         public override void CalculateAttributes()
//         {
//             int diceCount = GetDiceCount(Rarity);
//             Effect = RollDice(diceCount, 6);
//         }

//         private int GetDiceCount(RarityType rarity)
//         {
//             return rarity switch
//             {
//                 RarityType.Common => 1,
//                 RarityType.Uncommon => 2,
//                 RarityType.Rare => 4,
//                 RarityType.Legendary => 6,
//                 _ => 1
//             };
//         }
//     }

// }