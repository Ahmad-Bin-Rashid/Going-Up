using System;


namespace GoingDown
{
    public class Trap : Encounter
    {
        public int Damage { get; private set; }

        public Trap(int damage)
            : base("You triggered a trap!")
        {
            Damage = damage;
        }

        public override void Resolve(Player player)
        {
            Console.WriteLine(Description);

            if (player.RollEvasion())
            {
                Console.WriteLine("You successfully evaded the trap!");
            }
            else
            {
                Console.WriteLine($"You failed to evade the trap and took {Damage} damage.");
                player.TakeDamage(Damage);
            }
        }
    }
}
