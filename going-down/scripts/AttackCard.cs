// using System;

// namespace GoingDown
// {
//     public class AttackCard : Item
//     {
//         public int AttackValue { get; private set; }
//         public int MPCost { get; private set; }
//         public int Depth { get; private set; }
//         public string Effect { get; private set; }

//         public AttackCard(string name, RarityType rarity, int depth, int mpCost, string effect = null)
//             : base(name, rarity)
//         {
//             Depth = depth;
//             MPCost = mpCost;
//             Effect = effect;
//             CalculateAttributes();
//         }

//         public override void CalculateAttributes()
//         {
//             int diceCount = GetDiceCount(Rarity);
//             AttackValue = RollDice(diceCount, 6); // Calculate attack value based on rarity.
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

//         public void Use()
//         {
//             Console.WriteLine($"Using {Name}: Damage {AttackValue}, MP Cost {MPCost}");
//             if (!string.IsNullOrEmpty(Effect))
//             {
//                 Console.WriteLine($"Effect: {Effect}");
//             }
//         }
//     }
// }