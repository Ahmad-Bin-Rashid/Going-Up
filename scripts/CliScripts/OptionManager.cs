using Godot;
using System.Collections.Generic;
using System.Text.Json;

public static class OptionManager
{
	private static System.Collections.Generic.Dictionary<string, List<string>> options = new System.Collections.Generic.Dictionary<string, List<string>>();

	public static  void Ready()
	{
		string filePath = "res://data/dialog/options.json";
		if (LoadOptions(filePath))
        {
			GD.Print("Options Loaded from file succesfuly.");
		}
	}
    private static bool LoadOptions(string path)
	{
		try
		{
			string jsonData = FileAccess.Open(path, FileAccess.ModeFlags.Read).GetAsText();
			options = JsonSerializer.Deserialize<Dictionary<string, List<string>>>(jsonData);
			GD.Print("Options loaded successfully.");
			return true;
		}
		catch (System.Exception e)
		{
			GD.PrintErr($"Failed to read options file {path}: {e.Message}");
			return false;
		}
	}
	public static List<string> GetOptions(string category)
	{
		return options.ContainsKey(category) ? options[category] : new List<string>();
	}

}
