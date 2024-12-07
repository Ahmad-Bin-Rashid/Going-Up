using System;

namespace GoingDown
{
    public class TreasureEncounter : Encounter
    {
        public Item Treasure { get; private set; }

        public TreasureEncounter(Item treasure)
            : base("You found a treasure!")
        {
            Treasure = treasure;
        }

        public override void Resolve(Player player)
        {
            Console.WriteLine(Description);
            Console.WriteLine($"You received: {Treasure.Name}");
            player.AddItemToInventory(Treasure);
        }
    }
}
