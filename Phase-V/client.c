package UDPclient;
//imported libraries
import java.io.*;
import java.net.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.util.Arrays;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JOptionPane;
import javax.swing.text.*;
import java.util.zip.CRC32;
import java.util.zip.Checksum;

public class UDPclient extends JFrame {

    //display panel message
    protected GUI gui = new GUI("Client Side");
    //buttons on panel
    public JButton jbtGet = new JButton("Get File");
    public JButton jbtSend = new JButton("Send File");
    JTextArea jTextArea = new JTextArea();
    JScrollPane jScrollPane = new JScrollPane(jTextArea);
     DefaultCaret caret = (DefaultCaret) jTextArea.getCaret();

    public static void main(String args[]) throws Exception {

        Client client1 = new Client();
        client1.createclient();
         
    }

    //method for GUI button placement and functions
    public  UDPclient() {
        gui.setBackground(Color.white);
        JPanel jpButtons = new JPanel();
        jpButtons.add(jbtGet);
        jpButtons.add(jbtSend);
        //shortcut keyboard
        jbtGet.setMnemonic('G');
        jbtSend.setMnemonic('S');
        
        //place panels
        setLayout(new BorderLayout());
        add(gui, BorderLayout.CENTER);
        add(jpButtons, BorderLayout.SOUTH);
        

    }//end of UDPclient class
    
    

}//end of public class UDPclient extends JFrame

class Client extends UDPclient {

    //data fields
    private byte[] sendData;//used for sending datagram
    private byte[] receiveData;//used for receiving datagram
    private String fname;// used for server to determine to receive or send
    private byte[] bytearray = new byte[1024];//used for file transfer
    private byte[] packet;
    private byte[] checksumPacket;
    private byte[] sequencePacket;
    private byte[] newbytes;
    private byte[] checkData;
    private byte[] data;
    private byte[] datadest;
    private byte[] calculateChecksum;
    private byte[] sequence;
    private long sequencecount;
    long calculatedChecksum;
    private byte[] checksum;
    boolean ackStatus;
    private int port;
    private byte[] responsebyte;
    private byte[] ACKbyte;
    private byte[] sequenceBytes;
    InetAddress IPAddress;
    private int wrongSequenceCount;
    private int prevSeq;
    int dataBitError = 0;
   // UDPclient frame = new UDPclient();

    //constructor with default values
    Client() throws Exception {
        sendData = new byte[1024];
        receiveData = new byte[1024];
        packet = new byte[1024];
        checksumPacket = new byte[8];
        sequencePacket = new byte[8];
        data = new byte[1024];
        datadest = new byte[1024];
        calculateChecksum = new byte [1024];
        checkData = new byte[1024];
        IPAddress =  InetAddress.getByName("localhost");
        responsebyte = new byte[1024];
       // responsebyte = new byte[8];
        port = 9876;
        wrongSequenceCount = 0;
        prevSeq = -1;
    }//end constructor

    //methods
    //setFile name method
    public String setFile() throws Exception {
        fname = "mario.png";
        return fname;
    }//end function
    
public void createclient() throws Exception{
      final  UDPclient frame = new UDPclient();
        caret.setUpdatePolicy(DefaultCaret.ALWAYS_UPDATE);
        frame.getContentPane().add(jScrollPane);
        frame.setTitle("Client");
        frame.setSize(300, 300);
        jTextArea.append("Welcome to the Client\nWaiting for Command \n");
        frame.setLocationRelativeTo(null);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  
        frame.setVisible(true);
        //sequencecount = 0;
        
         frame.jbtGet.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {                    
                    setFile(); //access setFile() method
                    getFile();//access getFile() method
                } catch (Exception ex) {
                    Logger.getLogger(
                            UDPclient.class.getName()).log(
                                    Level.SEVERE, null, ex);
                }
            }//end of action event
        });//end of get file button

        //Send file button
        frame.jbtSend.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                  
                    setFilesend();//access setFile() method
                    send();//access getFile() method 
                } catch (Exception ex) {
                    Logger.getLogger(
                            UDPclient.class.getName()).log(
                                    Level.SEVERE, null, ex);
                }
            }//end of action event
        });//end of send file button
}
    //setFilesending name method
    public String setFilesend() {
        fname = "test.txt";
        return fname;
    }//end function

    //method for sending file to server
    public void send() throws Exception {
        
        //open Datagram socket 
        DatagramSocket clientSocket = new DatagramSocket();
      //  IPAddress = InetAddress.getByName("localhost");

        fname = "luigi.png";
        sendData = fname.getBytes();

        //create datagram with data to send length
        //ip address, port
        DatagramPacket sendPacket
                = new DatagramPacket(sendData, sendData.length, IPAddress, port);

        //send datagram to server
        clientSocket.send(sendPacket);

        DatagramPacket receivePacket
                = new DatagramPacket(receiveData, receiveData.length);
          
        //server receives datagram
        clientSocket.receive(receivePacket);
        int port = receivePacket.getPort();
        //set data to sentence
        String sentence = new String(receivePacket.getData());
        int totalBytes = 0;
        if (sentence.contains("yes"));
        {

            System.out.println("got response");
            File fileTransfer = new File("luigi.png");
            FileInputStream fileInput = new FileInputStream(fileTransfer);
            ByteArrayOutputStream bytearrayed = new ByteArrayOutputStream();
            sendPacket
                    = new DatagramPacket(bytearray, bytearray.length, IPAddress, port);
            //parse file
            for (int readNum; (readNum = fileInput.read(bytearray)) != -1;) {

                bytearrayed.write(bytearray, 0, readNum);
            //System.out.println("Read " + readNum + " bytes,\n");

                jTextArea.append("Read " + readNum + " bytes,\n");

                newbytes = bytearrayed.toByteArray();

                sendPacket
                        = new DatagramPacket(bytearray, bytearray.length, IPAddress, port);

                //write out datagram to socket
                clientSocket.send(sendPacket);
                //   System.out.println("sent " + readNum + " bytes,");
                jTextArea.append("Sent " + readNum + " bytes,\n");
                totalBytes += readNum;
                jTextArea.append("Total Bytes Sent: " + totalBytes + "\n\n");

            }
            jTextArea.append("------------------------------------------\n");
            sendData = "yes".getBytes();
            sendPacket
                    = new DatagramPacket(sendData, sendData.length, IPAddress, port);

            clientSocket.send(sendPacket);
            JOptionPane.showMessageDialog(null, "File Sent");
            // System.out.println(sendPacket.toString());
        }

    }

    //get from server
    public void getFile() throws Exception {
        //open Datagram socket 
        sequencecount = 0;
        DatagramSocket clientSocket = new DatagramSocket();
        InetAddress IPAddress = InetAddress.getByName("localhost");

        sendData = fname.getBytes();

        //create datagram with data to send length
        //ip address, port
        DatagramPacket sendPacket
                = new DatagramPacket(sendData, sendData.length, IPAddress, port);

        //send datagram to server
        clientSocket.send(sendPacket);

        //Receive
        ByteArrayOutputStream bout = new ByteArrayOutputStream();
        ByteArrayOutputStream checksumdataArray = new ByteArrayOutputStream();


        DatagramPacket receivePacket
                = new DatagramPacket(receiveData, receiveData.length);

        String sentence;
        
        //image frame
         JFrame frame2 = new JFrame("ShowImage");
         frame2.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
         frame2.setSize(250,250);
         GUI panel = new GUI();
         frame2.setContentPane(panel); 
         frame2.setVisible(true);

        //read datagram from server
        while (true) {//make into rdt_rcv

            clientSocket.receive(receivePacket);//look
            checksumdataArray.reset();
            //Actual Data
            bout.write(receivePacket.getData(),16,1008);
            //for checking data integrity
            checksumdataArray.write(receivePacket.getData(),16,1008);
            
            sentence = new String(receivePacket.getData());
            if (sentence.contains("yes")) {
                break;
            } 
            else {          
                //actual file data in bytes
               data = extract_data(rdt_rcv(bout),datadest);
               //actual data file integrity
               test_pkt(checksumdataArray, receivePacket, clientSocket, bout);

             
            }
                
         
         frame2.repaint();//repaint image
            
        }

        //notification of completion
        JOptionPane.showMessageDialog(null, "File Received");

        //close socket
        clientSocket.close();

    }
    
    public void test_pkt(ByteArrayOutputStream checksumdataArray,
            DatagramPacket receivePacket
    ,DatagramSocket clientSocket, ByteArrayOutputStream bout) throws IOException, InterruptedException, Exception {
        checkData = extract_data(rdt_rcv(checksumdataArray),datadest);
               //calculate received data checksum
               calculatedChecksum = checksumCalc(checkData);//returns a long
               //Extract checksum information from packet
               checksum = extract_checksum(checksum_pkt(receivePacket));//returns bytes[]
               sequence = extract_sequence(sequence_pkt(receivePacket));
               //check if data checksum is valid with checksum information
               //from the header
               //the true is not corrupt
               int waittest = 5000;
             if (corruptTest(checksum,calculatedChecksum)==true && 
                     (sequencecount == byteArrayToLong(sequence)) ){
        
                   System.out.println("sequence matched");
                   deliver_data(data);
                  
                  prevSeq = (int) sequencecount;
    
                   clientSocket.send(make_pkt());
                   sequencecount++;
                   waittest = 0;
           
             }//the false = corrupt
             else if(corruptTest(checksum,calculatedChecksum)==false || 
                     (sequencecount != byteArrayToLong(sequence)) ){
                 
                    if (prevSeq == (int) byteArrayToLong(sequence)){
               System.out.println("\nDuplicate sequence\n");
               System.out.println("Discarding Duplicate \n");
           
            data = new byte[0];
            
 
           }
           else
           {
             
                 System.out.println("wrong sequence or wrong checksum");
                 prevSeq = (int) byteArrayToLong(sequence);
        
                 clientSocket.send(make_pkt());
                 clientSocket.receive(receivePacket);
                  
             }
                    
        
    }
    }
    
	//isACK(rcvpkt,sequence)
    boolean corruptTest(byte[] checksum, long calculatedChecksum) throws IOException{
        
           if (byteArrayToLong(checksum) == calculatedChecksum) {
                   long response = 1;
                 
                   ACKbyte = getBytes(response);
                   ackStatus = true;
                   return ackStatus;
               }//not corrupt
           else {
               long response = 0;
               ACKbyte = getBytes(response);
               ackStatus = false;
               return ackStatus;//corrupt
           }
        
    }
    
    //response with sequence and ack type
      DatagramPacket make_pkt() 
              throws IOException, InterruptedException {
          
          byte[] packbyte = new byte[responsebyte.length];
          sequenceBytes = getBytes(sequencecount);
          System.arraycopy(ACKbyte, 0, packbyte, 0, ACKbyte.length);
          System.arraycopy(sequenceBytes, 0, packbyte, 8, sequenceBytes.length);
          DatagramPacket sendPacket = new DatagramPacket(packbyte, 
                           packbyte.length, IPAddress, port);
       
        return sendPacket;
    }
    
    byte[] rdt_rcv(ByteArrayOutputStream bout) throws IOException {
                packet = new byte[bout.size()];
                packet = bout.toByteArray();              
                return packet;
    }//end
    
    //take the checksum header packet from the entire received packet
    byte[] checksum_pkt(DatagramPacket rcv_pkt) throws IOException{
        
        ByteArrayOutputStream boutCheck = new ByteArrayOutputStream();
        boutCheck.flush();

        boutCheck.write(rcv_pkt.getData(),8,8); 

        checksumPacket = new byte[boutCheck.size()];
        checksumPacket = boutCheck.toByteArray();
        
        return checksumPacket;  
    }//end
    
    //take the sequence header packet from the entire received packet
     byte[] sequence_pkt(DatagramPacket rcv_pkt) throws IOException{
        ByteArrayOutputStream boutSeq = new ByteArrayOutputStream();
        boutSeq.flush();
        boutSeq.write(rcv_pkt.getData(),0,8);
        sequencePacket = new byte[boutSeq.size()];
        sequencePacket = boutSeq.toByteArray();
        
        return sequencePacket;  
    }//end
    
     //extract the checksum in bytes from the checksum header packet
    byte[] extract_checksum(byte[] packet){
         byte[] Check = new byte[8];
        long checker;
        System.arraycopy(packet, 0, Check, 0, 8);
        checker = byteArrayToLong(Check);
        System.out.println("CHECKSUM Received: " + checker);
        return Check;
    }//end
    
    //extract the sequence in btes from the sequence header packet
      byte[] extract_sequence(byte[] packet){
        byte[] Sequence = new byte[8];
        long SequenceNum;
        System.arraycopy(packet, 0, Sequence, 0, 8);
        SequenceNum = byteArrayToLong(Sequence);
        System.out.println("Sequence Received: " + SequenceNum);
        return Sequence;
    }//end
            
    //extract the data from the received data packet seperated from header packets
    byte[] extract_data(byte[] packet, byte[] destination) {
        
        destination = new byte[packet.length];
        System.arraycopy(packet, 0, destination, 0, packet.length);
        
        return destination;
    }//extract data
    
    //deliver data to the application layer
    public void deliver_data(byte[] extractedData) 
            throws FileNotFoundException, IOException, InterruptedException{
        FileOutputStream fileOutput = new FileOutputStream("naughty.jpg");
        fileOutput.write(extractedData, 0, extractedData.length);
        fileOutput.close();
    }//end deliver data to application layer
    
    //calculate checksum
    long checksumCalc(byte[]dataBytes){
        
        Checksum checksum = new CRC16();
        checksum.update(dataBytes, 0, dataBytes.length);
        long checksumValue = checksum.getValue();
        System.out.println("CHECKSUM Calculated: " + checksumValue);
        jTextArea.append("CHECKSUM Calculated: " + checksumValue + "\n\n");        
        return checksumValue;
    }//end checksum calculation
    
    //get bytes from a long value
    public static byte[] getBytes(long val) throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(baos);
        dos.writeLong(val);
        return baos.toByteArray();
    }//end getbytes
    
    private long byteArrayToLong(byte[] buffer){
        long value=0;
        long multiplier=1;
        for (int i = 7; i >= 0; i--) { //get from the right

            value=value+(buffer[i] & 0xff)*multiplier; // add the value * the hex multiplier
            multiplier=multiplier <<8;
            }
        return value;
   }//end byte array to long

}//end class Client
