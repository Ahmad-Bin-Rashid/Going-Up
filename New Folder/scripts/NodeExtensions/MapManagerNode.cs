using Godot;
using System;
using System.Collections.Generic;

namespace GoingDown
{
    public partial class MapManagerNode : Node
    {
        public Floor current_floor;

        [Export] public PackedScene RoomScene { get; set; }
        private Dictionary<Vector2, Node2D> roomInstances;
        private Dictionary<Button, Room> buttonToRoomMap = new Dictionary<Button, Room>();
        private List<RoomData> rooms;
        private List<ConnectionData> connections;

        private Vector2 graphOffset = new Vector2(50, 50); // Padding from screen edges
        private float graphScale; // Uniform scaling factor for the graph

        public override void _Ready()
        {
            current_floor = Global.Instance.mapManager.current_floor;
            roomInstances = new Dictionary<Vector2, Node2D>();
            InitializeGraph();
        }

        public void InitializeGraph()
        {
            rooms = new List<RoomData>();
            connections = new List<ConnectionData>();

            // Extract data from the Floor object
            foreach (var room in current_floor.Rooms)
            {
                var roomData = new RoomData(room, new Vector2(room.Position.Item1, room.Position.Item2));
                rooms.Add(roomData);
                foreach (var connectedRoom in room.Connections)
                {
                    connections.Add(new ConnectionData(
                        new Vector2(room.Position.Item1, room.Position.Item2),
                        new Vector2(connectedRoom.Position.Item1, connectedRoom.Position.Item2)
                    ));
                }
            }

            CalculateGraphScale();
            DrawGraph();
        }

        private void CalculateGraphScale()
        {
            // Determine the bounding box of the graph
            Vector2 minPos = Vector2.Inf;
            Vector2 maxPos = -Vector2.Inf;

            foreach (var roomData in rooms)
            {
                minPos = minPos.Min(roomData.Position);
                maxPos = maxPos.Max(roomData.Position);
            }

            Vector2 graphSize = maxPos - minPos;

            // Get the viewport size
            var viewportSize = GetViewport().GetVisibleRect().Size;

            // Calculate scaling factors
            Vector2 availableSpace = viewportSize - graphOffset * 2;
            graphScale = Math.Min(availableSpace.X / graphSize.X, availableSpace.Y / graphSize.Y); // Uniform scaling
        }

		private void DrawGraph()
		{
			// Step 1: Spawn Room Nodes
			foreach (var roomData in rooms)
			{
				var roomInstance = CreateRoomNode(roomData);
				roomInstances[roomData.Position] = roomInstance;
			}

			// Step 2: Draw Connections Between Rooms
			foreach (var connection in connections)
			{
				if (roomInstances.TryGetValue(connection.Start, out var startRoom) &&
					roomInstances.TryGetValue(connection.End, out var endRoom))
				{
					DrawEdge(startRoom.Position, endRoom.Position);
				}
				else
				{
					GD.PrintErr($"Connection points not found: Start={connection.Start}, End={connection.End}");
				}
			}
		}


        private Node2D CreateRoomNode(RoomData roomData)
        {
            // Instantiate the room node from the PackedScene
            Node2D roomInstance = (Node2D)RoomScene.Instantiate();

            // Scale the room's position to fit the screen
            roomInstance.Position = graphOffset + roomData.Position * graphScale;

            // Customize room based on type
            var button = roomInstance.GetNode<Button>("Button");
            button.Text = roomData.Room.Type.ToString();
            buttonToRoomMap[button] = roomData.Room;
            button.Connect("pressed", new Callable(this, nameof(OnButtonPressed)));

            // Add the room node to the scene tree
            AddChild(roomInstance);

            return roomInstance;
        }

		private void DrawEdge(Vector2 startPos, Vector2 endPos)
		{
			// Scale room positions correctly
			startPos = graphOffset + startPos * graphScale;
			endPos = graphOffset + endPos * graphScale;

			GD.Print($"Drawing edge from {startPos} to {endPos}");

			var line = new Line2D
			{
				Width = 4,
				DefaultColor = new Color(1, 1, 1), // White color
				ZIndex = 1, // Ensure it's visible
			};

			line.AddPoint(startPos);
			line.AddPoint(endPos);

			AddChild(line);

			GD.Print($"Edge added between {startPos} and {endPos}");
		}





        private void OnButtonPressed(Button button)
        {
            if (buttonToRoomMap.TryGetValue(button, out Room room))
            {
                GD.Print($"Room pressed: {room}");
                GD.Print($"Type: {room.Type}, Position: {room.Position}");
            }
        }

        private class ConnectionData
        {
            public Vector2 Start { get; }
            public Vector2 End { get; }

            public ConnectionData(Vector2 start, Vector2 end)
            {
                Start = start;
                End = end;
            }
        }

        private class RoomData
        {
            public Room Room { get; }
            public Vector2 Position { get; }

            public RoomData(Room room, Vector2 position)
            {
                Room = room;
                Position = position;
            }
        }
    }
}
