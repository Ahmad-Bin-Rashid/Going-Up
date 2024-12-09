using System;
using System.Collections.Generic;
using Godot;

namespace GoingDown
{
    public class LargeRoom : Room
    {
        public Enemy Enemy { get; private set; }

        private Random random = new();

        public LargeRoom((int, int) position)
            : base(Utils.RoomType.Large, position) 
        {
            random = new Random();
            Utils.EnemyType enemyType = (EnemyType)random.Next(1, Enum.GetValues(typeof(EnemyType)).Length);

            switch (enemyType)
            {
                case Utils.EnemyType.GoblinScout:
                    Enemy = new GoblinScout();
                    break;
                case Utils.EnemyType.SkeletonWarrior:
                    Enemy = new SkeletonWarrior();
                    break;
                case Utils.EnemyType.CaveSpider:
                    Enemy = new CaveSpider();
                    break;
            }

            GD.Print($"Large Room created with {Enemy.Name}");
        }

        public Enemy GetEnemy()
        {
            return Enemy;
        }

        public Enemy RemoveEnemy()
        {
            var temp = Enemy;
            Enemy = null;
            return temp;
        }
    }   
}
