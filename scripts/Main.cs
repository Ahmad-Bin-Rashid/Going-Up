using Godot;
using System;

namespace GoingDown
{
	public partial class Main : Node
	{
		CommandProcessor commandProcessor;
		// THIS IS EXECUTED ON PROGRAM START ........
		public override void _Ready()
		{

			// This gets a reference to command Processor (Do not change it my childern)
			Node commandProcessor_node = FindChild("CommandProcessor",true ,false);
			if (commandProcessor_node is CommandProcessor commandProcessor_node_type_casted )
			{
				commandProcessor = commandProcessor_node_type_casted;
			}
			InputManager.Instance.ShowCurrentOptions();

		}

		// THIS IS MAIN GAME LOOP THAT IS EXECUTED ........
		public override void _Process(double delta)
		{

		}

	
	}
}