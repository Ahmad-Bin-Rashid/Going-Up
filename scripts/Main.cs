using Godot;
using System;

public partial class Main : Node
{
	Dialog dialog; 							//Dialog object (check Dialog.cs for functions)
	CommandProcessor commandProcessor;
	// THIS IS EXECUTED ON PROGRAM START ........
	public override void _Ready()
	{

		// This gets a reference to dialog box (Do not change it my childern)
		RichTextLabel dialog_node = GetNode<RichTextLabel>("Background/Margins/Rows/CliText/Dialog/VBoxContainer/RichTextLabel");
		if (dialog_node is Dialog dialog_node_type_casted)  // is is used for type casting
		{
			dialog = dialog_node_type_casted;
		}

		// This gets a reference to command Processor (Do not change it my childern)
		Node commandProcessor_node = FindChild("CommandProcessor",true ,false);
		if (commandProcessor_node is CommandProcessor commandProcessor_node_type_casted )
		{
			commandProcessor = commandProcessor_node_type_casted;
		}
		dialog.ChangeText(dialog.GetPlayerMoveCategoryOptions(1));

	}

	// THIS IS MAIN GAME LOOP THAT IS EXECUTED ........
	public override void _Process(double delta)
	{

	}

	
	private void _on_dialog_input_text_changed(string newText)
	{
		int response = commandProcessor._Process_Command(newText);
		if (response == 0)
		{
			return;
		}
		dialog.ChangeText(response.ToString());
	}
}
