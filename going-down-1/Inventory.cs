using System;

namespace GoingDown
{
    public class Inventory
    {
        public List<Card> Cards { get; private set; }
        public List<Item> Items { get; private set; }

        public Inventory()
        {
            Cards = new List<Card>();
            Items = new List<Item>();
        }

        public void AddCard(Card card)
        {
            Cards.Add(card);
        }

        public void AddItem(Item item)
        {
            Items.Add(item);
        }

        public void RemoveCard(Card card)
        {
            Cards.Remove(card);
        }

        public void RemoveItem(Item item)
        {
            Items.Remove(item);
        }
    }
}