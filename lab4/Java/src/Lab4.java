import java.io.*;
import java.net.*;
import java.util.Scanner;

public class Lab4 {

    private static final int PORT = 9878;

    /*
     * Main method to start the ServerSocket, Listen for Clients,
     * and handle communication between Sender / Receiver
     */
    public static void main(String args[]) throws IOException {

        // Reading from System.in
        Scanner reader = new Scanner(System.in);

        System.out.println("Creating socket on PORT: " + PORT);

        try (

                // Create the server socket for the Receiver
                ServerSocket receiverSocket = new ServerSocket(PORT);

                // Create the socket for the sender by accepting connections
                Socket senderSocket = receiverSocket.accept();

                // Create output stream to sender
                PrintWriter out = new PrintWriter(senderSocket.getOutputStream(), true);

                // Create input stream from sender
                BufferedReader in = new BufferedReader(new InputStreamReader(senderSocket.getInputStream()));

        ) {

            // Create string to store input
            String inputLine;

            // Print port information
            System.out.println("Listening on port " + PORT);

            boolean flag = true;

            // While input is received
            while (flag) {

                // Store message being sent to robot
                String message;

                try {

                    // Print possible commands
                    System.out.print("Enter the correct number to select an operation: \n"
                            + "1. Move forward\n"
                            + "2. Move backward\n"
                            + "3. Rotate clockwise\n"
                            + "4. Rotate counter-clockwise\n"
                            + "5. Read distance sensor\n"
                            + "6. Read ambient temperature \n"
                            + "7. Quit\n");

                    // Scans the next token of the input as an int.
                    int command = reader.nextInt();

                    // Check for incorrect input
                    if (command == 7) {
                        System.out.println("Quit selected, quiting now.");
                        message = "7";
                        flag = false;
                    } else if (command > 7 || command < 1) {
                        System.out.println("Incorrect input, ensure int from 1 to 7 is used.");
                        break;
                    } else {
                        // Pass command chosen to function which further prompts user
                        // and stores result (Format: {command, additional})
                        message = processInput(command);
                    }

                    // Send the result to robot
                    out.println(message);


                } catch (NullPointerException e) {
                    System.out.println("Nothing entered, closing connection");
                    System.exit(-1);
                }

            }

            // Close scanner
            reader.close();

        } catch (IOException e) {
            // Could not listen on a given port
            System.err.println("Could not listen on port " + PORT);
            System.exit(-1);
        }

    }

    /*
     * Method that takes user entered command as input
     * and prompts the user for any additional required information
     * in order to be sent to the robot
     */
    private static String processInput(int command) {

        // Reading from System.in
        Scanner reader = new Scanner(System.in);

        int[] result = new int[2];

        // Store command as first entry
        result[0] = command;

        // Store any readings
        int additionalCommand;

        if (command == 1 || command == 2) { // Move forward or backward
            // Prompt user for distance
            System.out.println("Enter the distance in cm: ");

            // Read user input
            additionalCommand = reader.nextInt();

            if (additionalCommand > 20 || additionalCommand < 1) {
                System.out.println("Incorrect value.");
            } else {
                // Store result
                result[1] = additionalCommand;
            }

            // Store result
            result[1] = additionalCommand;
        } else if (command == 3 || command == 4) { // Rotate clockwise or counter-clockwise
            // Prompt user for degrees
            System.out.println("Enter the degrees to turn: ");

            // Read user input
            additionalCommand = reader.nextInt();

            if (additionalCommand > 359 || additionalCommand < 1) {
                System.out.println("Incorrect value.");
            } else {
                // Store result
                result[1] = additionalCommand;
            }
        } else if (command == 5) { // Read sonar
            // Notify user of sonar value being read
            System.out.println("Sonar will be read.");

            // Return arbitrary int
            result[1] = 0;
        } else if (command == 6) { // Read temperature
            // Prompt for temperature type
            System.out.print("Enter temperature type you wish to receive: \n"
                    + "1 - Celcius.\n"
                    + "2 - Fahrenheit.\n");

            // Read user input
            additionalCommand = reader.nextInt();

            // Incorrect value
            if (additionalCommand != 2 && additionalCommand != 1) {
                System.out.println("Incorrect value.");
            } else {
                // Store result
                result[1] = additionalCommand;
            }
        }

        // Return the resulting concatenated string
        return "" + result[0] + result[1] + "\n";
    }

}