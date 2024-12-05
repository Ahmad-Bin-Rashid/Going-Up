using Godot;
using System;

public partial class Dialog : RichTextLabel
{

	public override void _Ready()
	{
	}

	public override void _Process(double delta)
	{
	}
	public void ChangeText(string text)
	{
		Text = text;
	}
	public string GetPlayerMoveCategoryOptions(int choice)
	{
		string result = "";
		string[] options = GetAvailableMoveCategoryOptions();
		if (choice == 1)
		{
			result = ">> " + options[0] + "\n" + "   " + options[1] + "\n" + "   " + options[2] + "\n";
		}
		else if (choice == 2)
		{
			result = "   " + options[0] + "\n" + ">> " + options[1] + "\n" + "   " + options[2] + "\n";
		}
		else if (choice == 3)
		{
			result = "   " + options[0] + "\n" + "   " + options[1] + "\n" + ">> " + options[2] + "\n";
		}
		return result;
	}

	public string[] GetAvailableMoveCategoryOptions()
	{
		string[] options = new string[3];
		options[0] = "1) Attack.";
		options[1] = "2) Use Item.";
		options[2] = "3) Pass Turn.";
		return options;
	} 
	//change option
	//add options
	//
}
