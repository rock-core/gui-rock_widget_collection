#ifndef XMLCONFIGWRITER_H
#define	XMLCONFIGWRITER_H

#include <iostream>
#include <encoding.h>
#include <xmlwriter.h>

/**
 * Abstract XML Writer class for configuration files.
 * The method writeContent must be overwritten in subclasses.
 * When calling writeConfigFile a simple xml file will be written with one
 * configuration tag. After this tag the abstract writeContent method will be
 * called. If it is empty a simple xml document with exactly one configuration tag
 * will result.
 * To add nodes some convenient methods are available:<p>
 * <b>writeTag(const char* name, const char* content)</b><br>
 * This method will write a tag and close it. In between the content, if any
 * will be written, like this: &lt;tag&gt;content&lt;/tag&gt;. This method is best
 * used for simple tags.
 * <p>
 * <b>startTag(const char* name)</b><br>
 * This will start a tag but offers the ability via other methods to write attributes
 * and subnodes to the xml file.<p>
 * <b>endTag()</b><br>
 * This will end the last node opened with startTag.<p>
 * <b>writeAttribute(const char* name, const char* value)</b><br>
 * this will write an attribute to a node opened via startTag. The name of
 * the attribute and its value will be given, resulting in something like
 * this: &lt;tag name="value"&gt; <p>
 * <b>writeTagContent(const char* content)</b><br>
 * This will write the content between a node, like &lt;tag>content&lt;/tag&gt;. It uses
 * the last opened node, opened by startTag.
 *
 * <p>
 * <h2>Example</h2>
 *
 * startTag("tag1");<br>
 * writeTag("subtag1", "SubContent");<br>
 * startTag("subtag2");<br>
 * writeAttribute("subattribute", "att");<br>
 * writeTagContent("SubContent2");<br>
 * endTag();<br>
 * endTag();<br>
 * <p>
 *
 * This code results in:
 *
 * &lt;tag1&gt;<br>
 *  &lt;subtag1&gt;SubContent&lt;/subtag1&gt;<br>
 *  &lt;subtag2 subattribute="att"&gt;SubContent2&lt;/subtag2&gt;<br>
 * &lt;/tag1&gt;<br>
 *
 * <p>
 * <h2>History</h2>
 * 12-15-2010 renamed all node methods to tag method for clarity
 * 12-15-2010 created documentation
 * <p>
 *
 * @todo Handle different encodings, currently only "ISO-8859-1" is used
 * @todo Handle different floating points (locale dependent), currently only . is allowed
 * @todo Typesafe methods, currently only char values<
 * @todo Compression of the file may be configurable
 * @todo Do not start with configuration tag, to create multiple configuration file formats
 *
 */
class XMLConfigWriter
{
public:
    /**
     * Empty constructor
     */
    XMLConfigWriter();

    /**
     * Destructor
     */
    virtual ~XMLConfigWriter();

    /**
     * Writes a configuration file by specifying xml header
     * information and starting with a configuration tag.
     * Then the abstract writeContent will be called and if
     * successfull, teh configuration tag will be closed and
     * the xml file written.
     * @param filename the name of the file to write the xml document to
     * @return true if successfull, false otherwise
     */
    bool writeConfigFile(char* filename);

    /**
     * Abstract method which needs to be overwritten in
     * subclasses. This will be called from within writeConfigFile and all
     * Node writing should take place here.
     * @return true if successfull, false otherwise
     */
    virtual bool writeContent() = 0;

    /**
     * Starts a Tag with the given name
     * @param name the name of the tag
     * @return true if successfull, false otherwise
     */
    bool startTag(const char* name);

    /**
     * Starts and ends a tag with simple content (no attributes or sub nodes)
     * @param name the name of the tag
     * @param content teh content of the tag
     * @return true if successfull, false otherwise
     */
    bool writeTag(const char* name, const char* content = 0);

    /**
     * Writes the content for a node whioch was opened via
     * satrtNode.
     * @param content the content to be written
     * @return true if successfull, false otehrwise
     */
    bool writeTagContent(const char* content);

    /**
     * Writes an attrribute to a tag which was opened by startTag
     * @param name the anem of the attribute
     * @param value the value of the attribute
     * @return true if successfull, false otherwise
     */
    bool writeAttribute(const char* name, const char* value);

    /**
     * Ends a tag. Afterwards nothing can be added to it via the writer
     * @return true if successfull, false otherwise
     */
    bool endTag();
protected:
    /** The actual writer*/
    xmlTextWriterPtr writer;
    /** static encoding*/
    const char* encoding;

};

#endif	/* XMLCONFIGWRITER_H */

