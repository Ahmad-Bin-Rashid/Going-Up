using System;
using System.Collections.Generic;
using Godot;

namespace GoingDown
{
    public class Map
    {
        public List<Floor> Floors { get; private set; } = new();

        public void GenerateMap(int numFloors)
        {
            for (int i = 0; i < numFloors; i++)
            {
                Floor floor = new();
                floor.GenerateFloorGraph();
                Floors.Add(floor);
            }
        }
        public List<Floor> GetAllFloors()
        {
            return Floors;
        }
    }

}