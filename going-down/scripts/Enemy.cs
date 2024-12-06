// using System;
// using System.Collections.Generic;

// namespace GoingDown 
// {
//     public class Enemy : Character
//     {
//         public List<AttackCard> AttackCards { get; private set; } = new List<AttackCard>();

//         public Enemy(string name, int hp, int mp, int strength, int constitution, int ac, List<AttackCard> attackCards)
//             : base(name, hp, mp, strength, constitution, ac)
//         {
//             AttackCards.AddRange(attackCards);
//         }

//         public AttackCard ChooseAttack()
//         {
//             // Simple AI: Randomly select an attack card.
//             Random rand = new Random();
//             return AttackCards[rand.Next(AttackCards.Count)];
//         }

//         public override void EndTurn()
//         {
//             base.EndTurn();
//         }
//     }
// }
