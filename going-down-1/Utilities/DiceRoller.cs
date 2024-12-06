using System;

namespace GoingDown
{
    public static class DiceRoller
    {
        private static Random random = new();
        
        public static int Roll(int diceCount, int diceType)
        {
            int total = 0;
            for (int i = 0; i < diceCount; i++)
            {
                total += random.Next(1, diceType + 1);
            }
            return total;
        }

        public static int RollPercentile()
        {
            return random.Next(1, 101);
        }
    }

}