using System;
using System.Collections.Generic;

namespace GoingDown
{
    public abstract class Enemy
    {
        public string Name { get; private set; }
        public int HP { get; private set; }
        public int MaxHP { get; private set; }
        public int Constitution { get; set; }
        public int Strength { get; set; }
        public List<Card> AttackCards { get; private set; } = new List<Card>();

        protected Random random = new Random();

        protected Enemy(string name, int hp, int constitution, int Strength)
        {
            Name = name;
            MaxHP = hp;
            HP = hp;
            Constitution = constitution;
            Strength = Strength;
        }

        public bool IsAlive => HP > 0;

        public void TakeDamage(int damage)
        {
            HP -= damage;
            if (HP < 0)
            {
                HP = 0;
            }
        }

        public void Heal(int amount)
        {
            HP += amount;
            if (HP > MaxHP)
            {
                HP = MaxHP;
            }
        }

        public void AddAttackCard(Card card)
        {
            AttackCards.Add(card);
        }

        public void RemoveAttackCard(Card card)
        {
            AttackCards.Remove(card);
        }

        public abstract void Attack(Player player, Card card);

        public abstract void Defend(Player player, Card card);
    }
}