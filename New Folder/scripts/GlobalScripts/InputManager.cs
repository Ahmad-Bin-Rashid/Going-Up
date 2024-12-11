using Godot;
using System;
using System.Collections.Generic;

namespace GoingDown
{
	public partial class InputManager : Node
	{
		public static InputManager Instance { get; private set; }	// Used to access this singleton in other scripts
		private RichTextLabel dialog; 										// Used to Store Dialog box reference 
		
		
		//Manager references
		

		private List<string> currentOptions = new List<string>();

		public override void _Ready()
		{
			Instance = this ;
			
			//Initailizing OptionManager to Load options from file.
			OptionManager.Ready();
			currentOptions = OptionManager.GetOptions(Utils.GameState.Map_terversal.ToString());

			//Getting Dialog box reference from main scene.
			dialog = GetNode<RichTextLabel>("/root/main/Background/Margins/Rows/CliText/Dialog/VBoxContainer/RichTextLabel");
		}

		//This is used to get the text automatically from User.  
		public void OnTextSubmitted(string text)
		{
			int selected_option = ParseOptions(text);
			if(Global.Instance.currentGameState == Utils.GameState.Map_terversal)
			{
				Global.Instance.mapManager.MoveRooms(selected_option);
				EnterRoom(Global.Instance.mapManager.GetCurrentRoom());
			}
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
			if(Global.Instance.currentGameState == Utils.GameState.Menu)
			{
				currentOptions = OptionManager.GetOptions(Utils.GameState.Menu.ToString());
			}
			else if (Global.Instance.currentGameState == Utils.GameState.Combat)
			{
				currentOptions = OptionManager.GetOptions(Utils.GameState.Combat.ToString());
			}
			else if (Global.Instance.currentGameState == Utils.GameState.Attack_graph)
			{
				currentOptions = OptionManager.GetOptions(Utils.GameState.Attack_graph.ToString());
			}

			return GetNumberedList(currentOptions);
		}
		public int ParseOptions(string text)
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
		

		//Map functions
		public void EnterRoom(Room room)
		{
			string result= "";
			result = "Current Room: " + room.Type.ToString() + "\n" + room.GetRoomDescription() + "\n" + "\n" + GetNumberedList(room.GetConnectedRoomDoors()) + "\n" + "\n" +"Where Do you want to move my liege?"; 
			ChangeText(result);
		}

	}
}