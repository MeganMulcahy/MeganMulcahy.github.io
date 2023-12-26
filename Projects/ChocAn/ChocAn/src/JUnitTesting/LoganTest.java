package JUnitTesting;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Scanner;
import org.junit.Before;
import org.junit.Test;
import entities.*;
public class LoganTest{
	
	Storage storage;
	Report report;	
	
	@Before
	public void setUp() throws FileNotFoundException {
		storage = new Storage();
		storage.fileToArray();
		report = new Report();
	}
    @Test
    public void GenerateMemberReportTest() throws IOException {
    	report.generateMemberReport(storage);
    	
    	File file = new File("Fall2022Team3/Reports/MemberReports/Alfred_report.txt");
	    Scanner myReader = new Scanner(file);
	    boolean pass = true;
	   
	    if(myReader.nextLine().compareTo("Member name:Alfred") != 0) {
	    	pass = false;
	    	System.out.println("1");
	    }
	    if(myReader.nextLine().compareTo("Member Number: 165568751") != 0) {
	    	pass = false;
	    	System.out.println("2");
	    }
	    if(myReader.nextLine().compareTo("Member Street Address: 111 A Cool Street") != 0) {
	    	pass = false;
	    	System.out.println("3");
	    }
	    if(myReader.nextLine().compareTo("Member City: Miami") != 0) {
	    	pass = false;
	    	System.out.println("4");
	    }
	    if(myReader.nextLine().compareTo("Member State: FL") != 0) {
	    	pass = false;
	    	System.out.println("5");
	    }
	    if(myReader.nextLine().compareTo("Member Zip Code: 13828") != 0) {
	    	pass = false;
	    	System.out.println("6");
	    }
	   
	    myReader.nextLine();
	   
	    if(myReader.nextLine().compareTo("Services:") != 0) {
	    	pass = false;
	    	System.out.println("7");
	    }
	    if(myReader.nextLine().compareTo("	Date of Service: 01-01-2001") != 0) {
	    	pass = false;
	    	System.out.println("8");
	    }
	    if(myReader.nextLine().compareTo("	Provider Name: Abigail") != 0) {
	    	pass = false;
	    	System.out.println("9");
	    }
	    if(myReader.nextLine().compareTo("	Service Name: Therapy") != 0) {
	    	pass = false;
	    	System.out.println("10");
	    }
	    myReader.close();
	   
	    assert(pass);
    }
    @Test
    public void GenerateProviderReport() throws IOException {
    	report.generateProviderReport(storage);
    	
    	File file = new File("Fall2022Team3/Reports/ProviderReports/Abigail_report.txt");
	    Scanner myReader = new Scanner(file);
	    boolean pass = true;
	   
	    if(myReader.nextLine().compareTo("Provider name: Abigail") != 0) {
	    	pass = false;
	    	System.out.println("1");
	    }
	    if(myReader.nextLine().compareTo("Provider Number: 193836593") != 0) {
	    	pass = false;
	    	System.out.println("2");
	    }
	    if(myReader.nextLine().compareTo("Provider Street Address: 101 Some Street") != 0) {
	    	pass = false;
	    	System.out.println("3");
	    }
	    if(myReader.nextLine().compareTo("Member City: Tuscaloosa") != 0) {
	    	pass = false;
	    	System.out.println("4");
	    }
	    if(myReader.nextLine().compareTo("Member State: AL") != 0) {
	    	pass = false;
	    	System.out.println("5");
	    }
	    if(myReader.nextLine().compareTo("Member Zip Code: 14748") != 0) {
	    	pass = false;
	    	System.out.println("6");
	    }
	   
	    myReader.nextLine();
	   
	    if(myReader.nextLine().compareTo("Services:") != 0) {
	    	pass = false;
	    	System.out.println("7");
	    }
	    if(myReader.nextLine().compareTo("	Date of Service: 01-01-2001") != 0) {
	    	pass = false;
	    	System.out.println("8");
	    }
	    if(myReader.nextLine().compareTo("	Date of Service Report: 00-00-0000 00:00:00") != 0) {
	    	pass = false;
	    	System.out.println("9");
	    }
	    if(myReader.nextLine().compareTo("	Member Name: Alfred") != 0) {
	    	pass = false;
	    	System.out.println("10");
	    }
	    if(myReader.nextLine().compareTo("	Member Number: 165568751") != 0) {
	    	pass = false;
	    	System.out.println("11");
	    }
	    if(myReader.nextLine().compareTo("	Service Code: 123456") != 0) {
	    	pass = false;
	    	System.out.println("12");
	    }
	    if(myReader.nextLine().compareTo("	Fee to be paid: 1000") != 0) {
	    	pass = false;
	    	System.out.println("13");
	    }
	    
	    myReader.nextLine();
	    myReader.nextLine();
	    
	    if(myReader.nextLine().compareTo("Total number of consultations with members: 1") != 0) {
	    	pass = false;
	    	System.out.println("14");
	    }
	    
	    if(myReader.nextLine().compareTo("Total fee for week: 1000") != 0) {
	    	pass = false;
	    	System.out.println("15");
	    }
	    
	    myReader.close();
	   
	    assert(pass);
    }
    
    @Test
    public void someoneElses() {
    	
    }
}
