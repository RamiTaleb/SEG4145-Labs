import java.io.*;
import java.net.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.text.SimpleDateFormat;
import java.util.*;

public class Lab4Alt {

    private static final int PORT = 9876;
    public static JFrame frame;
    public static JLabel operation;
    public static JLabel timestamp;
    public static int command = -1;
    public static int additionalCommand;

    /*
     * Main method to start the ServerSocket, Listen for Clients,
     * and handle communication between Sender / Receiver
     */
    public static void main(String args[]) throws IOException {

        // Reading from System.in
        Scanner reader = new Scanner(System.in);

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

            frame = new JFrame("SEG4145 Lab 5");
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.getContentPane().setLayout(new GridLayout(5, 3));

            // Create first row
            frame.add(new JLabel("", JLabel.CENTER));
            frame.add(new JLabel("Select an Operation:", JLabel.CENTER));
            frame.add(new JLabel("", JLabel.CENTER));

            // Create second row
            createButton("Move Forward", 1, frame, out);
            createButton("Move Backward", 2, frame, out);
            createButton("Rotate Clockwise", 3, frame, out);

            // Create third row
            createButton("Rotate Counter Clockwise", 4, frame, out);
            createButton("Read Distance", 5, frame, out);
            createButton("Read Temperature", 6, frame, out);

            // Create fourth row
            frame.add(new JLabel("", JLabel.CENTER));
            operation = new JLabel("Last Operation: ", JLabel.CENTER);
            frame.add(operation);
            frame.add(new JLabel("", JLabel.CENTER));

            // Create fifth row
            frame.add(new JLabel("", JLabel.CENTER));
            timestamp = new JLabel("Timestamp: ", JLabel.CENTER);
            frame.add(timestamp);
            frame.add(new JLabel("", JLabel.CENTER));

            frame.pack();
            frame.setVisible(true);

            // While input is received
            while ((inputLine = in.readLine()) != null) {




            }

        } catch (IOException e) {
            // Could not listen on a given port
            System.err.println("Could not listen on port " + PORT);
            System.exit(-1);
        }

    }

    private static void createButton(String text, int val, JFrame frame, PrintWriter out) {
        JButton btn = new JButton(text);
        btn.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e)
            {
                // Update command
                command = val;

                // Update additional command based on command
                if (command == 1 || command == 2) {
                    String distance = JOptionPane.showInputDialog("Enter Distance (0 to 20):");
                    additionalCommand = Integer.parseInt(distance);
                } else if (command == 3 || command == 4) {
                    String degrees = JOptionPane.showInputDialog("Enter Degrees (0 to 359):");
                    additionalCommand = Integer.parseInt(degrees);
                } else if (command == 6) {
                    String tempType = JOptionPane.showInputDialog("Enter Temperature Type (1 - Celsius, 2 - Fahrenheit):");
                    additionalCommand = Integer.parseInt(tempType);
                } else {
                    additionalCommand = 0;
                }

                operation.setText("Last Operation: " + command);
                timestamp.setText("Timestamp: " + new SimpleDateFormat("dd/mm/yy HH:mm").format(new Date()));

                // Send to user
                out.println("" + command + additionalCommand + "\n");
            }
        });
        frame.add(btn);
    }

}
