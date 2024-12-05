using Godot;
using System;

public partial class CommandProcessor : Node
{
	
	public override void _Ready()
	{
	}

	public override void _Process(double delta)
	{
	}

	public int _Process_Command(string text)
	{
		if(int.TryParse(text, out int result))
		{
			return result;
		}
		else
		{
			return 0;
		}
	}
}
