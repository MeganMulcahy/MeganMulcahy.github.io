package JUnitTesting;

import static org.junit.jupiter.api.Assertions.*;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import entities.Member;
import entities.Operator;
import entities.OperatorTerminal;
import entities.ServiceRecord;
import entities.Storage;

class MeganTest {

	OperatorTerminal operatorTerminal;
	ServiceRecord serviceRecord;
	
	@BeforeEach
	void setUp() throws Exception {
		operatorTerminal = new OperatorTerminal();
		serviceRecord = new ServiceRecord();

	}

	@Test
	public void testAddMemberForSuccess() throws FileNotFoundException{
		Storage s = new Storage();
		s.fileToArray();
		int number = 123456789;
		operatorTerminal.addMemberInfo(number, "megan", "1234 street", "dallas", "NJ", "mgmulcahy@crimson.ua.edu", 12345, 0, true, s);
		int size = s.memberList.size();
		assertTrue((s.memberList.get(size-1).getNum()) == 123456789);  
		}
	@Test
	public void testDeleteMemberForSuccess() throws FileNotFoundException{
		Storage s = new Storage();
		s.fileToArray();
		int number = 467525164;
		boolean flag = false;
		for(int i = 0; i < s.memberList.size(); i++) {
			if(s.memberList.get(i).getNum() == number) {
				flag = true;
			}
		}
		operatorTerminal.deleteMemberInfo(number, s);
		assertTrue(flag);
		}
	@Test
	public void testDeleteMemberForFail() throws FileNotFoundException{
		Storage s = new Storage();
		s.fileToArray();
		int number = 123456789;
		operatorTerminal.deleteMemberInfo(number, s);
		boolean flag = false;
		for(int i = 0; i < s.memberList.size(); i++) {
			if(s.memberList.get(i).getNum() == number) {
				flag = true;
			}
		}
		assertFalse(flag);
		}
	@Test
	public void testSetCurrentTimeDateForFail() throws FileNotFoundException{
		boolean test = serviceRecord.setCurrentDateTime("11/2/2022 00:00:00");
		assertFalse(test);
	}
	public void testSetCurrentTimeDateForSuccess() throws FileNotFoundException{
		boolean test = serviceRecord.setCurrentDateTime("11-2-2022 00:00:00");
		assertTrue(test);
	}
	
}
