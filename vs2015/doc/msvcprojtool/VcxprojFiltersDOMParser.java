/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package msvcprojtool;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

/**
 *
 * @author liuxiufeng
 */
public class VcxprojFiltersDOMParser {
    DocumentBuilderFactory builderFactory = DocumentBuilderFactory.newInstance();
    
    Document document = null;
    String fileName = "";
    
    Element clItemGroup;
    Element customItemGroup;
    
    boolean bCl = false;
    boolean bCustom = false;
    
    public static void parseAndOutputFile(File file, String outDir)
    {
        
        VcxprojFiltersDOMParser parser = new VcxprojFiltersDOMParser();
        parser.parse(file);
        parser.parseRootElement();
        parser.output(outDir);
    }
    private void output(String outDir)
    {
        try
        {
            String filePath = outDir + "\\" + fileName;
            TransformerFactory tFactory = TransformerFactory.newInstance();
            Transformer transformer =  tFactory.newTransformer();
            transformer.setOutputProperty(OutputKeys.INDENT, "yes");

            DOMSource source = new DOMSource(document);
            //StreamResult result = new StreamResult(System.out);
            StreamResult result = new StreamResult(new FileOutputStream(new File(filePath)));
            transformer.transform(source, result);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }

    }
    private Element createClCompile(Element elem)
    {
        String value = elem.getAttribute("Include");
        System.out.println("ClCompile: " + value);
        Element clElem = document.createElement("ClCompile");
        clElem.setAttribute("Include", value);
        
        NodeList nodes = elem.getChildNodes();
        for (int i = 0; i < nodes.getLength(); i++)
        {
            Node childNode = nodes.item(i);
            if (childNode.getNodeType() == Node.ELEMENT_NODE)
            {
                if (childNode.getNodeName().equals("Filter"))
                {
                    String filterValue = childNode.getTextContent();
                    clElem.appendChild(document.createTextNode("\n      "));
                    Element filterElem = document.createElement("Filter");  
                    System.out.println(filterValue);
                    filterElem.setTextContent(filterValue); 
                    clElem.appendChild(filterElem);
                    clElem.appendChild(document.createTextNode("\n    "));
                }
                
            }
        }
        
        
        return clElem;
    }
    private Element createClInclude(Element elem)
    {
        String value = elem.getAttribute("Include");
        System.out.println("ClCompile: " + value);
        Element clElem = document.createElement("ClInclude");
        clElem.setAttribute("Include", value);
        
        NodeList nodes = elem.getChildNodes();
        for (int i = 0; i < nodes.getLength(); i++)
        {
            Node childNode = nodes.item(i);
            if (childNode.getNodeType() == Node.ELEMENT_NODE)
            {
                if (childNode.getNodeName().equals("Filter"))
                {
                    String filterValue = childNode.getTextContent();
                    clElem.appendChild(document.createTextNode("\n      "));
                    Element filterElem = document.createElement("Filter");  
                    System.out.println(filterValue);
                    filterElem.setTextContent(filterValue); 
                    clElem.appendChild(filterElem);
                    clElem.appendChild(document.createTextNode("\n    "));
                }
                
            }
        }
        return clElem;
    }
    private void parseItemGroup(Element itemGroupElement)
    {
        NodeList nodes = itemGroupElement.getChildNodes();
        for (int i = 0; i < nodes.getLength(); i++)
        {
            Node node = nodes.item(i);
            if (node.getNodeType() == Node.ELEMENT_NODE)
            {
                Element elem = (Element)node;
                if (node.getNodeName().equals("CustomBuild"))
                {   
                    if (!bCustom)
                    {
                        customItemGroup = itemGroupElement;
                        bCustom = true;
                    }
                    
                    if (!bCl)
                    {
                        clItemGroup = document.createElement("ItemGroup");
                        
                        bCl = true;
                    }
                    clItemGroup.appendChild(document.createTextNode("\n    "));                   
                    Element child = createClCompile(elem);
                    clItemGroup.appendChild(child);
                    
                }
                else if (node.getNodeName().equals("None"))
                {
                    if (!bCustom)
                    {
                        customItemGroup = itemGroupElement;
                        bCustom = true;
                    }
                    
                    if (!bCl)
                    {
                        clItemGroup = document.createElement("ItemGroup");
                        
                        bCl = true;
                    }
                    clItemGroup.appendChild(document.createTextNode("\n    "));
                    Element child = createClInclude(elem);
                    clItemGroup.appendChild(child);
                    
                }
            }
        }
    }
    private void parseRootElement()
    {
        
        Element rootElement = document.getDocumentElement();
        
        if (rootElement.getNodeName() != "Project")
        {
            System.out.println(fileName + " : has no Project Element");
            return;
        }
        
        
        NodeList nodes = rootElement.getChildNodes();
        for (int i = 0; i < nodes.getLength(); i++)
        {
            Node node = nodes.item(i);
            if (node.getNodeType() == Node.ELEMENT_NODE)
            {
                if (node.getNodeName().equals("ItemGroup"))
                {
                    parseItemGroup((Element)node);
                }
            }
        }
        
        // rebuild
        if (bCustom)
        {
            if (bCl)
            {
                rootElement.replaceChild(clItemGroup, customItemGroup);
            }
        }
        

    }
    
    private void parse(File file)
    {
        document = null;
        try 
        {
            bCl = false;
            bCustom = false;
            clItemGroup = null;
            DocumentBuilder builder = builderFactory.newDocumentBuilder();
            
            fileName = file.getName();
            document = builder.parse(file);
            
        } catch (ParserConfigurationException e) { 
            e.printStackTrace();  
      
        } catch (SAXException e) { 
         
            e.printStackTrace(); 
      
        } catch (IOException e) { 
        
            e.printStackTrace(); 
      
        } 
        
    }
    
    
    
}
