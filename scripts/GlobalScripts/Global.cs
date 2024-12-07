using Godot;
using System;
namespace GoingDown
{
	public partial class Global : Node
	{
		public static Global Instance { get; private set; }
		public MapManager mapManager ;
		public override void _Ready()
		{
			Instance = this;

			mapManager = GetNode<MapManager>("/root/main/MapManager");
			
		}




	}
}