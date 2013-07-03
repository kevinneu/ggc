package com.dbdky.tools;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import javax.xml.transform.Source;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;
import javax.xml.validation.Validator;
//import org.xml.sax.SAXEXception;

public class ConfValid {
    public static void main(String[] args) {
        boolean ret = false;
        if (args.length != 2) {
            return;
        }

        StringBuilder builder = new StringBuilder();
        builder.append(args[0]);
        builder.append("\t");
        builder.append(args[1]);
        builder.append("\t");
        
        try {
            ret = validateXmlWithSchema(args[0], args[1]);
            if (ret) {
                builder.append("OK");
            }
            else
            {
                builder.append("FAIL");
            }
        } catch (Exception e) {
            builder.append("FAIL");
        }

        System.out.println(builder.toString());
    }

    private static boolean validateXmlWithSchema(String schemaFileName,
			String xmlFileName) 
	//throws SAXException, IOException {
        throws Exception {
        boolean ret = false;
        SchemaFactory schemaFactory = SchemaFactory.newInstance("http://www.w3.org/2001/XMLSchema");
        File schemaFile = new File(schemaFileName);
        Schema schema = schemaFactory.newSchema(schemaFile);
        Validator validator = schema.newValidator();
        Source source = new StreamSource(xmlFileName);
        try {
            validator.validate(source);
            ret = true;
        } catch (Exception e) {
            ret = false;
        }

        return ret;
    }
}
