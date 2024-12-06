// using System;

// namespace GoingDown
// {
//     public class ItemGenerator
//     {
//         private Random random = new();

//         public Item.RarityType GenerateRarity(int playerLevel)
//         {
//             int commonThreshold = Math.Max(50 - playerLevel * 4, 10);
//             int uncommonThreshold = 30;
//             int rareThreshold = 15 + playerLevel * 2;
//             int legendaryThreshold = 5 + playerLevel * 2;

//             int roll = random.Next(1, 101); // Roll a d100
//             if (roll <= commonThreshold) return Item.RarityType.Common;
//             if (roll <= commonThreshold + uncommonThreshold) return Item.RarityType.Uncommon;
//             if (roll <= commonThreshold + uncommonThreshold + rareThreshold) return Item.RarityType.Rare;
//             return Item.RarityType.Legendary;
//         }

//         public Item GenerateItem(int playerLevel)
//         {
//             Item.RarityType rarity = GenerateRarity(playerLevel);

//             // Randomly pick item type
//             int itemTypeRoll = random.Next(1, 5); // 1-4 for Weapon, Armor, Potion, AttackCard
//             return itemTypeRoll switch
//             {
//                 1 => new Weapon("Random Weapon", rarity, (Weapon.WeaponType)random.Next(0, 4)),
//                 2 => new Armor("Random Armor", rarity, (Armor.ArmorType)random.Next(0, 3)),
//                 3 => new Potion("Random Potion", rarity, (Potion.PotionType)random.Next(0, 4)),
//                 4 => new AttackCard("Random Attack Card", rarity, 3, 5),
//                 _ => null
//             };
//         }
//     }

// }




// /********************************************************
//  * The following code is used to test the item classes. *
//  ********************************************************/
// // class Program
// // {
// //     static void Main()
// //     {
// //         ItemGenerator generator = new ItemGenerator();

// //         for (int level = 1; level <= 10; level++)
// //         {
// //             Console.WriteLine($"Player Level {level}:");
// //             for (int i = 0; i < 5; i++)
// //             {
// //                 Item item = generator.GenerateItem(level);
// //                 Console.WriteLine($"{item.Name} ({item.Rarity})");
// //             }
// //             Console.WriteLine();
// //         }
// //     }
// // }
