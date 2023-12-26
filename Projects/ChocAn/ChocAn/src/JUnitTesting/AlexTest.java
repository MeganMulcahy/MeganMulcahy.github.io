package JUnitTesting;
import java.io.IOException;

import static org.junit.Assert.assertTrue;
import static org.junit.jupiter.api.Assertions.*;

import java.io.FileNotFoundException;
import java.util.Scanner;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import entities.Provider;
import entities.ProviderTerminal;
import entities.ServiceRecord;
import entities.Storage;
@SuppressWarnings("unused")

public class AlexTest {
	ProviderTerminal pterm;
	Storage s;
	ServiceRecord sr;
	
	@Test
	public void testVerifyMemberFail() throws FileNotFoundException {
		pterm = new ProviderTerminal();
		s = new Storage();
		s.fileToArray();
		int result = pterm.verifyMember(373647583, s);
		int expectedResult = 0; // verifyMember should return 0 for an invalid member.
		assertEquals(expectedResult, result);
	}
	
	@Test
	public void testVerifyMemberPass() throws FileNotFoundException {
		pterm = new ProviderTerminal();
		s = new Storage();	
		s.fileToArray();
		int result = pterm.verifyMember(879584202, s);
		int expectedResult = 1; // verifyMember should return 1 for a valid member.
		assertEquals(expectedResult, result);
	}
	
	/*
	@Test
	public void testRequestDirectoryPass() {
		pterm.providerMenu(sc, Storage s, Provider p) {
	}
	
	public void testRequestDirectoryFail() {
		
	}
	*/
	
	@Test
	public void testSetCurrentTimeDateForFail() throws FileNotFoundException {
		sr = new ServiceRecord();
		boolean result = sr.setCurrentDateTime("02/16/2003 00:00:00");
		assertFalse(result);
	}
	
	@Test
	public void testSetCurrentTimeDateForSuccess() throws FileNotFoundException {
		sr = new ServiceRecord();
		boolean result = sr.setCurrentDateTime("02-16-2003 00:00:00");
		assertTrue(result);
	}
}
