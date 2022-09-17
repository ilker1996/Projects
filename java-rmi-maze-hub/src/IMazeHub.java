
import java.rmi.Remote;
import java.rmi.RemoteException;

public interface IMazeHub extends Remote {
    void createMaze(int width, int height) throws RemoteException;

    IMaze getMaze(int index) throws RemoteException;

    boolean removeMaze(int index) throws RemoteException;
}
