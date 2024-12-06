using System;
using System.Collections.Generic;

namespace GoingDown
{
    public abstract class Item
    {
        // Attributes
        public string Name { get; private set; }
        public RarityType Rarity { get; private set; }
        public string Description { get; protected set; }

        // Constructor
        protected Item(string name, RarityType rarity, string description = "")
        {
            Name = name;
            Rarity = rarity;
            Description = description;
        }

        // Abstract method for item-specific behavior
        public abstract void Use(Player player);

        public override string ToString()
        {
            return $"{Name} ({Rarity}) - {Description}";
        }
    }

    public enum RarityType
    {
        Common,
        Uncommon,
        Rare,
        Legendary
    }
}