using System;
using System.Collections.Generic;

namespace GoingDown
{
    public class GameLogic
    {
        private Player player;
        private Floor currentFloor;
        private Room currentRoom;

        // Starts the game
        public void StartGame()
        {
            Console.WriteLine("Welcome to Going Down!");

            // Initialize player
            player = new Player("Hero", 50, 20, 5, 3);

            // Generate a single floor
            currentFloor = new Floor();
            currentFloor.GenerateFloorGraph();

            // Start at the spawn room
            currentRoom = currentFloor.SpawnRoom;
            Console.WriteLine("\nYou are in the Spawn Room.");
            DisplayRoomOptions();

            // Main game loop
            while (player.IsAlive && currentRoom != currentFloor.BossRoom)
            {
                PlayerMove();
            }

            // Final boss encounter
            if (player.IsAlive)
            {
                Console.WriteLine("\nYou have reached the Boss Room!");
                CombatManager bossCombat = new CombatManager(player, ((BossRoom)currentRoom).Boss);
                bossCombat.StartCombat();

                if (player.IsAlive)
                {
                    Console.WriteLine("\nCongratulations! You have defeated the boss and won the game!");
                }
                else
                {
                    Console.WriteLine("\nThe boss has defeated you. Game over.");
                }
            }
            else
            {
                Console.WriteLine("\nYou have perished. Game over.");
            }
        }

        // Displays connected rooms and their types
        private void DisplayRoomOptions()
        {
            Console.WriteLine("\nConnected Rooms:");
            for (int i = 0; i < currentRoom.Connections.Count; i++)
            {
                Room connectedRoom = currentRoom.Connections[i];
                Console.WriteLine($"{i + 1}. {connectedRoom.Type} Room at {connectedRoom.Position}");
            }
        }

        // Handles player movement between rooms
        private void PlayerMove()
        {
            DisplayRoomOptions();
            Console.Write("\nChoose a room to enter (1, 2, ...): ");
            int choice = int.Parse(Console.ReadLine() ?? "1") - 1;

            if (choice < 0 || choice >= currentRoom.Connections.Count)
            {
                Console.WriteLine("Invalid choice. Try again.");
                return;
            }

            currentRoom = currentRoom.Connections[choice];
            Console.WriteLine($"\nYou have entered a {currentRoom.Type} Room.");

            HandleRoomEvents();
        }

        // Triggers events based on the room type
        private void HandleRoomEvents()
        {
            switch (currentRoom)
            {
                case TreasureRoom treasureRoom:
                    HandleTreasureRoom(treasureRoom);
                    break;
                case TrapRoom trapRoom:
                    HandleTrapRoom(trapRoom);
                    break;
                case LargeRoom largeRoom:
                    HandleLargeRoom(largeRoom);
                    break;
                case BossRoom bossRoom:
                    Console.WriteLine("Prepare for the final battle!");
                    break;
                default:
                    Console.WriteLine("This room is empty. You can move on.");
                    break;
            }
        }

        private void HandleTreasureRoom(TreasureRoom treasureRoom)
        {
            Item treasure = treasureRoom.GenerateLoot();
            Console.WriteLine($"You found a treasure: {treasure.Name}");
            player.AddItemToInventory(treasure);
        }

        private void HandleTrapRoom(TrapRoom trapRoom)
        {
            if (player.RollEvasion())
            {
                Console.WriteLine("You evaded the trap!");
            }
            else
            {
                int trapDamage = DiceRoller.Roll(1, 6); // Example: 1d6 damage
                Console.WriteLine($"You triggered a trap and took {trapDamage} damage!");
                player.TakeDamage(trapDamage);
            }
        }

        private void HandleLargeRoom(LargeRoom largeRoom)
        {
            Console.WriteLine("This is a large room with multiple encounters.");
            largeRoom.GenerateEncounters();

            foreach (var encounter in largeRoom.Encounters)
            {
                switch (encounter)
                {
                    case EnemyEncounter enemyEncounter:
                        Console.WriteLine($"An enemy {enemyEncounter.Enemy.Name} appears!");
                        CombatManager combat = new CombatManager(player, enemyEncounter.Enemy);
                        combat.StartCombat();

                        if (!player.IsAlive) return;
                        break;

                    case Trap trap:
                        Console.WriteLine("You triggered a trap!");
                        Console.WriteLine($"The trap dealt {trap.Damage} damage.");
                        player.TakeDamage(trap.Damage);
                        break;

                    case TreasureEncounter treasureEncounter:
                        Console.WriteLine("You found a treasure!");
                        treasureEncounter.Resolve(player);
                        break;

                    default:
                        Console.WriteLine("You found nothing unusual.");
                        break;
                }
            }
        }
    }
}
