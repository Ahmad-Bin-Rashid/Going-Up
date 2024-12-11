using Godot;
using GoingDown;
using System;
using System.Collections.Generic;
using System.Data;
using System.Reflection.Metadata.Ecma335;

public partial class MapManager : Node
{
	private Map map;
	public Floor current_floor ;
	private Room current_room;
	public override void _Ready()
	{
		map = new Map(DiceRoller.Roll(1,6));
		current_floor = GetFloorNum(0);
		current_room = current_floor.spawn;
	}
	public MapManager()
	{
		map = new Map(DiceRoller.Roll(1,6));
		current_floor = GetFloorNum(0);
		current_room = current_floor.spawn;
	}

	public Floor GetFloorNum(int num)
	{
		List<Floor> all_floors = new List<Floor>();
		all_floors = map.GetAllFloors();

		if (all_floors == null || num < 0 || num >= all_floors.Count)
		{
			GD.PrintErr("All floors / current map is empty.");
			throw new ArgumentOutOfRangeException(nameof(all_floors), "Index is out of range for the list.");
		}
		return all_floors[num];
	}
	public List<Room> GetConnectedRooms()
	{
		return current_room.GetConnectedsRoom();
	}
	public void MoveRooms(int door)
	{
		List<Room> connections = GetConnectedRooms();
		door = door - 1;
		if (connections == null || door < 0 || door >= connections.Count)
		{
			GD.PrintErr("That is not a door");
		}
		else
		{
			current_room = connections[door];
			current_room.visited = true;
		}
	}
	public Room GetCurrentRoom()
	{
		return current_room;
	}
	public Map GetMap()
	{
		return map;
	}
}
