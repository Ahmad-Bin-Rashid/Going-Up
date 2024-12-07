namespace GoingDown
{
    public abstract class Encounter
    {
        public string Description { get; protected set; }

        public Encounter(string description)
        {
            Description = description;
        }

        // Abstract method to handle what happens when the player interacts with the encounter
        public abstract void Resolve(Player player);
    }
}
