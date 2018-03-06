import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import javax.swing.*;


public class ConnectFour {
	private String infosent;
	private String rev;
	private JButton button;
	private JFrame frame;
    private JPanel mainPanel;
    private JButton[][] buttons;
    private JLabel[][] labels;
    private String player;
    
    private JFrame resultFrame;
    private JPanel resultPanel;
    private JLabel resultLabel;
    private String result;
    private boolean turn;

    public ConnectFour()
    {
    	//server LIS=new server();
    	//LIS.start();
    	turn = true;			//Determines who's turn it is
        frame = new JFrame("Connect Four");
        mainPanel = new JPanel();
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (Exception e) {}
        initialize();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.getContentPane().add(mainPanel);
        frame.setLocationByPlatform(true);
        frame.pack();
        frame.setVisible(true);
    }

    public static void main(String[] args)
    {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new ConnectFour();
            }
        });
    }
    //////////////////Creates Board////////////////////////
    private void initialize()
    {
        mainPanel.setLayout(new GridLayout(7,7));
        mainPanel.setPreferredSize(new Dimension(1100,600));
        
        
        buttons = new JButton[1][7];
        for (int i=0; i<1; i++)
        {
            for (int j=0; j<7; j++)
            {
                buttons[i][j] = new JButton("Drop Token");	//First row is a row of buttons that simulate a token being dropped
                buttons[i][j].setFont(new Font("Verdana", Font.BOLD, 18));
                buttons[i][j].addActionListener(new ButtonListener());
                mainPanel.add(buttons[i][j]);
            }
        }
        labels=new JLabel[7][7];	//A 2D array of labels that will simulate our board.
        for(int i=1;i<7;i++)
        {
        	for(int j=0;j<7;j++)
        	{
        		labels[i][j]=new JLabel(" ");
        		labels[i][j].setFont(new Font("Times New Roman",Font.BOLD,32));
        		labels[i][j].setHorizontalAlignment(SwingConstants.CENTER);
        		labels[i][j].setBorder(BorderFactory.createLineBorder(Color.YELLOW));
        		mainPanel.add(labels[i][j]);
        	}
        }
        player = "R";	//Using a string....for now, eventually want to let players chose what either red or black
    }
    
    
    ///////////////////Win or Lose screen///////////////
    
    public void ResultInitialize(){
        frame.dispose();
        resultFrame = new JFrame("Result");
        resultPanel = new JPanel();
        resultPanel.setLayout(new GridLayout(1,1));
        resultPanel.setPreferredSize(new Dimension(300,300));
        try {
        	UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (Exception e) {}
        initialize();
        resultFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        resultFrame.getContentPane().add(resultPanel);
        resultFrame.setLocationByPlatform(true);
        resultFrame.pack();
        resultFrame.setVisible(true);    
        resultLabel = new JLabel("You've "+result);
        resultLabel.setFont(new Font("Times New Roman",Font.BOLD,40));
        resultLabel.setHorizontalAlignment(SwingConstants.CENTER);
        resultPanel.add(resultLabel);
    }
    
    ///////////////////Series of functions that check for a win or loss////////////////////////
    
    public void resultcheck() //J are columns and i are rows
    {
    	int count = 0;
    	int count2 =0;
    	for(int i=1;i<7;i++){
    		if(labels[i][3].getText().equals("R") || labels[i][3].getText().equals("B")){
    			for(int j=0;j<7;j++){
    				if(labels[i][j].getText().equals("R")){
    					count++;
    					if(count==4){
    						result="Won";
    						ResultInitialize();
    					}
    				}
    				
    				else
    					count=0;
    					
    				if(labels[i][j].getText().equals("B")){
    					count2++;
    					if(count2==4){
    						result="Lost";
    						ResultInitialize();
    					}
    				}
    				
    				else
    					count2=0;
    					
    				
    				
    			}
    		}
    	}	
    	////////Vertical search//////////
    	for(int j=0;j<7;j++){
    		count = 0;
    		count2=0;
    		if(labels[4][j].getText().equals("R") || labels[4][j].getText().equals("B")){
    			for(int i=1;i<7;i++){
    				if(labels[i][j].getText().equals("R")){
    					count++;
    					if(count==4){
    						result="Won";
    						ResultInitialize();
    					}
    				}
    				else
    	    			count=0;
    				
    				if(labels[i][j].getText().equals("B")){
    					count2++;
    					if(count2==4){
    						result="Lost";
    						ResultInitialize();
    					}
    				}
    				
    				else
    					count2=0;
    			}
    		}
    	}
    	///////////////////Diagonal left////////////////////
    	int start = 3;
    	int row= 3;
    	int col=0;
    	int track = 3;
    	count = 0;
    	count2 = 0;
    	for(int s = 0;s<6;s++){
    		if(s >= 3){
    			track++;
    			col = track;
    			row = 1;
    			start = col;
    		}
    		
    		for(;start<7;start++){
    			if (labels[row][col].getText().equals("R")){
    				count++;
    				if(count==4){
        				result="won";
    	    			ResultInitialize();
        			}
    			}
    			else
    				count=0;
    			
    			if (labels[row][col].getText().equals("B")){
    				count2++;
    				if(count2==4){
        				result="Lost";
    	    			ResultInitialize();
        			}
    			}
    			else
    				count2=0;
    			
    			row++;
    			col++;
    		}
    		
    		if(s < 3){
    			track --;
    			row = track;
    			start = row;
    			col = 0;
    			
    		}
    		
    		
    	}
    	////////////////////Diagonal Right////////////////////
    	start = 3;
    	row= 3;
    	col=6;
    	track = 3;
    	count = 0;
    	
    	for(int s = 0;s<6;s++){
    		if(s >= 3){
    			track--;
    			col = track;
    			row = 1;
    			start = 6 - col;
    		}
    		
    		for(;start<7;start++){
    			if (labels[row][col].getText().equals("R")){
    				count++;
    				if(count==4){
        				result="won";
    	    			ResultInitialize();
        			}
    			}
    			else
    				count=0;
    			
    			if (labels[row][col].getText().equals("B")){
    				count2++;
    				if(count2==4){
        				result="Lost";
    	    			ResultInitialize();
        			}
    			}
    			else
    				count2=0;
    			
    			row++;
    			col--;
    		}
    		
    		if(s < 3){
    			track --;
    			row = track;
    			start = row;
    			col = 6;
    			if(s==2)
    				track = 6;
    		}
    	}
    }
    
    //////////////////////////Button on top of the board and what they do//////////////////////////
    
    private class ButtonListener implements ActionListener
    {
        @Override
        public void actionPerformed(ActionEvent e)
        {
            button = (JButton)(e.getSource());
            
            	for(int i=0; i<1;i++){
            		for(int j=0;j<7;j++){
            			if(buttons[i][j]==button){  //Token being drooped
            				for(int n=6;n>0;n--){
            					if(labels[n][j].getText().equals(" ")){
            						
            						if(turn ==false){	//Alternating turn
            							labels[n][j].setText("B");
            							labels[n][j].setForeground(Color.BLACK);
            							turn = true;
            						}
            							else{
            								labels[n][j].setText(player);
            								labels[n][j].setForeground(Color.RED);
            								turn = false;
            							}
            						//infosent=String.valueOf(n+" "+j);
            						resultcheck(); //Checks for win after token is dropped
            						break;
            					}
            						
            					}
            					}
            			}
            		}
            	
        ////////////Client-server architecure to be used for multiplayer - STILL WIP/////////////////////
            	
            	/*try{
    				Socket sock = new Socket("opponent IP", Opponent Port#); 
    				InputStream recvStream = sock.getInputStream();
    				OutputStream sendStream = sock.getOutputStream();
    			
    				byte[] send = infosent.getBytes();
    				sendStream.write(send, 0, send.length);
    				
    				byte[] recv = new byte[1024];
    				recvStream.read(recv, 0, 1024);
    				
    				
    				recvStream.close();
    				sendStream.close();
    				}catch(IOException e1){
    					System.out.println("Something went wrong");
    				}
    				
                
                
            }
        }
    
   
    
    public class server extends Thread
	{
		public void run()
		{
			try{
				ServerSocket ss = new ServerSocket(15000);
			do{
				Socket sock = ss.accept();
				InputStream recvStream = sock.getInputStream();
				OutputStream sendStream = sock.getOutputStream();
				
				byte[] buffer = new byte[1024];
				recvStream.read(buffer, 0, 1024);
				rev=new String(buffer, 0, 1024);
				int first=Integer.valueOf(rev.charAt(0));
		    	int second=Integer.valueOf(rev.charAt(2));
		    	first=first-48;
		    	second=second-48;
		    	
		    	labels[first][second].setText("B");
		    	losscheck();
		    	
				sendStream.write(buffer, 0, 1024);
				sendStream.close();
			}while(true);
			
			}catch (IOException e)
			{
				System.out.println("Sorry, something went wrong");
			}
		*/		
		}
	}
	

}