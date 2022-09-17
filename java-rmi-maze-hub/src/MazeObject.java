
import java.io.Serializable;

public abstract class MazeObject implements Serializable {
    private static final long serialVersionUID = 1L;
    private Position position;
    private final MazeObjectType type;

    public MazeObject(Position position, MazeObjectType type) {
        this.position = position;
        this.type = type;
    }

    public Position getPosition() {
        return position;
    }

    public void setPosition(Position position) {
        this.position = position;
    }

    public MazeObjectType getType() {
        return type;
    }

    @Override
    public String toString() {
        switch (type) {
        case WALL:
            return "X";
        case AGENT:
            return "A";
        case GOLD:
            return "G";
        case HOLE:
            return "O";
        }
        return "";
    }

    // Returns agent's new position after replacing its location in the gridMap
    public abstract Position moveInto(Agent agent, MazeObject[][] gridMap);
}
