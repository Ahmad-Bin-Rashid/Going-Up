using Godot;
using System;
namespace GoingDown
{
	public partial class CliInput : LineEdit
	{

		public override void _Ready()
		{
			//This makes so that the TextBox is always selected when game starts.
			GrabFocus();		
		}

		private void _on_dialog_input_text_changed(string newText)
		{
			// This sends the entered text to InputManager. 
			InputManager.Instance.OnTextSubmitted(newText);
			//This clears the TextBox.
			Clear();
		}
	}
}