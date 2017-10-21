import java.net.*;
import java.io.*;


class StreetGangClient
{
	public static void main(String args[]) throws IOException 
	{
		System.out.println("sending request to server....");
		Socket client=new Socket("127.0.0.1", 7390);
		System.out.println("successfully conneted");
		
		BufferedReader br1=new BufferedReader(new InputStreamReader(System.in));
		PrintStream ps=new PrintStream(client.getOutputStream());
		BufferedReader br=new BufferedReader(new InputStreamReader(client.getInputStream()));
		
		while(true)
		{
			System.out.println("input the data u want to send to echo server: ");
			String s=br1.readLine();
			ps.println(s);
			
			if(s.equals("exit"))
			{
				System.exit(1);
			}
			
			String st=br.readLine();
			System.out.println("data returned by the server:   ");
			System.out.println(st);
		}
	}
}

