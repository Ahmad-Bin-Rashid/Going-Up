using System;
using System.Collections.Generic;

namespace GoingDown
{
    public class BossRoom : Room
    {
        public Enemy Boss { get; private set; }

        public BossRoom((int, int) position, Enemy boss)
            : base(RoomType.Boss, position)
        {
            Boss = boss;
        }
    }

}