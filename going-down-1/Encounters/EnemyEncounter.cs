using System;

namespace GoingDown
{
    public class EnemyEncounter : Encounter
    {
        public Enemy Enemy { get; private set; }

        public EnemyEncounter(Enemy enemy)
            : base($"An enemy {enemy.Name} appears!")
        {
            Enemy = enemy;
        }

        public override void Resolve(Player player)
        {
            Console.WriteLine(Description);
            CombatManager combat = new CombatManager(player, Enemy);
            combat.StartCombat();
        }
    }
}
