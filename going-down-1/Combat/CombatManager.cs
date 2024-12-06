using System;

namespace GoingDown
{
    public class CombatManager
    {
        // Attributes
        private Player player;
        private Enemy enemy;
        private bool isPlayerTurn;

        // Constructor
        public CombatManager(Player player, Enemy enemy)
        {
            this.player = player;
            this.enemy = enemy;
            isPlayerTurn = true;
        }

        // Starts the combat loop
        public void StartCombat()
        {
            Console.WriteLine($"Combat started: {player.Name} vs {enemy.Name}");

            while (player.IsAlive && enemy.IsAlive)
            {
                if (isPlayerTurn)
                {
                    Console.WriteLine("\n--- Player's Turn ---");
                    PlayerTurn();
                }
                else
                {
                    Console.WriteLine("\n--- Enemy's Turn ---");
                    EnemyTurn();
                }

                // Switch turns
                isPlayerTurn = !isPlayerTurn;
            }

            Console.WriteLine(player.IsAlive ? "Player Wins!" : $"{enemy.Name} Wins!");
        }

        // Handles the player's turn
        private void PlayerTurn()
        {
            Console.WriteLine($"Player HP: {player.HP}/{player.MaxHP} | MP: {player.MP}/{player.MaxMP}");
            Console.WriteLine($"Enemy HP: {enemy.HP}/{enemy.MaxHP}\n");

            Console.WriteLine("Available Cards:");
            for (int i = 0; i < player.Deck.Count; i++)
            {
                Console.WriteLine($"{i + 1}. {player.Deck[i]}");
            }

            Console.Write("\nChoose a card (or enter 0 to pass): ");
            int choice = int.Parse(Console.ReadLine() ?? "0");

            if (choice == 0 || choice > player.Deck.Count)
            {
                Console.WriteLine($"{player.Name} chooses to pass.");
                return;
            }

            Card selectedCard = player.Deck[choice - 1];
            if (player.MP < selectedCard.MPCost)
            {
                Console.WriteLine("Not enough MP to use this card!");
                return;
            }

            player.UseCard(selectedCard, enemy);
        }

        // Handles the enemy's turn
        private void EnemyTurn()
        {
            if (enemy.IsAlive)
            {
                enemy.PerformAttack(player);
            }
        }
    }

}