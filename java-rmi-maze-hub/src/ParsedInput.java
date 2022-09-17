public class ParsedInput {
    public static ParsedInput parse(String input) throws Exception {
        String[] inputParts = input.split("\\s");
        if (inputParts.length == 0)
            return null;
        ParsedInput result = null;
        if (inputParts[0].equalsIgnoreCase("cm")) {
            ParsedInputType type = ParsedInputType.CREATE_MAZE;
            result = new ParsedInput(type, 2);
            result.addArg(0, Integer.parseInt(inputParts[1])); // width
            result.addArg(1, Integer.parseInt(inputParts[2])); // height
        } else if (inputParts[0].equalsIgnoreCase("dm")) {
            ParsedInputType type = ParsedInputType.DELETE_MAZE;
            result = new ParsedInput(type, 1);
            result.addArg(0, Integer.parseInt(inputParts[1])); // maze index
        } else if (inputParts[0].equalsIgnoreCase("sm")) {
            ParsedInputType type = ParsedInputType.SELECT_MAZE;
            result = new ParsedInput(type, 1);
            result.addArg(0, Integer.parseInt(inputParts[1])); // maze index
        } else if (inputParts[0].equalsIgnoreCase("co")) {
            ParsedInputType type = ParsedInputType.CREATE_OBJECT;
            result = new ParsedInput(type, 3);
            result.addArg(0, Integer.parseInt(inputParts[1])); // position_x
            result.addArg(1, Integer.parseInt(inputParts[2])); // position_y
            result.addArg(2, MazeObjectType.valueOf(inputParts[3].toUpperCase())); // object_type
        } else if (inputParts[0].equalsIgnoreCase("do")) {
            ParsedInputType type = ParsedInputType.DELETE_OBJECT;
            result = new ParsedInput(type, 2);
            result.addArg(0, Integer.parseInt(inputParts[1])); // position_x
            result.addArg(1, Integer.parseInt(inputParts[2])); // position_y
        } else if (inputParts[0].equalsIgnoreCase("la")) {
            ParsedInputType type = ParsedInputType.LIST_AGENTS;
            result = new ParsedInput(type, 0);
        } else if (inputParts[0].equalsIgnoreCase("ma")) {
            ParsedInputType type = ParsedInputType.MOVE_AGENT;
            result = new ParsedInput(type, 3);
            result.addArg(0, Integer.parseInt(inputParts[1])); // agent_id
            result.addArg(1, Integer.parseInt(inputParts[2])); // position_x
            result.addArg(2, Integer.parseInt(inputParts[3])); // position_y
        } else if (inputParts[0].equalsIgnoreCase("p")) {
            result = new ParsedInput(ParsedInputType.PRINT_MAZE, 0);
        } else if (inputParts[0].equalsIgnoreCase("q")) {
            result = new ParsedInput(ParsedInputType.QUIT, 0);
        }

        return result;
    }

    private final ParsedInputType type;
    private final Object[] args;

    public ParsedInput(ParsedInputType type, int argc) {
        this.type = type;
        this.args = new Object[argc];
    }

    public ParsedInputType getType() {
        return type;
    }

    public Object[] getArgs() {
        return args;
    }

    public void addArg(int index, Object object) {
        this.args[index] = object;
    }
}
