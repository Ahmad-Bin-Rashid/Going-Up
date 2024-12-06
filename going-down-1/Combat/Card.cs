using System;

namespace GoingDown
{
    public class Card
    {
        // Attributes
        public string Name { get; private set; }
        public int BaseDamage { get; private set; }
        public int MPCost { get; private set; }
        public int Depth { get; private set; } // Depth in combo graph
        public string Effect { get; private set; }

        // Constructor
        public Card(string name, int baseDamage, int mpCost, int depth, string effect = null)
        {
            Name = name;
            BaseDamage = baseDamage;
            MPCost = mpCost;
            Depth = depth;
            Effect = effect;
        }

        // Calculates the total damage dealt by this card
        public int CalculateDamage(int strength)
        {
            return BaseDamage + strength; // Adds the player's strength to base damage
        }

        public override string ToString()
        {
            return $"{Name} (Damage: {BaseDamage}, MP Cost: {MPCost}, Depth: {Depth}, Effect: {Effect})";
        }
    }

}