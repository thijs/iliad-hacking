/*
 * This file is part of browser.
 *
 * browser is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * browser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 */


static xmlDoc *theManifest = NULL;



int manifest_get_page_count(char* type)
{
    parseIndexdata()
}


// xmlDocPtr theManifest, xmlNodePtr cur, clDisplayItem_t * displayItem, char *szContainerPath
static void getPagelistInfo(xmlChar *type)
{
    cur = indexNode->children;
    
    while (cur != NULL)
    {
        if (!xmlStrcmp(cur->name, (const xmlChar *) "pagelist"))
        {
            CL_LOGPRINTF("Found node pagelist");
            if (!xmlStrcmp(type, xmlGetProp(cur, (const xmlChar *) "type")))
            {
                return xmlGetProp(cur, (const xmlChar *) "count");
            }
        }
        cur = cur->next;
    }
    return;
}

int erBrXmlParseManifest(char *szFilename)
{
    xmlNode *rootElement = NULL;
    xmlNode *cur = NULL;

    theManifest = xmlParseFile(szFilename);

    if (theManifest == NULL)
    {
        CL_ERRORPRINTF("Document not parsed successfully");
        return -1;
    }

    /* Get the root element node */
    rootElement = xmlDocGetRootElement(theManifest);

    // Check whether we have a document with the correct root (i.e. package)
    if (xmlStrcmp(rootElement->name, (const xmlChar *) "package"))
    {
        CL_ERRORPRINTF("Document of the wrong type, root node != package");
        xmlFreeDoc(theManifest);
        // return ER_MDS_XML_FORMAT;
        return -1;
    }

    cur = rootElement->children;
    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "index")))
        {
            CL_LOGPRINTF("Found node: index");
            indexNode = cur;
            break;
        }

        cur = cur->next;
    }
    
    return 0;
}

void erBrXmlFreeManifest()
{
    /* free the document */
    xmlFreeDoc(theManifest);

    /* Free the global variables that may have been allocated by the parser */
    xmlCleanupParser();
}


// Check if relative path is equal to the supplied full path (incl. filenames)
// Needs full path to manifest file to get the working dir
// Does not yet work with lower relative paths (e.g. ../../index.html)
static int url_compare(char *manifest, char *full_path, char *relative_file)
{
  int len1, len2;
  char *temp, *prefix_path, *local_path;

  BR_MANIFESTPRINTF("Comparing urls: %s and %s\n", full_path, relative_file);
  
  temp = strdup(manifest);
  prefix_path = dirname(temp);
  
/* TODO: 
     Need to work on this code some more... */
/*
  result = strtok(relative_file, "/");
  printf("(before)result=\"%s\"\n", result);
  while (strcmp(result, "..") == 0)
  {
    printf("(in while)result=\"%s\"\n", result);
    result = strtok(NULL, "/");
  }
  printf("(after)result=\"%s\"\n", result);
*/
  
  len1 = strlen(relative_file);
  len2 = strlen(prefix_path);
  
  // Remove ./ from beginning of relative_file
  if (strncmp(relative_file, "./", 2) == 0)
  {
    relative_file+=2;
  }
  
  local_path = (char*)malloc(sizeof(char)*(len1+len2+1));
  strcpy(local_path, prefix_path);
  strcat(local_path, "/");
  strcat(local_path, relative_file);
  
  if (strcmp(full_path, local_path) == 0)
  {
    // files are equal
    return 0;
  }
  else
  {
    // files are not equal
    return -1;
  }
}