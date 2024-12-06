// using System;
// using System.Collections.Generic;

// namespace GoingDown
// {
//     public class Player : Character
//     {
//         public int Level { get; private set; } = 1;
//         public List<AttackCard> Deck { get; private set; } = new List<AttackCard>();
//         public List<Item> Inventory { get; private set; } = new List<Item>();
//         public Weapon EquippedWeapon { get; private set; }
//         public Armor EquippedArmor { get; private set; }

//         public Player(string name, int hp, int mp, int strength, int constitution, int ac)
//             : base(name, hp, mp, strength, constitution, ac) { }

//         public void EquipWeapon(Weapon weapon)
//         {
//             EquippedWeapon = weapon;
//             Strength += weapon.Damage;
//         }

//         public void EquipArmor(Armor armor)
//         {
//             EquippedArmor = armor;
//             Constitution += armor.ConstitutionModifier;
//         }

//         public void AddCardToDeck(AttackCard card)
//         {
//             Deck.Add(card);
//         }

//         public void LevelUp()
//         {
//             Level++;
//             MaxHP += 10;
//             HP = MaxHP;
//             MaxMP += 5;
//             MP = MaxMP;
//             Strength++;
//             Constitution++;
//         }

//         public override void EndTurn()
//         {
//             base.EndTurn();
//             // Additional player-specific end-turn logic.
//         }
//     }
// }


