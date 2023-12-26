package entities;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.ArrayList;

public class TESTBED {
	public static void main(String[] args) throws FileNotFoundException
	{
		// You may need to change this to "Fall2022Team3/src/entities/[textfile].txt"
		// For some reason my (Alex) computer will freak out if I have it in front.
		File file = new File("fall2022team3/src/entities/Providers2.txt");
		
		
		Scanner sc = new Scanner(file);
		
		// Create an ArrayList whose data type is String[].
		// We will use this to store each line from the text file.
		ArrayList<String[]> fileOutput = new ArrayList<String[]>();
		
		// Create the "holding pen" for the current line we're working with.
		// One String array, one String.
		String[] holdingArray;
		String currentLine = "";
		
		// This loop will continue as long as the Scanner object attached to our file
		// has another line to analyze.
		while(sc.hasNextLine()) { 
			currentLine = sc.nextLine();
			holdingArray = currentLine.split(","); // This function splits a line into an array.
			fileOutput.add(holdingArray); // We then append that array to the end of the ArrayList.
		}
		
		
		// TESTING FOLLOWS BEYOND THIS POINT
		
		for (String[] eachString : fileOutput) {
            for (String var : eachString) {
            	System.out.print(var); 
            	// Prints out every element in the Array at the current location
            	// in the ArrayList.
            }
            System.out.println();
        }
		System.out.println(fileOutput.get(0)[1]); // This should output "Megan Mulcahy".
		System.out.println(fileOutput.get(1)[1]); // This should output "Addison Amston".
		System.out.println(fileOutput.get(1)[5]); // This should output "22222".
		sc.close();
	}
}
