using System;
using System.Collections.Generic;

namespace GoingDown
{
    public class Boss : Enemy
    {
        public List<SpecialAbility> SpecialAbilities { get; private set; } = new List<SpecialAbility>();

        public Boss(string name, int hp, int constitution, int strength) : base(name, hp, constitution, strength)
        {
        }

        public void AddSpecialAbility(SpecialAbility specialAbility)
        {
            SpecialAbilities.Add(specialAbility);
        }

        public void RemoveSpecialAbility(SpecialAbility specialAbility)
        {
            SpecialAbilities.Remove(specialAbility);
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