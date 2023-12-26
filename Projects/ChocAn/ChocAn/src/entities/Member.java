package entities;
import java.util.*;

/**
 * @author aahmed
 *
 */

public class Member {
	public int memberNum; // 9 digits
	public String memberName; // 25 characters
	public String memberAddress; // 25 characters
	public String memberCity; // 14 characters
	public String memberState; //2 characters
	public String memberEmail;
	public int memberZip; // 5 digits
	public int memberAmountOwed;
	boolean memberStatus; //0 for suspended (owes fees), 1 for active
	
	public Member() {
		memberNum = 123456789;
		memberName = "TESTNAME";
		memberAddress = "TESTADDRESS";
		memberCity = "TESTCITY";
		memberState = "State";
		memberEmail = "test@email.com";
		memberZip = 12345;
		memberAmountOwed = 1234567;
		memberStatus = false;
	}
	
	public Member(int nu, String na, String ad, String ci, String state, String email, int zi, int amntOwed, boolean status) {
		memberNum = nu;
		memberName = na;
		memberAddress = ad;
		memberCity = ci;
		memberState = state;
		memberEmail = email;
		memberZip = zi;
		memberAmountOwed = amntOwed;
		memberStatus = status;
		}
	
	public int getNum() {
		return memberNum;
	}
	
	public void setNum(int nu) {
		memberNum = nu;
	}
	
	public String getName() {
		return memberName;
		
	}
	
	public void setName(String na) {
		memberName = na;
	}
	
	public String getAddress() {
		return memberAddress;
	}
	
	public void setAddress(String ad) {
		memberAddress = ad;
	}
	
	public String getCity() {
		return memberCity;
	}
	
	public void setCity(String ci) {
		memberCity = ci;
	}
	
	public String getEmail() {
		return memberEmail;
	}
	
	public void setEmail(String email) {
		memberEmail = email;
	}
	
	public int getZip() {
		return memberZip;
	}
	
	public void setZip(int zi) {
		memberZip = zi;
	}
	
	public int getAmountOwed() {
		return memberAmountOwed;
	}
	
	public void setAmountOwed(int amntOwed) {
		memberAmountOwed = amntOwed;
	}
	
	public boolean getStatus() {
		return memberStatus;
	}
	
	public void setStatus(boolean status) {
		memberStatus = status;
	}
	
	public String getState() {
		return memberState;
	}
	
	public void setState(String state) {
		memberState = state;
	}

}