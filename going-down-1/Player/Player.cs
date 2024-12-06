using System;
using System.Collections.Generic;

namespace GoingDown
{
    public class Player
    {
        // Attributes
        public string Name { get; private set; }
        public int HP { get; private set; }
        public int MaxHP { get; private set; }
        public int MP { get; private set; }
        public int MaxMP { get; private set; }
        public int Strength { get; set; }
        public int Constitution { get; set; }
        public int Level { get; private set; }
        public int Experience { get; private set; }
        public Weapon EquippedWeapon { get; private set; }
        public Armor EquippedArmor { get; private set; }
        public List<Item> Inventory { get; private set; } = new List<Item>();
        public List<Card> Deck { get; private set; } = new List<Card>();

        private Random random = new();

        // Constructor
        public Player(string name, int hp, int mp, int strength, int constitution)
        {
            Name = name;
            MaxHP = hp;
            HP = hp;
            MaxMP = mp;
            MP = mp;
            Strength = strength;
            Constitution = constitution;
            Level = 1;
            Experience = 0;
        }

        public bool IsAlive => HP > 0;

        // Takes damage, applying Constitution as damage reduction
        public void TakeDamage(int damage)
        {
            int reducedDamage = Math.Max(damage - Constitution, 0);
            HP -= reducedDamage;
            Console.WriteLine($"{Name} took {reducedDamage} damage. HP: {HP}/{MaxHP}");
            if (HP <= 0)
            {
                Console.WriteLine($"{Name} has been defeated!");
            }
        }

        // Heals the player
        public void Heal(int amount)
        {
            HP = Math.Min(HP + amount, MaxHP);
            Console.WriteLine($"{Name} healed for {amount}. HP: {HP}/{MaxHP}");
        }

        // Restores MP
        public void RestoreMP(int amount)
        {
            MP = Math.Min(MP + amount, MaxMP);
            Console.WriteLine($"{Name} restored {amount} MP. MP: {MP}/{MaxMP}");
        }

        // Level up the player and increase stats
        public void LevelUp()
        {
            Level++;
            MaxHP += 10;
            MaxMP += 5;
            Strength++;
            Constitution++;
            HP = MaxHP;
            MP = MaxMP;
            Console.WriteLine($"{Name} leveled up to Level {Level}! Stats increased.");
        }

        // Adds experience and levels up if needed
        public void GainExperience(int amount)
        {
            Experience += amount;
            Console.WriteLine($"{Name} gained {amount} EXP.");
            if (Experience >= Level * 10) // Example scaling: Level * 10 EXP to level up
            {
                Experience -= Level * 10;
                LevelUp();
            }
        }

        // Equips a weapon
        public void EquipWeapon(Weapon weapon)
        {
            EquippedWeapon = weapon;
            Console.WriteLine($"{Name} equipped {weapon.Name}.");
        }

        // Equips armor
        public void EquipArmor(Armor armor)
        {
            EquippedArmor = armor;
            Console.WriteLine($"{Name} equipped {armor.Name}.");
        }

        // Adds an item to inventory
        public void AddItemToInventory(Item item)
        {
            Inventory.Add(item);
            Console.WriteLine($"{Name} received {item.Name}.");
        }

        // Uses a card in combat
        public void UseCard(Card card, Enemy target)
        {
            if (MP < card.MPCost)
            {
                Console.WriteLine("Not enough MP to use this card!");
                return;
            }

            MP -= card.MPCost;
            int damage = card.CalculateDamage(Strength);
            target.TakeDamage(damage);
            Console.WriteLine($"{Name} used {card.Name} and dealt {damage} damage to {target.Name}.");

            if (!string.IsNullOrEmpty(card.Effect))
            {
                Console.WriteLine($"Effect: {card.Effect}");
            }
        }

        // Performs an evasion roll for traps
        public bool RollEvasion()
        {
            int roll = random.Next(1, 21); // Roll a d20
            bool success = roll + Constitution >= 15; // Example threshold: 15
            Console.WriteLine(success ? "Evasion successful!" : "Evasion failed.");
            return success;
        }

        // Displays player stats
        public void DisplayStats()
        {
            Console.WriteLine($"\n{Name} - Level {Level}");
            Console.WriteLine($"HP: {HP}/{MaxHP} | MP: {MP}/{MaxMP}");
            Console.WriteLine($"Strength: {Strength} | Constitution: {Constitution}");
            Console.WriteLine($"Equipped Weapon: {EquippedWeapon?.Name ?? "None"}");
            Console.WriteLine($"Equipped Armor: {EquippedArmor?.Name ?? "None"}");
            Console.WriteLine($"Experience: {Experience}/{Level * 10}");
        }
    }

}