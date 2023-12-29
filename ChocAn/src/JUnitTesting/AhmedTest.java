package JUnitTesting;

import java.io.IOException;
import static org.junit.Assert.*;
import org.junit.Before;
import org.junit.Test;

import entities.OperatorTerminal;
import entities.ProviderDirectory;
import java.util.*;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import entities.Service;
import entities.ServiceRecord;
import entities.Storage;


public class AhmedTest {

	OperatorTerminal operatorTerminal;
	ServiceRecord serviceRecord;
	
	@Before
	public void setUp() throws Exception {
		operatorTerminal = new OperatorTerminal();
		serviceRecord = new ServiceRecord();

	}
	
	@Test
	public void testDeleteProviderForFail() throws FileNotFoundException{
		Storage s = new Storage();
		s.fileToArray();
		int number = 123456789;
		operatorTerminal.deleteProviderInfo(number, s);
		boolean flag = false;
		for(int i = 0; i < s.providerList.size(); i++) {
			if(s.providerList.get(i).getNumber() == number) {
				flag = true;
			}
		}
		assertFalse(flag);
		}
	
	@Test
	public void testDeleteProviderForSuccess() throws FileNotFoundException{
		Storage s = new Storage();
		s.fileToArray();
		int number = 467525164;
		boolean flag = false;
		for(int i = 0; i < s.providerList.size(); i++) {
			if(s.providerList.get(i).getNumber() == number) {
				flag = true;
			}
		}
		operatorTerminal.deleteMemberInfo(number, s);
		assertTrue(flag);
		}
	
	@Test
	public void testAddProviderForSuccess() throws FileNotFoundException{
		Storage s = new Storage();
		s.fileToArray();
		int number = 123456789;
		operatorTerminal.addProviderInfo(number, "Ahmed", "650 6th Ave", "Tuscaloosa", "AL", "aahmed6@crimosn.ua.edu", 35444, s);
		int size = s.providerList.size();
		assertTrue((s.providerList.get(size-1).getNumber()) == 123456789);  
		}
}