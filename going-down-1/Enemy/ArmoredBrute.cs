using System;
using System.Collections.Generic;

namespace GoingDown
{
    public class ArmoredBrute : Boss
    {
        public ArmoredBrute()
            : base("Armored Brute", 30, 15, 2)
        {
            // Add specific attack cards
            AttackCards.Add(new Card("Heavy Slam", 2, 0, 1));
            AttackCards.Add(new Card("Ground Stomp", 2, 0, 1, "Reduces player's next attack roll by 2"));

            // Add special abilities
            SpecialAbilities.Add(new SpecialAbility(
                "Shield Bash",
                (player, self) =>
                {
                    int damage = self.BaseDamage + 2;
                    player.TakeDamage(damage);
                    Console.WriteLine($"{self.Name}'s Shield Bash deals {damage} damage and increases AC by 2!");
                    self.Constitution += 2;
                }
            ));
        }
    }
}