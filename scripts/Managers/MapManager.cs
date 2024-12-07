using Godot;
using GoingDown;
using System;
using System.Collections.Generic;
using System.Data;
using System.Reflection.Metadata.Ecma335;

public partial class MapManager : Node
{
	private Map map = new Map();
	private Floor current_floor;
	private Room current_room;
	public override void _Ready()
	{
		map.GenerateMap(DiceRoller.Roll(1,8));
		current_floor = GetFloorNum(0);
		current_room = current_floor.SpawnRoom;
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
		}
	}
	public Room GetCurrentRoom()
	{
		return current_room;
	}
	
}
