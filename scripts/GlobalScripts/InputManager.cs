using Godot;
using System;
using System.Collections.Generic;

namespace GoingDown
{
	public partial class InputManager : Node
	{
		public static InputManager Instance { get; private set; }	// Used to access this singleton in other scripts
		RichTextLabel dialog; 										// Used to Store Dialog box reference 
		private List<string> currentOptions = new List<string>();

		public override void _Ready()
		{
			Instance = this ;
			//Initailizing OptionManager to Load options from file.
			OptionManager.Ready();
			currentOptions = OptionManager.GetOptions(Utils.optionNames.out_of_combat.ToString());

			//Getting Dialog box reference from main scene.
			dialog = GetNode<RichTextLabel>("/root/main/Background/Margins/Rows/CliText/Dialog/VBoxContainer/RichTextLabel");
		}

		//This is used to get the text automatically from User.  
		public void OnTextSubmitted(string text)
		{
			GD.Print("The User Entered : " + text);
		}

		//This is a helper function to get a numbered list as a single string
		private string GetNumberedList(List<string> items)
		{
			if (items == null || items.Count == 0)
			{
				return "No options available.";
			}

			var result = new System.Text.StringBuilder();

			for (int i = 0; i < items.Count; i++)
			{
				result.AppendLine($"{i + 1}) {items[i]}");
			}

			return result.ToString();
		}
		public void ChangeText(string text)
		{
			dialog.Text = text;
		}
		public void ShowCurrentOptions()
		{
			ChangeText(GetCurrentOptions());
		} 
		public string GetCurrentOptions()
		{
			return GetNumberedList(currentOptions);
		}

	}
}