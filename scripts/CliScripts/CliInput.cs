using Godot;
using System;

public partial class CliInput : LineEdit
{
	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		GrabFocus();
	}

	// Called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _Process(double delta)
	{
	}

	private void _on_dialog_input_text_changed(string newText)
	{
		Clear();
	}
}
