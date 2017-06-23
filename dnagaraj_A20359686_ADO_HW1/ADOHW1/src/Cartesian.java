import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;


public class Cartesian {

	private static Scanner fileScanner1;
	private static Scanner fileScanner2;
	
	// Scans and reads to input files. Each row is considered as a tuple and column as attributes.
	File fileHandler1 = new File("src/table1.txt");
	File fileHandler2 = new File("src/table2.txt");

	
	//Method to do cartesian product 
	public void cartesianProduct() throws IOException {
		try {

			fileScanner1 = new Scanner(fileHandler1);
			fileScanner2 = new Scanner(fileHandler2);
			List<String> tuple = new ArrayList<String>();
			while (fileScanner1.hasNext()) {
				String firstfileln = fileScanner1.nextLine(); // Each line of the first file is read and added to a list.
				tuple.add(firstfileln);
			}
			List<String> tupletwo = new ArrayList<String>();
			while (fileScanner2.hasNext()) {
				String secfileln = fileScanner2.nextLine(); // Each line of the second file is read and added to a list.
				tupletwo.add(secfileln);
			}
			System.out.print("cartesian product \n");
			for(int i=0;i<tuple.size();i++){    // Each line in the first file is combined with each line of second file to do the cartesian product.
				for(int j=0;j<tupletwo.size();j++){
					ArrayList<String> cartprod=new ArrayList<String>();
					List<String> tupleone = new ArrayList<String>();
					tupleone.add(tuple.get(i));
					List<String> tuplethree=new ArrayList<String>();
					tuplethree.add(tupletwo.get(j));
					cartprod.addAll(tupleone);
					cartprod.addAll(tuplethree);
					for (String temp : cartprod){
						System.out.print(temp);
					}
					System.out.print("\n");
				}
			}

		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.out.print("\n");
	}

	
	// Method to do Natural Join
	public void naturalJoin() throws FileNotFoundException{

		fileScanner1 = new Scanner(fileHandler1);
		ArrayList<String> naturaljoin=new ArrayList<String>();
		System.out.print("Natural Join \n");
		while (fileScanner1.hasNext()) {
			String line = fileScanner1.nextLine();
			String[] firtabtokens = line.split("[ ]"); //Each line of the first file is read and split into elements.
			Scanner fileScanner2 = new Scanner(fileHandler2);
			while (fileScanner2.hasNext()){
				String sectabline = fileScanner2.nextLine();
				String[] sectabtokens = sectabline.split("[ ]"); //Each line of the second file is read and split into elements.
				ArrayList<String> rows=new ArrayList<String>();
				if((firtabtokens[0].equals(sectabtokens[0]))){  //First column in each file is considered as primary key and checked whether the value is same
					// If values are same natural join is done with two tuples in two files.
					rows.add(firtabtokens[0]);
					rows.add(firtabtokens[1]);
					rows.add(firtabtokens[2]);
					rows.add(sectabtokens[1]);
					rows.add(sectabtokens[2]);
					for (String temp:rows){
						System.out.print(temp+" ");
					}
					System.out.print("\n");
				}
			}
		}
		System.out.print("\n");			
	}

// Method to do left outer join
	public void outerjoin() throws FileNotFoundException{
		fileScanner1 = new Scanner(fileHandler1);
		System.out.print("Left Outer Join \n");
		int tabonecol;
		int tabtwocol;
		while (fileScanner1.hasNext()) {
			int count = 0;
			int outerjoinrowlength=0;
			String line = fileScanner1.nextLine();
			String[] firtabtokens = line.split("[ ]"); //Each line of the first file is read and split into elements.
			tabonecol=firtabtokens.length;
			Scanner fileScanner2 = new Scanner(fileHandler2);
			ArrayList<String> rows=new ArrayList<String>();
			while (fileScanner2.hasNext()){
				String sectabline = fileScanner2.nextLine();
				String[] sectabtokens = sectabline.split("[ ]"); //Each line of the first file is read and split into elements.
				tabtwocol=sectabtokens.length;
				outerjoinrowlength = tabonecol + tabtwocol -1;
				if((firtabtokens[0].equals(sectabtokens[0]))){  //First column in each file is considered as primary key and checked whether the value is same
					// If values are same left outer join is done with two tuples in two files.
					count++;
					for(int i=0;i<tabonecol;i++){
						rows.add(firtabtokens[i]);
					}
					for(int j=1;j<tabtwocol;j++){
						rows.add(sectabtokens[j]);
					}
					for (String temp:rows){
						System.out.print(temp+" ");
					}
					System.out.print("\n");
				}
			}
			if(count<1){ //If values are not same left outer join is done and the other  attribute values are null.
				int counter=0;
				for(int i=0;i<outerjoinrowlength;i++){
					if(i<tabonecol){
						rows.add(firtabtokens[i]);}
					else
					{rows.add(null);	}

				}
				for (String temp:rows){
					System.out.print(temp+" ");
				}
				System.out.print("\n");
			}
		}
		System.out.print("\n");			
	}

	public static void main(String[] args) throws IOException {
		// TODO Auto-generated method stub
		Cartesian product = new Cartesian();
		product.cartesianProduct(); // Call cartesian product method
		product.naturalJoin(); // Call natural join method
		product.outerjoin(); // Call left outer join method

	}

}
