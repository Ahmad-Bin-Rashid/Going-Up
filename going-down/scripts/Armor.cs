// using System;

// namespace GoingDown
// {
//     public class Armor : Item
//     {
//         public enum ArmorType { Helmet, Boots, Chestplate }
//         public ArmorType Type { get; set; }
//         public int AttributeBonus { get; private set; }

//         public Armor(string name, RarityType rarity, ArmorType type)
//             : base(name, rarity)
//         {
//             Type = type;
//             CalculateAttributes();
//         }

//         public override void CalculateAttributes()
//         {
//             int diceCount = GetDiceCount(Rarity);
//             AttributeBonus = RollDice(diceCount, 6);
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