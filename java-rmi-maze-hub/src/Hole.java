public class Hole extends MazeObject {

    private static final long serialVersionUID = 1L;

    public Hole(Position position) {
        super(position, MazeObjectType.HOLE);
    }

    // Return null since agent dropped to the hole
    @Override
    public Position moveInto(Agent agent, MazeObject[][] gridMap) {
        Position position = agent.getPosition();
        gridMap[position.getY()][position.getX()] = null;
        
        return null;
    }
}
