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
        public Weapon EquippedWeapon { get; private set; }
        public Armor EquippedArmor { get; private set; }
        public Inventory Inventory { get; private set; }

        private Random random = new();

        public Player(string name, int hp, int mp, int strength, int constitution)
        {
            Name = name;
            MaxHP = hp;
            HP = hp;
            MaxMP = mp;
            MP = mp;
            Strength = strength;
            Constitution = constitution;
            Inventory = new Inventory();
        }

        public bool IsAlive => HP > 0;

        public void EquipWeapon(Weapon weapon)
        {
            EquippedWeapon = weapon;
        }

        public void EquipArmor(Armor armor)
        {
            EquippedArmor = armor;
        }

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

        public void RestoreMP(int amount)
        {
            MP += amount;
            if (MP > MaxMP)
            {
                MP = MaxMP;
            }
        }
    }

}