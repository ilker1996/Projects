import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;
import java.util.List;

public class MazeHubImpl extends UnicastRemoteObject implements IMazeHub {
    private static final long serialVersionUID = 1L;
    private List<IMaze> mazeList;

    public MazeHubImpl() throws RemoteException {
        super();
        mazeList = new ArrayList<>();
    }

    @Override
    public void createMaze(int width, int height) throws RemoteException {
        IMaze newMaze = new MazeImpl();
        newMaze.create(height, width);
        mazeList.add(newMaze);
    }

    @Override
    public IMaze getMaze(int index) throws RemoteException {
        try {
            return mazeList.get(index);
        } catch (IndexOutOfBoundsException ex) {
            return null;
        }

    }

    @Override
    public boolean removeMaze(int index) throws RemoteException {
        try {
            mazeList.remove(index);
            return true;
        } catch (IndexOutOfBoundsException ex) {
            return false;
        }
    }
}
