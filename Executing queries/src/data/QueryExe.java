package data;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class QueryExe {
	
	private static Scanner fileScanner1;
	private static String[][] arrvalue = new String[6][3];
	
     String line = " ";
     String [] tempsto;
     
     int lines = 0 ;
	File fileHandler1 = new File("src/data/table.txt");
	
	
public QueryExe() throws IOException {
		BufferedReader reader = new BufferedReader(new FileReader("src/data/table.txt"));
		while (reader.readLine() != null) 
			lines++;
		reader.close();
}

public void readFile() throws IOException{
	
	//Let the columns of the array be A, B, C and Relation be R
	
		try{
			fileScanner1 = new Scanner(fileHandler1);
			int i =0;
			while (fileScanner1.hasNext()) {
				String firstfileln = fileScanner1.nextLine();
				tempsto = firstfileln.split(" ");
				for (int j = 0; j< tempsto.length; j++) { 
		                    arrvalue[i][j] = tempsto[j];  
		                }
				i++;
		            }
			
		}
		 catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		printTable();
		selectQuery();
		groupQuery();
		}
public void printTable(){
		System.out.print("The given file is :" +"\n");
		System.out.print("A"+" "+"B"+" "+"C"+"\n");
		
	       for (int i =0; i <arrvalue.length; i++) {
	            for (int j = 0; j < tempsto.length; j++) {
	              System.out.print(arrvalue[i][j]+" ");
	            }
	            System.out.print("\n");
	        }
	       System.out.print("\n");
	       }
	       
public void selectQuery(){
	       System.out.print("The given file is considered as table R" +"\n");
	       System.out.print("\n");
	       System.out.print("The query "+'"'+"Select * from R where C = b"+'"'+" is executed" +"\n");
		for (int i =0; i <arrvalue.length; i++){
				int j=2;// Since C is the third column 
				if((arrvalue[i][j]).equals("b")){
					for (int k = 0; k <tempsto.length; k++){
						System.out.print(arrvalue[i][k]+" ");
					}
					System.out.print("\n");
				}
		}
			
		}

public void groupQuery(){
	List<Object> grouped = new ArrayList<Object>();
	for (int i =0; i <arrvalue.length; i++){
		
		int j =2,a=0;
	int sum=0;
	List<String> row = new ArrayList<>();
		for (int k =0; k <arrvalue.length; k++){
			
			if((arrvalue[i][j]).equals(arrvalue[k][j])){
				sum += Integer.parseInt(arrvalue[k][a]);
				}
			
		}
		String total=String.valueOf(sum);
		row.add(total);
		row.add(arrvalue[i][j]);
		
		if(!grouped.contains(row))
		{
		grouped.add(row);}

	}
	 
      System.out.print("\n");
      System.out.print("The query "+'"'+"Select sum(A),C from R group by C"+'"'+" is executed" +"\n");
      System.out.print("\n");
	
	for (Object temp:grouped){
		System.out.print(temp+" ");
		System.out.print("\n");
		
	
	}
	
}

	public static void main(String[] args) throws IOException {
		// TODO Auto-generated method stub
QueryExe query = new QueryExe();
query.readFile();
	}

}
