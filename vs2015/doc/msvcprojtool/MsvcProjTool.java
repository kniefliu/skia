/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package msvcprojtool;

import java.io.*;
import javax.xml.parsers.*;
import javax.xml.transform.OutputKeys;
import org.w3c.dom.*;
import org.xml.sax.SAXException;

import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.dom.DOMSource; 
import javax.xml.transform.stream.StreamResult;



/**
 *
 * @author liuxiufeng
 */
public class MsvcProjTool {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        File rootDir = new File("E:\\code\\osknief\\skia_knief\\vs2015\\x64_shared_debug\\obj");
        String outDirName = "E:\\code\\osknief\\skia_knief\\vs2015\\x64_shared_debug\\vcxproj";
        processDirs(rootDir, outDirName);
    }
    
    private static void processDirs(File dir, String outDirName)
    {
        File outDir = new File(outDirName);
        if (!outDir.exists())
        {
            outDir.mkdir();
        }
        for (File f : dir.listFiles()) {
            
            if (f.isDirectory())
            {
                processDirs(f, outDirName + "\\" + f.getName());
                continue;
            }
            
            String filename = f.getName();
            
            if (filename.endsWith(".vcxproj"))
            {   
                System.out.println("fileName:" + filename);
                processVcxproj(f, outDirName);
            }
            if (filename.endsWith(".vcxproj.filters"))
            {
                System.out.println("fileName:" + filename);
                processVcxprojFilters(f, outDirName);
            }
                    
        }
    }
    
    private static void processVcxproj(File f, String outDir)
    {
        VcxprojDOMParser.parseAndOutputFile(f, outDir);       
        
    }
    private static void processVcxprojFilters(File f, String outDir)
    {
        VcxprojFiltersDOMParser.parseAndOutputFile(f, outDir);
    }
    
}
