package JUnitTesting;

import static org.junit.Assert.assertTrue;
import static org.junit.jupiter.api.Assertions.*;

import java.io.FileNotFoundException;
import java.util.Scanner;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import entities.Operator;
import entities.OperatorTerminal;
import entities.Provider;
import entities.ServiceRecord;
import entities.Storage;

public class BrookeTest {

	ServiceRecord serviceRecord; 
	OperatorTerminal operatorTerminal;
	Storage storage;
	
	@BeforeEach
	public void setUp() throws Exception {
		serviceRecord = new ServiceRecord();
		operatorTerminal = new OperatorTerminal();	
	}

	@Test
	public void testAddProviderForSuccess() throws FileNotFoundException {
		Storage s = new Storage();
		s.fileToArray();
		int num = 111111111;
		operatorTerminal.addProviderInfo(num, "Brooke", "1234 Campus Dr", "Tuscaloosa", "Alabama", "beboskus@rolltide.com", 121212, s);
		int size = s.providerList.size();
		assertTrue((s.providerList.get(size-1).getNumber()) == 111111111);
		
	}
	

	@Test
	public void testServiceDateTimeForFail() throws FileNotFoundException {
		boolean test = serviceRecord.setCurrentDateTime("12.30.40");
		assertEquals(false,test);
	}
	

	@Test
	public void testServiceDateTimeForSuccess() throws FileNotFoundException {
		boolean test = serviceRecord.setCurrentDateTime("12:30:40");
		assertEquals(true,test);
	}
	
	
	@Test
	public void testDateOfServiceForSuccess() throws FileNotFoundException {
		boolean test = serviceRecord.setDateOfService("01-30-2003");
		assertEquals(true,test);
	}
	
	
	@Test
	public void testDateOfServiceForFails() throws FileNotFoundException {
		boolean test = serviceRecord.setDateOfService("01:30:2003");
		assertEquals(false,test);
	}
	
}
