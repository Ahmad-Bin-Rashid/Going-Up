using System;
using System.Collections.Generic;

namespace GoingDown
{
    public class Goblin : Enemy
    {
        public Goblin()
            : base("Goblin Scout", 15, 11, 1)
        {
            // Add specific attack cards
            AttackCards.Add(new Card("Slash", 1, 0, 1));
            AttackCards.Add(new Card("Quick Jab", 1, 0, 1));
            AttackCards.Add(new Card("Dodge", 0, 2, 0, "Increases AC by 2 for next turn"));
        }

        public override void PerformAttack(Player player)
        {
            Console.WriteLine($"{Name} is preparing to attack!");
            base.PerformAttack(player);
        }
    }
}