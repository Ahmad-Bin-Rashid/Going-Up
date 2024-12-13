#include <unordered_map>

// Define the RoomHash struct
struct RoomHash {
    std::size_t operator()(const Room& room) const {
        std::size_t h1 = std::hash<int>{}(room.roomNumber);
        std::size_t h2 = std::hash<int>{}(static_cast<int>(room.roomType));
        std::size_t h3 = std::hash<int>{}(room.position.first);
        std::size_t h4 = std::hash<int>{}(room.position.second);
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
    }
};

// Define the equality operator for Room
bool operator==(const Room& lhs, const Room& rhs) {
    return lhs.roomNumber == rhs.roomNumber &&
           lhs.roomType == rhs.roomType &&
           lhs.position == rhs.position;
}

// Define the unordered_map
std::unordered_map<Room, LargeRoom, RoomHash> roomMap;

// Example usage
void addRoomToMap(const Room& room, RoomType roomType) {
    roomMap[room] = std::make_unique<RoomType>(roomType);
}
