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
public class VcxprojDOMParser 
{
    DocumentBuilderFactory builderFactory = DocumentBuilderFactory.newInstance();
    
    Document document = null;
    String fileName = "";
    
    Element clIncludeItemGroup;
    Element clCompileItemGroup;
    Element customItemGroup;
    
    boolean bClInclude = false;
    boolean bClCompile = false;
    boolean bCustom = false;
    
    public static void parseAndOutputFile(File file, String outDir)
    {
        
        VcxprojDOMParser parser = new VcxprojDOMParser();
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
    private Element createClCompile(String includeAttr)
    {
        Element clElem = document.createElement("ClCompile");
        clElem.setAttribute("Include", includeAttr);
        return clElem;
    }
    private Element createClInclude(String includeAttr)
    {
        Element clElem = document.createElement("ClInclude");
        clElem.setAttribute("Include", includeAttr);
        return clElem;
    }
    private Element createIntDir()
    {
        Element clElem = document.createElement("IntDir");
        clElem.setTextContent("$(Platform)_$(Configuration)\\$(ProjectName)\\");
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
                    
                    if (!bClCompile)
                    {
                        clCompileItemGroup = document.createElement("ItemGroup");
                        
                        bClCompile = true;
                    }
                    clCompileItemGroup.appendChild(document.createTextNode("\n    "));
                    String value = elem.getAttribute("Include");
                    System.out.println("ClCompile: " + value);
                    Element child = createClCompile(value);
                    clCompileItemGroup.appendChild(child);
                    
                }
                else if (node.getNodeName().equals("None"))
                {
                    if (!bCustom)
                    {
                        customItemGroup = itemGroupElement;
                        bCustom = true;
                    }
                    
                    if (!bClInclude)
                    {
                        clIncludeItemGroup = document.createElement("ItemGroup");
                        bClInclude = true;
                    }
                    clIncludeItemGroup.appendChild(document.createTextNode("\n    "));
                    String value = elem.getAttribute("Include");
                    System.out.println("ClInclude: " + value);
                    Element child = createClInclude(value);
                    clIncludeItemGroup.appendChild(child);
                    
                }
            }
        }
    }
    private void parsePropertyGroup(Element propertyGroupElement)
    {
        NodeList nodes = propertyGroupElement.getChildNodes();
        boolean bHasOutDir = false;
        for (int i = 0; i < nodes.getLength(); i++)
        {
            Node node = nodes.item(i);
            if (node.getNodeType() == Node.ELEMENT_NODE)
            {
                if (node.getNodeName().equals("OutDir"))
                {
                    bHasOutDir = true;
                    break;
                }
            }
        }
        if (bHasOutDir)
        {
            Element intdirElem = createIntDir();
            propertyGroupElement.appendChild(intdirElem);
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
                if (node.getNodeName().equals("Target"))
                {
                    rootElement.removeChild(node);
                }
                if (node.getNodeName().equals("PropertyGroup"))
                {
                    parsePropertyGroup((Element)node);
                }
            }
        }
        
        // rebuild
        if (bCustom)
        {
            if (bClInclude)
            {
                clIncludeItemGroup.appendChild(document.createTextNode("\n  "));
                rootElement.insertBefore(clIncludeItemGroup, customItemGroup);
                //rootElement.insertBefore(document.createTextNode("\n  "), customItemGroup);
            }
            if (bClCompile)
            {
                rootElement.insertBefore(document.createTextNode("\n  "), customItemGroup);
                clCompileItemGroup.appendChild(document.createTextNode("\n  "));
                rootElement.insertBefore(clCompileItemGroup, customItemGroup);
                
            }
            rootElement.removeChild(customItemGroup);
        }
        

    }
    
    private void parse(File file)
    {
        document = null;
        try 
        {
            bClInclude = false;
            bClCompile = false;
            bCustom = false;
            clIncludeItemGroup = null;
            clCompileItemGroup = null;
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


    

