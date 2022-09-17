
public class MazeObjectFactory {
    public MazeObject getMazeObject(Position position, MazeObjectType type, int agentId) {
        switch (type) {

        case WALL:
            return new Wall(position);
        case AGENT:
            return new Agent(position, agentId);
        case GOLD:
            return new Gold(position);
        case HOLE:
            return new Hole(position);
        }
        // Return empty maze object
        return null;
    }
}