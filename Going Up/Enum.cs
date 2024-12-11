namespace GoingUp
{
    public enum RoomType
    {
        Spawn,
        Boss,
        Large,
        Treasure,
        Trap
    }
	public enum TreasureType
	{
		Armor,
		Weapon,
		Potion
	}
	public enum TrapType
	{
		Fire,
		Ice,
		Poison
	}
	public enum EnemyType
	{
		GoblinScout,
		SkeletonWarrior,
		CaveSpider
	}
	public enum GameState
	{
		Menu,
		Map_terversal,
		Combat,
		Attack_graph
	}
}