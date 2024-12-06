using System;
using System.Collections.Generic;

namespace GoingDown
{
    public abstract class Enemy
    {
        // Attributes
        public string Name { get; private set; }
        public int HP { get; private set; }
        public int MaxHP { get; private set; }
        public int Constitution { get; set; }
        public int BaseDamage { get; private set; }
        public List<Card> AttackCards { get; private set; } = new List<Card>();

        protected Random random = new Random();

        // Constructor
        protected Enemy(string name, int hp, int constitution, int baseDamage)
        {
            Name = name;
            MaxHP = hp;
            HP = hp;
            Constitution = constitution;
            BaseDamage = baseDamage;
        }

        // Methods

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

        // Determines which card to use (basic AI)
        public virtual Card ChooseAttack()
        {
            if (AttackCards.Count == 0)
            {
                Console.WriteLine($"{Name} has no attack cards!");
                return null;
            }

            int index = random.Next(AttackCards.Count);
            return AttackCards[index];
        }

        // Executes the attack logic
        public virtual void PerformAttack(Player player)
        {
            Card attackCard = ChooseAttack();
            if (attackCard == null) return;

            int damage = attackCard.CalculateDamage(BaseDamage);
            player.TakeDamage(damage);
            Console.WriteLine($"{Name} used {attackCard.Name}, dealing {damage} damage to {player.Name}!");

            if (!string.IsNullOrEmpty(attackCard.Effect))
            {
                Console.WriteLine($"Effect: {attackCard.Effect}");
            }
        }

        public bool IsAlive => HP > 0;
    }
}