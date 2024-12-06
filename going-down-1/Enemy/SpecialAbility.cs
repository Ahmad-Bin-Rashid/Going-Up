using System;

namespace GoingDown
{
    public class SpecialAbility
    {
        public string Name { get; private set; }
        public Action<Player, Enemy> Execute { get; private set; }

        public SpecialAbility(string name, Action<Player, Enemy> execute)
        {
            Name = name;
            Execute = execute;
        }
    }
}