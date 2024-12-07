using Godot;
using System;

namespace GoingDown
{
public partial class Utils : Node
{
	// This script is used to Make enums and utility functions that will be used all over the game.

	public enum RoomType
    {
        Spawn,
        Boss,
        Large,
        Treasure,
        Trap
    }
	public enum GameState
	{
		Menu,
		Map_terversal,
		Combat,
		Attack_graph
		

	}
	public override void _Ready()
	{
	}


}
}