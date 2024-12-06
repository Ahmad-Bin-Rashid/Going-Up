using System;

namespace GoingDown
{
    public class AttackCard : Item
    {
        public int AttackValue { get; private set; }

        public AttackCard(string name, RarityType rarity)
            : base(name, rarity)
        {
            CalculateAttributes();
        }

        public override void CalculateAttributes()
        {
            int diceCount = GetDiceCount(Rarity);
            AttackValue = RollDice(diceCount, 6);
        }

        private int GetDiceCount(RarityType rarity)
        {
            return rarity switch
            {
                RarityType.Common => 1,
                RarityType.Uncommon => 2,
                RarityType.Rare => 4,
                RarityType.Legendary => 6,
                _ => 1
            };
        }
    }

}