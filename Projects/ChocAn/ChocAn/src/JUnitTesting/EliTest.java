package JUnitTesting;

import java.io.IOException;
import static org.junit.Assert.*;
import org.junit.Before;
import org.junit.Test;
import entities.ProviderDirectory;
import java.util.*;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import entities.Service;

public class EliTest {
    
    @Test
    public void read() throws IOException {
        String file = "C:\\Users\\elich\\git\\fall2022team3\\Fall2022Team3\\src\\JUnitTesting\\EliTest.txt";
        ArrayList<String[]> testArr = ProviderDirectory.readFile(file);
        if (testArr != null) {
            assert(true);
        }

        try {
        	file = "badpath.txt";
            testArr = ProviderDirectory.readFile(file);
        } catch (Exception e) {
            assert(true);
        }

    }

    @Test
    public void write() throws IOException {
        try {
            String file = "EliTest.txt";
            ArrayList<String[]> testArr = ProviderDirectory.readFile(file);
            ProviderDirectory.writeToFile("EliTest2.txt", testArr);
            File f = new File("EliTest2");
            if(f.exists()) {
                assert(true);
            }
        } catch (Exception e) {
            assert(true);
        }
    }

    @Test
    public void testServiceObject() {
        Service testService = new Service(123456, "testname", 15);

        assert(testService.getServiceCode() == 123456);
        assert(testService.getServiceFee() == 15);
        assert(testService.getServiceName() == "testname");

        assert(testService.getServiceCode() != 23456);
        assert(testService.getServiceFee() != 5);
        assert(testService.getServiceName() != "estname");

        testService.setServiceName("estname");
        testService.setServiceFee(5);
        testService.setServiceCode(23456);

        assert(testService.getServiceCode() == 23456);
        assert(testService.getServiceFee() == 5);
        assert(testService.getServiceName() == "estname");

    }



}
