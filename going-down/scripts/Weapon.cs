// using System;

// namespace GoingDown
// {
//     public class Weapon : Item
//     {
//         public enum WeaponType { Sword, Dagger, Mace, Bow }
//         public WeaponType Type { get; set; }
//         public int Damage { get; private set; }

//         public Weapon(string name, RarityType rarity, WeaponType type)
//             : base(name, rarity)
//         {
//             Type = type;
//             CalculateAttributes();
//         }

//         public override void CalculateAttributes()
//         {
//             int diceCount = GetDiceCount(Rarity);
//             int diceType = Type switch
//             {
//                 WeaponType.Sword => 6,
//                 WeaponType.Dagger => 4,
//                 WeaponType.Mace => 8,
//                 WeaponType.Bow => 6,
//                 _ => 6
//             };
//             Damage = RollDice(diceCount, diceType);
//         }

//         private static int GetDiceCount(RarityType rarity)
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