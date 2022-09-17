import java.rmi.Naming;

public class RMIServer {
    public static void main(String[] args) {
        try {
            Naming.rebind("MazeHub", new MazeHubImpl());
        } catch (Exception e) {
            System.out.println("RMIServer failed: " + e);
        }
    }
}
