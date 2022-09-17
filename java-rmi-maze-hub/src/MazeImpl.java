import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.LinkedHashMap;
import java.util.Map;

public class MazeImpl extends UnicastRemoteObject implements IMaze {

    private static final long serialVersionUID = 1L;

    private MazeObject[][] gridMap;
    private Map<Integer, Agent> agentMap;

    private int agentID = 0;
    private int gridWidth;
    private int gridHeight;

    private final MazeObjectFactory factory = new MazeObjectFactory();

    public MazeImpl() throws RemoteException {
        super();
    }

    @Override
    public void create(int height, int width) throws RemoteException {
        this.gridWidth = width;
        this.gridHeight = height;

        gridMap = new MazeObject[gridHeight][gridWidth];
        agentMap = new LinkedHashMap<>();
    }

    @Override
    public MazeObject getObject(Position position) throws RemoteException {
        try {
            return gridMap[position.getY()][position.getX()];

        } catch (ArrayIndexOutOfBoundsException ex) {
            return null;
        }

    }

    @Override
    public boolean createObject(Position position, MazeObjectType type) throws RemoteException {
        try {
            if (gridMap[position.getY()][position.getX()] != null) {
                return false;
            }

            MazeObject object = factory.getMazeObject(position, type, agentID);
            gridMap[position.getY()][position.getX()] = object;

            if (type == MazeObjectType.AGENT) {
                agentMap.put(agentID, (Agent) object);
                agentID++;
            }

            return true;
        } catch (ArrayIndexOutOfBoundsException ex) {
            return false;
        }
    }
    // TODO: If removed object is agent remove agent from the hashmap
    @Override
    public boolean deleteObject(Position position) throws RemoteException {
        try {
            if (gridMap[position.getY()][position.getX()] == null) {
                return false;
            } else {
                MazeObject object = gridMap[position.getY()][position.getX()];

                // If it is agent, remove from the agent list
                if(object.getType() == MazeObjectType.AGENT)
                {   
                    int id = ((Agent) object).getId();
                    agentMap.remove(id);
                }
                return true;
            }

        } catch (ArrayIndexOutOfBoundsException ex) {
            return false;
        }
    }

    @Override
    public Agent[] getAgents() throws RemoteException {
        return agentMap.values().toArray(new Agent[0]);
    }

    @Override
    public boolean moveAgent(int id, Position position) throws RemoteException {
        try {
            Agent agent = agentMap.get(id);
            Position agentPosition = agent.getPosition();

            // If manhattan distance is exactly 1
            if (position.distance(agentPosition) == 1) {
                MazeObject object = null;
                // Get object in the cell

                object = gridMap[position.getY()][position.getX()];

                // If the cell is empty
                if (object == null) {
                    gridMap[position.getY()][position.getX()] = agent;
                    agent.setPosition(position);
                    gridMap[agentPosition.getY()][agentPosition.getX()] = null;
                }
                // If there is hole, gold or wall in the cell
                else {
                    Position newPos = object.moveInto(agent, gridMap);
                    if(newPos == null)
                    {
                        // Agent is removed from simulation
                        agentMap.remove(id);
                    }
                    else{
                        // Set agent's new position
                        agent.setPosition(position);
                    }
                }

                return true;
            } else {
                return false;
            }
        } catch (ArrayIndexOutOfBoundsException | NullPointerException ex) {
            return false;
        }

    }

    @Override
    public String print() throws RemoteException {
        StringBuffer buffer = new StringBuffer("");
        buffer.append("+");
        for (int i = 0; i < gridWidth; i++) {
            buffer.append("-");
        }
        buffer.append("+\n");
        for (int j = 0; j < gridHeight; j++) {
            buffer.append("|");
            for (int i = 0; i < gridWidth; i++) {
                if (gridMap[j][i] != null) {
                    buffer.append(gridMap[j][i].toString());
                } else {
                    buffer.append(" ");
                }

            }
            buffer.append("|\n");
        }
        buffer.append("+");
        for (int i = 0; i < gridWidth; i++) {
            buffer.append("-");
        }
        return buffer.toString();
    }
}
