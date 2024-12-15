void DrawMap(const std::set<int>& visitedRooms) const {
        drawConnections(visitedRooms);
        for (const auto& room : rooms) {
            if (visitedRooms.find(room.roomNumber) != visitedRooms.end()) {
                char symbol = ' ';
                switch (room.roomType) {
                    case RoomType::Spawn:    symbol = 'S'; break;
                    case RoomType::Treasure: symbol = 'T'; break;
                    case RoomType::Trap:     symbol = 'X'; break;
                    case RoomType::Large:    symbol = 'L'; break;
                    case RoomType::Boss:     symbol = 'B'; break;
                }

                SetCursorAtXY(room.position.first, room.position.second);
                std::cout << symbol;
            }
        }
    }

    void drawConnections(const std::set<int>& visitedRooms) const {
        for (const auto& room : rooms) {
            if (visitedRooms.find(room.roomNumber) != visitedRooms.end()) {
                for (int connectedRoomNumber : room.connectedRooms) {
                    if (visitedRooms.find(connectedRoomNumber) != visitedRooms.end()) {
                        const Room& connectedRoom = rooms[connectedRoomNumber];
                        drawLine(room.position.first, room.position.second, connectedRoom.position.first, connectedRoom.position.second);
                    }
                }
            }
        }
    }

    void drawLine(int x1, int y1, int x2, int y2) const {
        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;

        while (true) {
            SetCursorAtXY(x1, y1);
            std::cout << '.';  
            if (x1 == x2 && y1 == y2) break;
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
    }
    
    void printMapAndConnections() {
        for (const auto& room : rooms) {
            std::cout << room << " Connected Rooms: ";
            for (int connectedRoomNumber : room.connectedRooms) {
                std::cout << connectedRoomNumber << " ";
            }
            std::cout << std::endl;
        }
    }