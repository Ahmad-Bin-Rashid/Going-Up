using System;
using System.Collections.Generic;

namespace GoingDown
{
    public class Goblin : Enemy
    {
        public Goblin(string name, int hp, int constitution, int strength) : base(name, hp, constitution, strength)
        {
        }

        public override void Attack(Player player, Card card)
        {
            int damage = card.Damage + Strength;

            player.TakeDamage(damage);
        }

        public override void Defend(Player player, Card card)
        {
            int damage = card.Damage - Constitution;

            TakeDamage(damage);
        }
    }
}