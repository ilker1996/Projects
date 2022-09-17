public class Wall extends MazeObject {

    private static final long serialVersionUID = 1L;

    public Wall(Position position) {
        super(position, MazeObjectType.WALL);

    }

    // Agent stays in the same place
    @Override
    public Position moveInto(Agent agent, MazeObject[][] gridMap) {
        return agent.getPosition();
    }
}
