import java.rmi.Naming;
import java.rmi.RemoteException;
import java.util.Scanner;

public class RMIClient {
    private static final String[] Logger = { "Operation Failed.", "Operation Success." };

    private final static int FAIL = 0;
    private final static int SUCCESS = 1;

    public static void main(String[] args) {
        IMazeHub mazeHub = null;
        IMaze selectedMaze = null;
        boolean quit = false;
        try {
            mazeHub = (IMazeHub) Naming.lookup("//localhost/MazeHub");
        } catch (Exception e) {
            System.out.println("RMI Client is failed : " + e);
        }

        Scanner scanner = null;
        try {
            scanner = new Scanner(System.in);
            ParsedInput parsedInput = null;
            String input;
            while (!quit) {
                input = scanner.nextLine();
                try {
                    parsedInput = ParsedInput.parse(input);
                } catch (Exception ex) {
                    parsedInput = null;
                }
                if (parsedInput == null) {
                    System.out.println("Wrong input format. Try again.");
                    continue;
                }
                switch (parsedInput.getType()) {
                case CREATE_MAZE:
                    createMaze(parsedInput.getArgs(), mazeHub);
                    break;
                case DELETE_MAZE:
                    // TODO: Should we delete recorded maze ?
                    if (deleteMaze(parsedInput.getArgs(), mazeHub)) {
                        // If deletion is successful, delete recorded maze
                        selectedMaze = null;
                    }

                    break;
                case SELECT_MAZE:
                    selectedMaze = selectMaze(parsedInput.getArgs(), mazeHub);
                    break;
                case PRINT_MAZE:
                    printMaze(selectedMaze);
                    break;
                case CREATE_OBJECT:
                    createMazeObject(parsedInput.getArgs(), selectedMaze);
                    break;
                case DELETE_OBJECT:
                    deleteMazeObject(parsedInput.getArgs(), selectedMaze);
                    break;
                case LIST_AGENTS:
                    listAgents(selectedMaze);
                    break;
                case MOVE_AGENT:
                    moveAgent(parsedInput.getArgs(), selectedMaze);
                    break;
                case QUIT:
                    quit = true;
                    break;
                }
            }
        } finally {
            scanner.close();
        }
    }

    private static void createMaze(Object args[], IMazeHub mazeHub) {
        int width = (int) args[0];
        int height = (int) args[1];
        if (width <= 0 || height <= 0) {
            System.out.println(Logger[FAIL]);
            return;
        }
        try {
            mazeHub.createMaze(width, height);
        } catch (RemoteException e) {}
    }

    private static boolean deleteMaze(Object args[], IMazeHub mazeHub) {
        int index = (int) args[0];
        boolean success = false;
        try {
            success = mazeHub.removeMaze(index);
        } catch (RemoteException e) {
            System.out.println(Logger[FAIL]);
            return false;
        }

        System.out.println(Logger[success ? SUCCESS : FAIL]);

        return success;
    }

    private static IMaze selectMaze(Object args[], IMazeHub mazeHub) {
        int index = (int) args[0];
        IMaze selectedMaze = null;
        try {
            selectedMaze = mazeHub.getMaze(index);
        } catch (RemoteException e) {
            System.out.println(Logger[FAIL]);
            return null;
        }

        System.out.println(Logger[(selectedMaze != null) ? SUCCESS : FAIL]);

        return selectedMaze;
    }

    private static void printMaze(IMaze maze) {
        String output = "";
        try {
            output = maze.print();
        } catch (NullPointerException | RemoteException e) {
            return;
        }
        System.out.println(output);
    }

    private static void createMazeObject(Object args[], IMaze maze) {
        int posX = (int) args[0];
        int posY = (int) args[1];
        MazeObjectType type = (MazeObjectType) args[2];
        boolean success = false;
        try {
            success = maze.createObject(new Position(posX, posY), type);
        } catch (NullPointerException | RemoteException e) {
            System.out.println(Logger[FAIL]);
            return;
        }

        System.out.println(Logger[success ? SUCCESS : FAIL]);
    }

    private static void deleteMazeObject(Object args[], IMaze maze) {
        int posX = (int) args[0];
        int posY = (int) args[1];
        boolean success = false;
        try {
            success = maze.deleteObject(new Position(posX, posY));
        } catch (NullPointerException | RemoteException e) {
            System.out.println(Logger[FAIL]);
            return;
        }

        System.out.println(Logger[success ? SUCCESS : FAIL]);
    }

    private static void listAgents(IMaze maze) {
        Agent[] agentList = null;
        try {
            agentList = maze.getAgents();
        } catch (RemoteException | NullPointerException e) {
            return;
        }

        if (agentList != null) {
            for (Agent agent : agentList) {
                agent.print();
            }
        }
    }

    private static void moveAgent(Object args[], IMaze maze) {
        int agentId = (int) args[0];
        int posX = (int) args[1];
        int posY = (int) args[2];
        boolean success = false;
        try {
            success = maze.moveAgent(agentId, new Position(posX, posY));
        } catch (RemoteException e) {
            System.out.println(Logger[FAIL]);
            return;
        }

        System.out.println(Logger[success ? SUCCESS : FAIL]);
    }
}
