using Godot;
using System;
using System.Transactions;

namespace GoingDown
{
	public partial class Main : Node
	{
		
		// THIS IS EXECUTED ON PROGRAM START ........
		public override void _Ready()
		{

			// This gets a reference to command Processor (Do not change it my children)
			
			//Moved stuff to Global.cs in scripts/GlobalScripts/Global.cs
			InputManager.Instance.EnterRoom(Global.Instance.mapManager.GetCurrentRoom());

		}

		// THIS IS MAIN GAME LOOP THAT IS EXECUTED ........
		public override void _Process(double delta)
		{

		}

	
	}
}