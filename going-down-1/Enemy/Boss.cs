using System;
using System.Collections.Generic;

namespace GoingDown
{
    public class Boss : Enemy
    {
        // Additional attributes for bosses
        public List<SpecialAbility> SpecialAbilities { get; private set; } = new List<SpecialAbility>();

        public Boss(string name, int hp, int constitution, int baseDamage)
            : base(name, hp, constitution, baseDamage)
        {
        }

        // Use a special ability
        public void UseSpecialAbility(Player player)
        {
            if (SpecialAbilities.Count == 0)
            {
                Console.WriteLine($"{Name} has no special abilities!");
                return;
            }

            int index = random.Next(SpecialAbilities.Count);
            SpecialAbility ability = SpecialAbilities[index];
            Console.WriteLine($"{Name} used special ability: {ability.Name}");
            ability.Execute(player, this);
        }

        // Overrides PerformAttack to include special abilities
        public override void PerformAttack(Player player)
        {
            if (random.Next(1, 101) <= 30) // 30% chance to use a special ability
            {
                UseSpecialAbility(player);
            }
            else
            {
                base.PerformAttack(player);
            }
        }
    }

}