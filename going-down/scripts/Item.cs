// using System;

// namespace GoingDown
// {
//     public abstract class Item
//     {
//         public enum RarityType { Common, Uncommon, Rare, Legendary }
//         public string Name { get; set; }
//         public RarityType Rarity { get; set; }

//         protected Item(string name, RarityType rarity)
//         {
//             Name = name;
//             Rarity = rarity;
//         }

//         public abstract void CalculateAttributes();

//         // Utility: Roll dice
//         protected int RollDice(int numberOfDice, int diceType)
//         {
//             Random random = new();
//             int total = 0;
//             for (int i = 0; i < numberOfDice; i++)
//             {
//                 total += random.Next(1, diceType + 1); // 1 to diceType
//             }
//             return total;
//         }
//     }
// }